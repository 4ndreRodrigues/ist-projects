package pt.ulisboa.tecnico.tuplespaces.server.domain;

import java.util.ArrayList;
import java.util.List;

public class ServerState {

  private List<String> tuples;
  private final List<String> lockedTuples = new ArrayList<>();

  public ServerState() {
    this.tuples = new ArrayList<String>();
  }

  public synchronized void put(String tuple) {
    tuples.add(tuple);
    notifyAll();  // Wake up waiting clients
  }

  private String getMatchingTuple(String pattern) {
    for (String tuple : this.tuples) {
      if (tuple.matches(pattern)) {
        return tuple;
      }
    }
    return null;
  }

  public synchronized String read(String pattern) {
    String tuple;
    while (true) {
      tuple = getMatchingTuple(pattern);
      if(tuple!=null){
        return tuple;
      } 
      try {
          wait();  // Wait until a tuple is available
      } catch (InterruptedException e) {
          Thread.currentThread().interrupt();
      }
    }
  }

  private String deleteMatchingTuple(String pattern){
    for (int i = 0; i < tuples.size(); i++) {
      if (tuples.get(i).matches(pattern)) {
        return tuples.remove(i); // Remove and return the tuple
      }
    }
    return null; // No match found
  }

  public synchronized String take(String pattern) {
    String tuple;
    
    while (true) {
      tuple = deleteMatchingTuple(pattern);
      if(tuple!=null){
        return tuple;
      } 
      try {
          wait();  // Wait until a tuple is available
      } catch (InterruptedException e) {
          Thread.currentThread().interrupt();
      }
    }
  }

  public synchronized void handleRequest(String searchPattern) {
    while (lockedTuples.contains(searchPattern)) {
      try {
        wait();  // Wait until the tuple is released
      } catch (InterruptedException e) {
        Thread.currentThread().interrupt();
      }
    }
  }

  public synchronized List<String> lockPattern(String pattern) {
    List<String> locked = new ArrayList<>();
    for (String tuple : tuples) {
      if (tuple.matches(pattern)) {
        locked.add(tuple);
        lockedTuples.add(tuple);
      }
    }
    return locked;
  }

  public synchronized void unlockTuples() {
    lockedTuples.clear();
    notifyAll();  // Notify waiting threads
  }

  public List<String> getTupleSpacesState() {
    return new ArrayList<>(tuples); // Return a copy to prevent external modification
  }
}

