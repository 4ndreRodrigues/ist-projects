import sys
sys.path.insert(1, '../Contract/target/generated-sources/protobuf/python')

import grpc
import TupleSpaces_pb2 as pb2
import TupleSpaces_pb2_grpc as pb2_grpc

class ClientService:
    def __init__(self, host_port: str, client_id: int):
        pass
        # Create channel/stub for each server
        self.channel = grpc.insecure_channel(host_port)
        self.stub = pb2_grpc.TupleSpacesStub(self.channel)

    # Implement individual methods for each remote operation of the TupleSpaces service
    def put(self, tuple_data: str):
        request = pb2.PutRequest(newTuple=tuple_data)
        try:
            self.stub.put(request)
            print("OK\n")
        except grpc.RpcError as e:
            print(f"Caught exception with description: {e.details()}")

    def read(self, pattern: str):
        request = pb2.ReadRequest(searchPattern=pattern)
        try:
            response = self.stub.read(request)
            print(f"OK\n{response.result}\n")
        except grpc.RpcError as e:
            print(f"Caught exception with description: {e.details()}")
    
    def take(self, pattern: str):
        request = pb2.TakeRequest(searchPattern=pattern)
        try:
            response = self.stub.take(request)
            print(f"OK\n{response.result}\n")
        except grpc.RpcError as e:
            print(f"Caught exception with description: {e.details()}")

    def getTupleSpacesState(self):  
        request = pb2.GetTupleSpacesStateRequest()
        try:
            response = self.stub.getTupleSpacesState(request)
            tuples = response.tuple 

            print("OK\n[", end="")
            if tuples:
                print(tuples[0], end="")
                for tuple in tuples[1:]:
                    print(f", {tuple}", end="")
            print("]\n")

        except grpc.RpcError as e:
            print(f"Caught exception with description: {e.details()}")

    def closechannel(self):
        self.channel.close()
        

        

