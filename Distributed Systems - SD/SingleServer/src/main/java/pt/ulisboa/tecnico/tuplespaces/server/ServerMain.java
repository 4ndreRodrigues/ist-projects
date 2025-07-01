package pt.ulisboa.tecnico.tuplespaces.server;

import io.grpc.BindableService;
import io.grpc.Server;
import io.grpc.ServerBuilder;
import io.grpc.ServerInterceptors;

import java.io.IOException;

public class ServerMain {

    public static void main(String[] args)  throws IOException, InterruptedException {
      // TODO
      // receive and print arguments
		  System.out.printf("Received %d arguments%n", args.length);
		  for (int i = 0; i < args.length; i++) {
			  System.out.printf("arg[%d] = %s%n", i, args[i]);
	  	}

		  // check arguments
		  if (args.length < 1) {
			  System.err.println("Argument(s) missing!");
			  System.err.printf("Usage: java %s port%n", ServerMain.class.getName());
			  return;
		  }

      final int port = Integer.parseInt(args[0]);
		  final BindableService impl = new TupleSpacesImpl();

      // Create a new server to listen on port
		  Server server = ServerBuilder.forPort(port).addService(ServerInterceptors.intercept(impl, new MetadataInterceptor())).build();

		  try{
			  // Start the server
			  server.start();

			  // Server threads are running in the background.
			  System.out.println("Server started");

			  // Do not exit the main thread. Wait until server is terminated.
			  server.awaitTermination();
		
		  } catch (IOException e) {
        System.err.println("Failed to start server: " + e.getMessage());
      }

    }
}
