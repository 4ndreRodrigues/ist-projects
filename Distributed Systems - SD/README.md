# TupleSpaces

Distributed Systems Project 2025

**Group A32**

*(choose one of the following levels and erase the other one)*  
**Difficulty level: I am Death incarnate!**


### Code Identification

In all source files (namely in the *groupId*s of the POMs), replace __GXX__ with your group identifier. The group
identifier consists of either A or T followed by the group number - always two digits. This change is important for 
code dependency management, to ensure your code runs using the correct components and not someone else's.

### Team Members

| Number | Name              | User                                | Email                                          |
|--------|-------------------|-------------------------------------|------------------------------------------------|
| 92429  | Barbara Martins   | <https://github.com/dockbdm>        | <mailto:barbara.martins@tecnico.ulisboa.pt>    |
| 99214  | Ema Estima        | <https://github.com/emaoliv>        | <mailto:ema.oliveira@tecnico.ulisboa.pt>       |
| 92421  | Andre Rodrigues   | <https://github.com/4ndreRodrigues> | <mailto:andrerodrigues2000@tecnico.ulisboa.pt> |

## Getting Started

The overall system is made up of several modules.
The definition of messages and services is in _Contract_.

See the [Project Statement](https://github.com/tecnico-distsys/Tuplespaces-2025) for a complete domain and system description.

### Prerequisites

The Project is configured with Java 17 (which is only compatible with Maven >= 3.8), but if you want to use Java 11 you
can too -- just downgrade the version in the POMs.

To confirm that you have them installed and which versions they are, run in the terminal:

```s
javac -version
mvn -version
```

### Installation

To compile and install all modules:

```s
mvn clean install
```

To create a virtual environment and install the packages:

```s
python -m venv .venv
python -m pip install grpcio
python -m pip install grpcio-tools
```

To generate code in Contract folder:

```s
mvn install
mvn exec:exec
```

## Built With

* [Maven](https://maven.apache.org/) - Build and dependency management tool;
* [gRPC](https://grpc.io/) - RPC framework.