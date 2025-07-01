package pt.ulisboa.tecnico.tuplespaces.frontend;

import io.grpc.Metadata;
import io.grpc.Status;
import io.grpc.stub.MetadataUtils;
import io.grpc.stub.StreamObserver;
import pt.ulisboa.tecnico.tuplespaces.centralized.contract.TupleSpacesGrpc;
import pt.ulisboa.tecnico.tuplespaces.centralized.contract.TupleSpacesOuterClass;
import pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesGrpc;
import pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass;

import java.util.concurrent.CountDownLatch;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.List;
import java.util.ArrayList;

enum RequestType {
    PUT,
    READ,
    TAKE
}

public class FrontEndService extends TupleSpacesGrpc.TupleSpacesImplBase{

    private final ReplicatedTupleSpacesGrpc.ReplicatedTupleSpacesStub[] stubs;
    private final List<ClientRequest> requestsQueue = new ArrayList<>(); // Queue to store requests and keep track of their order

    private class ClientRequest {
        private final RequestType type; // Usando o enum RequestType
        private final int counter;
        String tuple;
    
        public ClientRequest(RequestType type, int counter, String tuple) {
            this.type = type;
            this.counter = counter;
            this.tuple = tuple; 
        }
    }

    // Constructor
    public FrontEndService(ReplicatedTupleSpacesGrpc.ReplicatedTupleSpacesStub[] stubs) {
        this.stubs = stubs;
    }

    private synchronized String getNextRequest(RequestType type) {
        Integer lowerCounter = null;
        String newTuple = null;
        for (ClientRequest nextRequest: requestsQueue) {
            if (nextRequest.type == type) {
                if (lowerCounter == null || lowerCounter > nextRequest.counter) {
                    lowerCounter = nextRequest.counter;
                } 
            }
        }
        if (lowerCounter != null) {
            for (ClientRequest nextRequest: requestsQueue) {
                if (nextRequest.type == type && nextRequest.counter == lowerCounter) {
                    newTuple = nextRequest.tuple;
                    requestsQueue.remove(nextRequest); // Remove the request from the queue
                    break;
                }
            }
        }
        System.out.println("Next request " + type + ": " + newTuple);
        
        return newTuple;
    }

    // Handle Put Requests from Client
    @Override
    public void put(TupleSpacesOuterClass.PutRequest request, StreamObserver<TupleSpacesOuterClass.PutResponse> responseObserver) {
        // Extract tuple from the request
        String newTuple = request.getNewTuple();
        int counter = request.getCounter();

        // Add the request to the queue
        synchronized (requestsQueue) {
            requestsQueue.add(new ClientRequest(RequestType.PUT, counter, newTuple));
        }  

        newTuple = getNextRequest(RequestType.PUT);

        // Create the corresponding PutRequest for ReplicatedTupleSpaces.proto
        ReplicatedTupleSpacesOuterClass.PutRequest replicatedRequest = ReplicatedTupleSpacesOuterClass.PutRequest.newBuilder()
                .setNewTuple(newTuple)
                .build();

        // Forward the request to the replicated servers
        forwardPutToServers(replicatedRequest, responseObserver);
    }

    // Forward the Put Request to the Replicated Servers
    private void forwardPutToServers(ReplicatedTupleSpacesOuterClass.PutRequest request, StreamObserver<TupleSpacesOuterClass.PutResponse> responseObserver) {
        CountDownLatch latch = new CountDownLatch(stubs.length);
        AtomicInteger successCount = new AtomicInteger(0);

        // Get delays from context
        String delayCsv = FrontEndMetadataInterceptor.DELAY_CTX_KEY.get();

        List<Integer> delays = parseDelays(delayCsv);

        responseObserver.onNext(TupleSpacesOuterClass.PutResponse.newBuilder().build());
        responseObserver.onCompleted();

        // Iterate over all servers
        for (int i = 0; i < stubs.length; i++) {
            int delay = (i < delays.size()) ? delays.get(i) : 0;
            Metadata metadata = new Metadata();
            metadata.put(Metadata.Key.of("delay", Metadata.ASCII_STRING_MARSHALLER), String.valueOf(delay));

            ReplicatedTupleSpacesGrpc.ReplicatedTupleSpacesStub delayedStub = MetadataUtils.attachHeaders(stubs[i], metadata);

            delayedStub.put(request, new StreamObserver<ReplicatedTupleSpacesOuterClass.PutResponse>() {
                @Override
                public void onNext(ReplicatedTupleSpacesOuterClass.PutResponse response) {
                    successCount.incrementAndGet();
                }

                @Override
                public void onError(Throwable t) {
                    System.err.println("Error in PUT request: " + t.getMessage());
                }

                @Override
                public void onCompleted() {
                    latch.countDown();
                }
            });
        }

        // Wait for the responses
        try {
            latch.await();
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            responseObserver.onError(Status.INTERNAL.withDescription("Interrupted while waiting for the response").asRuntimeException());
            return;
        }

        int quorum = stubs.length;
        if (successCount.get() >= quorum) {
    
         } else {
            responseObserver.onError(Status.INTERNAL.withDescription("Quorum not reached").asRuntimeException());
        }
    }

    // Handle Read Requests from Client (TupleSpaces.proto)
    @Override
    public void read(TupleSpacesOuterClass.ReadRequest request, StreamObserver<TupleSpacesOuterClass.ReadResponse> responseObserver) {

        String newSearchPattern = request.getSearchPattern();
        int counter = request.getCounter();

        // Log the request
        synchronized (requestsQueue) {
            requestsQueue.add(new ClientRequest(RequestType.READ, counter, newSearchPattern));
        }

        String newTuple = getNextRequest(RequestType.READ);

        ReplicatedTupleSpacesOuterClass.ReadRequest replicatedRequest = ReplicatedTupleSpacesOuterClass.ReadRequest.newBuilder()
                .setSearchPattern(newTuple)
                .build();

        // Forward the request to the replicated servers
        forwardReadToServers(replicatedRequest, responseObserver);
    }

    // Forward the Read Request to the Replicated Servers
    private void forwardReadToServers(ReplicatedTupleSpacesOuterClass.ReadRequest request, StreamObserver<TupleSpacesOuterClass.ReadResponse> responseObserver) {
        CountDownLatch latch = new CountDownLatch(1);
        AtomicInteger successCount = new AtomicInteger(0);
        AtomicBoolean responseSent = new AtomicBoolean(false); // Ensure only one response is sent

        // Get delays from context
        String delayCsv = FrontEndMetadataInterceptor.DELAY_CTX_KEY.get();

        List<Integer> delays = parseDelays(delayCsv);

        // Iterate over all servers
        for (int i = 0; i < stubs.length; i++) {
            int delay = (i < delays.size()) ? delays.get(i) : 0;
            Metadata metadata = new Metadata();
            metadata.put(Metadata.Key.of("delay", Metadata.ASCII_STRING_MARSHALLER), String.valueOf(delay));

            ReplicatedTupleSpacesGrpc.ReplicatedTupleSpacesStub delayedStub = MetadataUtils.attachHeaders(stubs[i], metadata);

            delayedStub.read(request, new StreamObserver<ReplicatedTupleSpacesOuterClass.ReadResponse>() {
                @Override
                public void onNext(ReplicatedTupleSpacesOuterClass.ReadResponse response) {
                    successCount.incrementAndGet();
                    if (responseSent.compareAndSet(false, true)) {
                        responseObserver.onNext(TupleSpacesOuterClass.ReadResponse.newBuilder()
                            .setResult(response.getResult())  // Set the result from the first successful response
                            .build());
                        responseObserver.onCompleted();  // Complete the response after the first success
                    } else {
                        // If a response has already been sent, ignore any subsequent responses
                        System.out.println("Duplicate response received, ignoring it.");
                    }
                }

                @Override
                public void onError(Throwable t) {
                    System.err.println("Error in READ request: " + t.getMessage());
                }

                @Override
                public void onCompleted() {
                    latch.countDown();
                }
            });
        }

        // Wait for the responses
        try {
            latch.await();
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            responseObserver.onError(Status.INTERNAL.withDescription("Interrupted while waiting for the response").asRuntimeException());
            return;
        }

        // If quorum is met, return response to client
        if (successCount.get() > 0) {
            //responseObserver.onCompleted();
        } else {
            responseObserver.onError(Status.INTERNAL.withDescription("No successful read operation").asRuntimeException());
        }
    }

    // Handle Take Requests from Client (TupleSpaces.proto)
    @Override
    public void take(TupleSpacesOuterClass.TakeRequest request, StreamObserver<TupleSpacesOuterClass.TakeResponse> responseObserver) {

        String newSearchPattern = request.getSearchPattern();
        int clientId = request.getClient();
        int counter = request.getCounter();

        // Log the request
        synchronized (requestsQueue) {
            requestsQueue.add(new ClientRequest(RequestType.TAKE, counter, newSearchPattern));
        }

        String newTuple = getNextRequest(RequestType.TAKE);

        ReplicatedTupleSpacesOuterClass.TakeRequest replicatedRequest = ReplicatedTupleSpacesOuterClass.TakeRequest.newBuilder()
                .setSearchPattern(newTuple)
                .build();

        // Forward the request to the replicated servers
        forwardTakeToServers(replicatedRequest, responseObserver, clientId);
    }

    // Forward the Take Request to the Replicated Servers
    private void forwardTakeToServers(ReplicatedTupleSpacesOuterClass.TakeRequest request, StreamObserver<TupleSpacesOuterClass.TakeResponse> responseObserver, Integer clientId) {
       
        // Determine the voter set based on the clientId
        int[] voterSet = {clientId % 3, (clientId + 1) % 3}; 

        String searchPattern = request.getSearchPattern();
        List<List<String>> allMatchingTuples = new ArrayList<>();  // Store results from all replicas

        CountDownLatch voteLatch = new CountDownLatch(2);   // Wait for responses from both replicas

        // Get delays from context
        String delayCsv = FrontEndMetadataInterceptor.DELAY_CTX_KEY.get();
        List<Integer> delays = parseDelays(delayCsv);

        // Step 1: Enviar request ao voterSet e recolher tuplos candidatos
        for (int replicaId : voterSet) {
            int delay = delays.get(replicaId);
            Metadata metadata = new Metadata();
            metadata.put(Metadata.Key.of("delay", Metadata.ASCII_STRING_MARSHALLER), String.valueOf(delay));

            ReplicatedTupleSpacesOuterClass.ReqRequest voteRequest = ReplicatedTupleSpacesOuterClass.ReqRequest.newBuilder()
                    .setClientId(clientId).setSearchPattern(searchPattern)
                    .build();

            ReplicatedTupleSpacesGrpc.ReplicatedTupleSpacesStub delayedStub = MetadataUtils.attachHeaders(stubs[replicaId], metadata);
            
            delayedStub.request(voteRequest, new StreamObserver<ReplicatedTupleSpacesOuterClass.ReqResponse>() {
                @Override
                public void onNext(ReplicatedTupleSpacesOuterClass.ReqResponse response) {
                    allMatchingTuples.add(response.getMatchingTuplesList());
                    for (List<String> allMatchingTuple : allMatchingTuples) {
                        System.out.println("Matching tuple from replica " + replicaId + ": " + allMatchingTuple);
                    }
                }

                @Override
                public void onError(Throwable t) {
                    System.err.println("Error in TAKE request: " + t.getMessage());
                    voteLatch.countDown();  // Ensure latch is counted down even on error
                }

                @Override
                public void onCompleted() {
                    voteLatch.countDown();
                }
            });  
        }

        // Step 2: Wait for responses from all replicas
        try {
            voteLatch.await();  // Wait for both replicas' responses
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            responseObserver.onError(Status.ABORTED.withDescription("Interrupted while waiting for votes").asRuntimeException());
            e.printStackTrace();
            return;
        }

        if (allMatchingTuples.size() < voterSet.length) {
            responseObserver.onError(Status.ABORTED.withDescription("Not enough votes received").asRuntimeException());
            return;
        }

        List<String> intersection = new ArrayList<>(allMatchingTuples.get(0));  // Start with the first replica's results
        intersection.retainAll(allMatchingTuples.get(1));  // Retain only the common elements with the second replica
        if (intersection.isEmpty()) {
            responseObserver.onNext(TupleSpacesOuterClass.TakeResponse.newBuilder().setResult("No matching tuple found").build());
            responseObserver.onCompleted();
            return;
        }
        System.out.println("Both replicas granted access. Performing take operation...");
        
        //Choose tuple to remove
        ReplicatedTupleSpacesOuterClass.TakeRequest takeRequest = ReplicatedTupleSpacesOuterClass.TakeRequest
            .newBuilder()
            .setSearchPattern(intersection.get(0))
            .setClientId(clientId)
            .build();

        CountDownLatch takeLatch = new CountDownLatch(3);
        List<String> results = new ArrayList<>(); 

        // Step 3: Check if both replicas responded and proceed with the 'take' operation
        for (int i = 0; i < 3; i++) {
            int delay = delays.get(i);
            Metadata metadata = new Metadata();
            metadata.put(Metadata.Key.of("delay", Metadata.ASCII_STRING_MARSHALLER), String.valueOf(delay));
            ReplicatedTupleSpacesGrpc.ReplicatedTupleSpacesStub delayedStub = MetadataUtils.attachHeaders(stubs[i], metadata);
            
            delayedStub.take(takeRequest, new StreamObserver<ReplicatedTupleSpacesOuterClass.TakeResponse>() {
                @Override
                public void onNext(ReplicatedTupleSpacesOuterClass.TakeResponse response) {
                    synchronized (results) {
                        results.add(response.getResult());
                    }
                }
                @Override
                public void onError(Throwable t) {
                    System.err.println("Error in TAKE request: " + t.getMessage());
                    takeLatch.countDown();  // Ensure latch is counted down even on error
                    responseObserver.onError(t);  // Propagate the error to the responseObserver
                }
                @Override
                public void onCompleted() {
                    takeLatch.countDown();
                }
            });
        }
        
        // Step 4: Wait for all replicas to respond
        try {
            takeLatch.await();  // Wait for all replicas to respond
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            responseObserver.onError(Status.INTERNAL.withDescription("Interrupted while waiting for the response").asRuntimeException());
            return;
        }

        // Step 5: Send the response to the client
        String finalResult = results.isEmpty() ? "No matching tuple found" : results.get(0);  // Get the first result
        responseObserver.onNext(TupleSpacesOuterClass.TakeResponse.newBuilder().setResult(finalResult).build());
        responseObserver.onCompleted();  // Complete the response
    }


    // Handle getTupleSpacesState Requests from Client (TupleSpaces.proto)
    @Override
    public void getTupleSpacesState(TupleSpacesOuterClass.GetTupleSpacesStateRequest request, StreamObserver<TupleSpacesOuterClass.GetTupleSpacesStateResponse> responseObserver) {

        ReplicatedTupleSpacesOuterClass.GetTupleSpacesStateRequest replicatedRequest = ReplicatedTupleSpacesOuterClass.GetTupleSpacesStateRequest.newBuilder()
                .build();

        // Forward the request to the replicated servers
        forwardGetTupleSpacesStateToServers(replicatedRequest, responseObserver);
    }

    // Forward the GetTupleSpacesState Request to the Replicated Servers
    private void forwardGetTupleSpacesStateToServers(ReplicatedTupleSpacesOuterClass.GetTupleSpacesStateRequest request, StreamObserver<TupleSpacesOuterClass.GetTupleSpacesStateResponse> responseObserver) {
        // Initialize a latch to wait for responses from replicas (only need one successful response)
        CountDownLatch latch = new CountDownLatch(1);
        AtomicBoolean success = new AtomicBoolean(false);

        // Iterate over all replica servers
        for (int i = 0; i < stubs.length; i++) {
            ReplicatedTupleSpacesGrpc.ReplicatedTupleSpacesStub replicaStub = MetadataUtils.attachHeaders(stubs[i], new Metadata());

            // Asynchronously forward the request to each replica
            replicaStub.getTupleSpacesState(request, new StreamObserver<ReplicatedTupleSpacesOuterClass.GetTupleSpacesStateResponse>() {
                @Override
                public void onNext(ReplicatedTupleSpacesOuterClass.GetTupleSpacesStateResponse response) {
                    if (success.compareAndSet(false, true)) {  // Make sure this block runs ONCE
                        TupleSpacesOuterClass.GetTupleSpacesStateResponse finalResponse = TupleSpacesOuterClass.GetTupleSpacesStateResponse.newBuilder()
                            .addAllTuple(response.getTupleList())
                            .build();

                        responseObserver.onNext(finalResponse);   // Send the result
                        responseObserver.onCompleted();          // End the stream
                    }
                }

                @Override
                public void onError(Throwable t) {
                    System.err.println("Error in GetTupleSpacesState request: " + t.getMessage());
                }

                @Override
                public void onCompleted() {
                    latch.countDown();  // Ensure latch is counted down even if no successful response
                }
            });
        }

        // Wait for at least one successful response
        try {
            latch.await();  // Wait for any replica to respond
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            responseObserver.onError(Status.INTERNAL.withDescription("Interrupted while waiting for the response").asRuntimeException());
        }

        // If no replica responded successfully, return an error
        if (!success.get()) {
            responseObserver.onError(Status.INTERNAL.withDescription("No successful GetTupleSpacesState operation").asRuntimeException());
        }
    }

    private List<Integer> parseDelays(String delayCsv) {
        List<Integer> delays = new ArrayList<>();
        if (delayCsv == null || delayCsv.isEmpty()) return delays;

        String[] parts = delayCsv.split(",");
        for (String part : parts) {
            try {
                delays.add(Integer.parseInt(part.trim()));
            } catch (NumberFormatException e) {
                delays.add(0); // Default to 0 on error
            }
        }

        return delays;
    }

}
