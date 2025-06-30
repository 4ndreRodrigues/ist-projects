package pt.ulisboa.tecnico.tuplespaces.client.grpc;

import io.grpc.Metadata;
import io.grpc.stub.MetadataUtils;
import io.grpc.ManagedChannel;
import io.grpc.ManagedChannelBuilder;
import io.grpc.stub.StreamObserver;
import io.grpc.StatusRuntimeException;
import java.util.List;
import java.util.concurrent.CountDownLatch;

import pt.ulisboa.tecnico.tuplespaces.centralized.contract.TupleSpacesGrpc;
import pt.ulisboa.tecnico.tuplespaces.centralized.contract.TupleSpacesOuterClass;


public class ClientService {

    private final ManagedChannel channel;
    private TupleSpacesGrpc.TupleSpacesStub stub;
    private int client_id;
    private int counter;


    public ClientService(String host_port, int client_id) {
        this.client_id = client_id;
        this.counter = 0;
        this.channel = ManagedChannelBuilder.forTarget(host_port).usePlaintext().build();
        // Create a stub for the service
        this.stub = TupleSpacesGrpc.newStub(channel);
    }

    public void closeChannel(){
        this.channel.shutdownNow();
    }

    private io.grpc.Metadata.Key<String> createDelayMetadataKey() {
        return Metadata.Key.of("delay", Metadata.ASCII_STRING_MARSHALLER);
    }

    public void read(String tuple, String delay) {
        counter++;
        // Add delay to metadata
        Metadata metadata = new Metadata();
        metadata.put(createDelayMetadataKey(), delay);
        TupleSpacesGrpc.TupleSpacesStub delayedStub = MetadataUtils.attachHeaders(stub, metadata);
        TupleSpacesOuterClass.ReadRequest request = TupleSpacesOuterClass.ReadRequest.newBuilder()
                .setSearchPattern(tuple)
                .setCounter(counter)
                .build();

        // Create a CountDownLatch to wait for the operation to complete
        CountDownLatch latch = new CountDownLatch(1);

       // Make asynchronous call with StreamObserver
        delayedStub.read(request, new StreamObserver<TupleSpacesOuterClass.ReadResponse>() {
            @Override
            public void onNext(TupleSpacesOuterClass.ReadResponse response) {
                System.out.println("OK\n" + response.getResult() + "\n");
            }

            @Override
            public void onError(Throwable t) {
                System.out.println("Caught exception with description: " + t.getMessage());
            }

            @Override
            public void onCompleted() {
                latch.countDown();  // Count down to unblock the main thread
            }
        });

        // Wait for the response to complete (or timeout if needed)
        try {
            latch.await();  // Blocks until the latch count reaches 0
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    public void put(String tuple, String delay) {
        counter++;
        // Add delay to metadata
        Metadata metadata = new Metadata();
        metadata.put(createDelayMetadataKey(), delay);
        TupleSpacesGrpc.TupleSpacesStub delayedStub = MetadataUtils.attachHeaders(stub, metadata);
        TupleSpacesOuterClass.PutRequest request = TupleSpacesOuterClass.PutRequest.newBuilder()
                .setNewTuple(tuple)
                .setCounter(counter)
                .build();


        // Create a CountDownLatch to wait for the operation to complete
        CountDownLatch latch = new CountDownLatch(1);

        // Make asynchronous call with StreamObserver
        delayedStub.put(request, new StreamObserver<TupleSpacesOuterClass.PutResponse>() {
            @Override
            public void onNext(TupleSpacesOuterClass.PutResponse response) {
                System.out.println("OK\n");
                latch.countDown();
            }

            @Override
            public void onError(Throwable t) {
                System.out.println("Caught exception with description: " + t.getMessage());
                latch.countDown();
            }

            @Override
            public void onCompleted() {
                latch.countDown();  // Count down to unblock the main thread
            }
        });

        // Wait for the response to complete (or timeout if needed)
        try {
            latch.await();  // Blocks until the latch count reaches 0
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    public void take(String tuple, String delay){
        counter++;
        // Add delay to metadata
        Metadata metadata = new Metadata();
        metadata.put(createDelayMetadataKey(), delay);
        TupleSpacesGrpc.TupleSpacesStub delayedStub = MetadataUtils.attachHeaders(stub, metadata);
        TupleSpacesOuterClass.TakeRequest request = TupleSpacesOuterClass.TakeRequest.newBuilder()
                .setSearchPattern(tuple)
                .setClient(client_id)  // Send the client ID
                .setCounter(counter)
                .build();
        
        // Create a CountDownLatch to wait for the operation to complete
        CountDownLatch latch = new CountDownLatch(1);

        // Make asynchronous call with StreamObserver
        delayedStub.take(request, new StreamObserver<TupleSpacesOuterClass.TakeResponse>() {
            @Override
            public void onNext(TupleSpacesOuterClass.TakeResponse response) {
                System.out.println("OK\n" + response.getResult() + "\n");
            }

            @Override
            public void onError(Throwable t) {
                System.out.println("Caught exception with description: " + t.getMessage());
            }

            @Override
            public void onCompleted() {
                latch.countDown();  // Count down to unblock the main thread
            }
        });

        // Wait for the response to complete (or timeout if needed)
        try {
            latch.await();  // Blocks until the latch count reaches 0
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    public void getTupleSpacesState(){
        TupleSpacesOuterClass.GetTupleSpacesStateRequest request = TupleSpacesOuterClass.GetTupleSpacesStateRequest.newBuilder().build();

        // Create a CountDownLatch to wait for the operation to complete
        CountDownLatch latch = new CountDownLatch(1);
        
        // Create a StreamObserver for handling the response asynchronously
        StreamObserver<TupleSpacesOuterClass.GetTupleSpacesStateResponse> responseObserver = new StreamObserver<TupleSpacesOuterClass.GetTupleSpacesStateResponse>() {
            @Override
            public void onNext(TupleSpacesOuterClass.GetTupleSpacesStateResponse response) {
                List<String> tuples = response.getTupleList();

                System.out.print("OK\n[");
                if (!tuples.isEmpty()) {
                    // Print the first element
                    System.out.print(tuples.get(0));

                    // Print the rest of the list
                    for (int i = 1; i < tuples.size(); i++) {
                        System.out.print(", " + tuples.get(i));
                    }
                }
                System.out.println("]\n");
            }

            @Override
            public void onError(Throwable t) {
                System.out.println("Error occurred while getting tuple space state: " + t.getMessage());
                latch.countDown();
            }

            @Override
            public void onCompleted() {
                latch.countDown();  // Count down to unblock the main thread
            };
        };

        try {
            // Make the call with the request and the response observer
            stub.getTupleSpacesState(request, responseObserver);
            latch.await();
        } catch (StatusRuntimeException e) {
            System.out.println("Caught exception with description: " + e.getStatus().getDescription());
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt(); // Best practice
            System.out.println("Thread was interrupted while waiting: " + e.getMessage());
        }
    }
}
