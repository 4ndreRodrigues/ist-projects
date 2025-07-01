package pt.ulisboa.tecnico.tuplespaces.frontend;

import io.grpc.ManagedChannel;
import io.grpc.ManagedChannelBuilder;
import io.grpc.Server;
import io.grpc.ServerBuilder;
import pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesGrpc;


import java.io.IOException;
import java.util.concurrent.TimeUnit;

public class FrontEnd {
    private final int clientPort; // The port the frontend gRPC server listens on
    private ManagedChannel[] channels; // Channels to communicate with the tuple space servers
    private ReplicatedTupleSpacesGrpc.ReplicatedTupleSpacesStub[] stubs; // Stubs to invoke methods on servers
    private Server grpcServer; // The gRPC server that handles client requests

    // Constructor for FrontEnd, initializes channels and stubs to connect to 3 servers
    public FrontEnd(int clientPort, String[] server_1, String[] server_2, String[] server_3) {
        this.clientPort = clientPort;
        this.channels = new ManagedChannel[3];
        this.stubs = new ReplicatedTupleSpacesGrpc.ReplicatedTupleSpacesStub[3];

        // Create channels to connect to the servers
        this.channels[0] = ManagedChannelBuilder.forAddress(server_1[0], Integer.parseInt(server_1[1])).usePlaintext().build();
        this.channels[1] = ManagedChannelBuilder.forAddress(server_2[0], Integer.parseInt(server_2[1])).usePlaintext().build();
        this.channels[2] = ManagedChannelBuilder.forAddress(server_3[0], Integer.parseInt(server_3[1])).usePlaintext().build();

        // Create stubs to communicate with the servers
        this.stubs[0] = ReplicatedTupleSpacesGrpc.newStub(channels[0]);
        this.stubs[1] = ReplicatedTupleSpacesGrpc.newStub(channels[1]);
        this.stubs[2] = ReplicatedTupleSpacesGrpc.newStub(channels[2]);
    }

    // Starts the FrontEnd gRPC server to handle client requests
    public void start() throws IOException {
        try {
            // Initialize the server to listen for client requests
            grpcServer = ServerBuilder.forPort(clientPort)
                    .addService(new FrontEndService(stubs)) // Add the service to handle client requests
                    .intercept(new FrontEndMetadataInterceptor())  // Add the Metadata Interceptor
                    .build()
                    .start();

            System.out.println("FrontEnd initialized on port " + clientPort);
            System.out.println("Connected to TupleSpaces");

            // Add a shutdown hook to cleanly stop the server and channels when the application is terminated
            Runtime.getRuntime().addShutdownHook(new Thread(() -> {
                try {
                    FrontEnd.this.stop();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }));
        } catch (IOException e) {
            throw new IOException("Failed to start FrontEnd: " + e.getMessage(), e);
        }
    }

    // Stops the FrontEnd gRPC server and shuts down the channels
    public void stop() throws InterruptedException {
        if (grpcServer != null) {
            grpcServer.shutdown().awaitTermination(5, TimeUnit.SECONDS); // Wait for server to shut down
        }

        // Shutdown channels
        for (ManagedChannel channel : channels) {
            if (channel != null) {
                channel.shutdown().awaitTermination(5, TimeUnit.SECONDS); // Wait for channels to shut down
            }
        }
    }

    // Blocks the main thread until the server is shut down
    public void blockUntilShutdown() throws InterruptedException {
        if (grpcServer != null) {
            grpcServer.awaitTermination(); // Block until the server terminates
        }
    }
}