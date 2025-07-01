package pt.ulisboa.tecnico.tuplespaces.frontend;

import java.util.ArrayList;

public class ResponseCollector {
    ArrayList<String> collectedResponses;

    public ResponseCollector() {
        collectedResponses = new ArrayList<>();
    }

    synchronized public void addString(String s) {
        collectedResponses.add(s);
        notifyAll();  // Acorda todas as threads que estão esperando respostas
    }

    synchronized public String getStrings() {
        String res = "";
        for (String s : collectedResponses) {
            res = res.concat(s);
        }
        return res;
    }

    synchronized public void waitUntilAllReceived(int n) throws InterruptedException {
        while (collectedResponses.size() < n) 
            wait();  // Espera até que todas as respostas tenham sido recebidas
    }
}
