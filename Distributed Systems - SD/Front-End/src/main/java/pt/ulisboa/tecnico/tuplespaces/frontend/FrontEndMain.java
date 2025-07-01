package pt.ulisboa.tecnico.tuplespaces.frontend;

import java.io.IOException;

public class FrontEndMain {

    public static void main(String[] args) throws IOException, InterruptedException {

        System.out.println(FrontEndMain.class.getSimpleName());

        System.out.printf("Received %d arguments%n", args.length);
        for (int i = 0; i < args.length; i++) {
            System.out.printf("arg[%d] = %s%n", i, args[i]);
        }

        if (args.length != 4) {
            System.err.println("Argument(s) missing!");
            System.err.println("Usage: mvn exec:java -Dexec.args=\"<client_port> <server1_host:port> <server2_host:port> <server3_host:port>\"");
            return;
        }

        final int clientPort = Integer.parseInt(args[0]);

        // Parse replica server info
        String[] serverInfo1 = args[1].split(":");
        String[] serverInfo2 = args[2].split(":");
        String[] serverInfo3 = args[3].split(":");

        if (serverInfo1.length != 2 || serverInfo2.length != 2 || serverInfo3.length != 2) {
            System.err.println("Invalid server info format! Expected <host>:<port>");
            return;
        }

        try {
            FrontEnd frontend = new FrontEnd(
                clientPort,
                serverInfo1,
                serverInfo2,
                serverInfo3
            );

            frontend.start();

            System.out.println("FrontEnd started on port " + clientPort);

            frontend.blockUntilShutdown();

        } catch (IOException e) {
            System.err.println("Failed to start FrontEnd: " + e.getMessage());
        }
    }
}