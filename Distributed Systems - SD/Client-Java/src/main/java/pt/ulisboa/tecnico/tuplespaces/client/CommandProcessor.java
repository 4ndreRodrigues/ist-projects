package pt.ulisboa.tecnico.tuplespaces.client;

import pt.ulisboa.tecnico.tuplespaces.client.grpc.ClientService;

import java.util.Scanner;

public class CommandProcessor {

    private static final String SPACE = " ";
    private static final String BGN_TUPLE = "<";
    private static final String END_TUPLE = ">";
    private static final String PUT = "put";
    private static final String READ = "read";
    private static final String TAKE = "take";
    private static final String SLEEP = "sleep";
    private static final String EXIT = "exit";
    private static final String GET_TUPLE_SPACES_STATE = "getTupleSpacesState";

    private final ClientService clientService;

    public CommandProcessor(ClientService clientService) {
        this.clientService = clientService;
    }

    void parseInput() {

        Scanner scanner = new Scanner(System.in);
        boolean exit = false;

        while (!exit) {
            System.out.print("> ");
            String line = scanner.nextLine().trim();
            String[] split = line.split(SPACE);
             switch (split[0]) {
                case PUT:
                    this.put(split);
                    break;

                case READ:
                    this.read(split);
                    break;

                case TAKE:
                    this.take(split);
                    break;

                case GET_TUPLE_SPACES_STATE:
                    this.getTupleSpacesState();
                    break;

                case SLEEP:
                    this.sleep(split);
                    break;

                case EXIT:
                    exit = true;
                    break;

                default:
                    this.printUsage();
                    break;
             }
        }
        scanner.close();
        clientService.closeChannel();
    }


    private void put(String[] split){
        if (!this.inputIsValid(split)) {
            this.printUsage();
            return;
        }

        String tuple = split[1];

        String delay;
        if (split.length > 2) {
            delay = split[2] + "," + split[3] + "," + split[4];
        }
        else{
            delay="0,0,0";
        }

        // Now pass delay to the clientService.put method
        clientService.put(tuple, delay);
    }


    private void read(String[] split){
        // check if input is valid
        if (!this.inputIsValid(split)) {
            this.printUsage();
            return;
        }
        
        // get the tuple
        String tuple = split[1];

        String delay;
        if (split.length > 2) {
            delay = split[2]+","+split[3]+","+split[4];
        }

        else{
            delay = "0,0,0";
        }

        // read the tuple
        //System.out.println("TODO: implement read command");
        clientService.read(tuple, delay);
    }

private void take(String[] split){
         // check if input is valid
        if (!this.inputIsValid(split)) {
            this.printUsage();
            return;
        }
        
        // get the tuple
        String tuple = split[1];
        
        String delay;
        if (split.length > 2) {
            delay = split[2]+","+split[3]+","+split[4];
        }

        else{
            delay = "0,0,0";
        }

        // take the tuple
        //System.out.println("TODO: implement take command");
        clientService.take(tuple, delay);
    }

    private void getTupleSpacesState(){

        // get the tuple spaces state
        //System.out.println("TODO: implement getTupleSpacesState command");
        clientService.getTupleSpacesState();

    }

    private void sleep(String[] split) {
      if (split.length != 2){
        this.printUsage();
        return;
      }
      Integer time;

      // checks if input String can be parsed as an Integer
      try {
         time = Integer.parseInt(split[1]);
      } catch (NumberFormatException e) {
        this.printUsage();
        return;
      }

      try {
        Thread.sleep(time*1000);
      } catch (InterruptedException e) {
        throw new RuntimeException(e);
      }
    }

    private void printUsage() {
        System.out.println("Usage:\n" +
                "- put <element[,more_elements]> [delay]\n" +  // Add the delay argument here
                "- read <element[,more_elements]> [delay]\n" +
                "- take <element[,more_elements]>\n" +
                "- getTupleSpacesState <server>\n" +
                "- sleep <integer>\n" +
                "- exit\n");
    }

    private boolean inputIsValid(String[] input){
        if (input.length < 2 
            ||
            !input[1].substring(0,1).equals(BGN_TUPLE) 
            || 
            !input[1].endsWith(END_TUPLE)
            || 
            input.length > 5
            ) {
            return false;
        }
        else {
            return true;
        }
    }
}
