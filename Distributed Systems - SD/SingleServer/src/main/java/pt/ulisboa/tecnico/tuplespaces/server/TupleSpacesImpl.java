package pt.ulisboa.tecnico.tuplespaces.server;

import io.grpc.Metadata;
import io.grpc.stub.StreamObserver;
import io.grpc.Context;

import pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesGrpc;
import pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass;
import pt.ulisboa.tecnico.tuplespaces.server.domain.ServerState;

import java.util.List;
import java.util.concurrent.TimeUnit;

public class TupleSpacesImpl extends ReplicatedTupleSpacesGrpc.ReplicatedTupleSpacesImplBase {

    private final ServerState serverState;
    public static final Context.Key<Metadata> METADATA_KEY = Context.key("metadata_key");

    public TupleSpacesImpl() {
        this.serverState = new ServerState(); // Initialize server state
    }

    @Override
    public void put(ReplicatedTupleSpacesOuterClass.PutRequest request, StreamObserver<ReplicatedTupleSpacesOuterClass.PutResponse> responseObserver) {
        // Extract delay from metadata
        int delay = extractDelayFromMetadata();

        // Apply delay before processing the put operation
        simulateDelay(delay);

        serverState.put(request.getNewTuple());

        System.out.printf("put done tuple: %s%n", request.getNewTuple());

        responseObserver.onNext(ReplicatedTupleSpacesOuterClass.PutResponse.newBuilder().build()); // Empty response
        responseObserver.onCompleted();
    }

    @Override
    public void read(ReplicatedTupleSpacesOuterClass.ReadRequest request, StreamObserver<ReplicatedTupleSpacesOuterClass.ReadResponse> responseObserver) {
        // Extract delay from metadata
        int delay = extractDelayFromMetadata();

        // Apply delay before processing the read operation
        simulateDelay(delay);

        String result = serverState.read(request.getSearchPattern());

        if (result != null) {
            responseObserver.onNext(ReplicatedTupleSpacesOuterClass.ReadResponse.newBuilder().setResult(result).build());
        } else {
            responseObserver.onNext(ReplicatedTupleSpacesOuterClass.ReadResponse.newBuilder().setResult("No matching tuple found").build());
        }
        responseObserver.onCompleted();
    }

    @Override
    public void take(ReplicatedTupleSpacesOuterClass.TakeRequest request, StreamObserver<ReplicatedTupleSpacesOuterClass.TakeResponse> responseObserver) {
        // Extract delay from metadata
        int delay = extractDelayFromMetadata();

        // Apply delay before processing the take operation
        simulateDelay(delay);

        int clientId = request.getClientId();
        String pattern = request.getSearchPattern();

        String result = serverState.take(pattern);
        ReplicatedTupleSpacesOuterClass.TakeResponse response = ReplicatedTupleSpacesOuterClass.TakeResponse.newBuilder().setResult(result).build();
        System.out.printf("Releasing critical section from client %d%n", clientId);
        responseObserver.onNext(response);
        serverState.unlockTuples();

        responseObserver.onCompleted();
    }

    @Override
    public void request(ReplicatedTupleSpacesOuterClass.ReqRequest request, StreamObserver<ReplicatedTupleSpacesOuterClass.ReqResponse> responseObserver) {
        int clientId = request.getClientId();
        String searchPattern = request.getSearchPattern();
        System.out.printf("Requesting critical section from client %d%n", clientId);
        
        serverState.handleRequest(searchPattern);
        List<String> lockedTuples = serverState.lockPattern(searchPattern);

        System.out.printf("Na seccao critica do client: %d", clientId);

        ReplicatedTupleSpacesOuterClass.ReqResponse.Builder responseBuilder = ReplicatedTupleSpacesOuterClass.ReqResponse.newBuilder();
        for (String tuple : lockedTuples) {
            responseBuilder.addMatchingTuples(tuple);
        }
        responseObserver.onNext(responseBuilder.build());
        responseObserver.onCompleted();
    }


    @Override
    public void getTupleSpacesState(ReplicatedTupleSpacesOuterClass.GetTupleSpacesStateRequest request, StreamObserver<ReplicatedTupleSpacesOuterClass.GetTupleSpacesStateResponse> responseObserver) {
        List<String> tuples = serverState.getTupleSpacesState();

        ReplicatedTupleSpacesOuterClass.GetTupleSpacesStateResponse.Builder responseBuilder = ReplicatedTupleSpacesOuterClass.GetTupleSpacesStateResponse.newBuilder();
        responseBuilder.addAllTuple(tuples);

        responseObserver.onNext(responseBuilder.build());
        responseObserver.onCompleted();
    }


    private int extractDelayFromMetadata() {
        // Extract delay from the current gRPC context metadata
        Metadata metadata = METADATA_KEY.get();

        if (metadata != null) {
            String delayStr = metadata.get(Metadata.Key.of("delay", Metadata.ASCII_STRING_MARSHALLER));
            try {
                if (delayStr != null) {
                    return Integer.parseInt(delayStr);
                }
            } catch (NumberFormatException e) {
                return 0; // In case of failure to parse delay, return 0 (no delay)
            }
        }
        return 0; // Default to no delay
    }

    private void simulateDelay(int delay) {
        try {
            if (delay > 0) {
                System.out.println("Received delay metadata: " + delay);
                TimeUnit.SECONDS.sleep(delay); // Simulate delay by sleeping the thread
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
}