package pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces;

import static io.grpc.MethodDescriptor.generateFullMethodName;

/**
 */
@javax.annotation.Generated(
    value = "by gRPC proto compiler (version 1.48.0)",
    comments = "Source: ReplicatedTupleSpaces.proto")
@io.grpc.stub.annotations.GrpcGenerated
public final class ReplicatedTupleSpacesGrpc {

  private ReplicatedTupleSpacesGrpc() {}

  public static final String SERVICE_NAME = "pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpaces";

  // Static method descriptors that strictly reflect the proto.
  private static volatile io.grpc.MethodDescriptor<pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.PutRequest,
      pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.PutResponse> getPutMethod;

  @io.grpc.stub.annotations.RpcMethod(
      fullMethodName = SERVICE_NAME + '/' + "put",
      requestType = pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.PutRequest.class,
      responseType = pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.PutResponse.class,
      methodType = io.grpc.MethodDescriptor.MethodType.UNARY)
  public static io.grpc.MethodDescriptor<pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.PutRequest,
      pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.PutResponse> getPutMethod() {
    io.grpc.MethodDescriptor<pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.PutRequest, pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.PutResponse> getPutMethod;
    if ((getPutMethod = ReplicatedTupleSpacesGrpc.getPutMethod) == null) {
      synchronized (ReplicatedTupleSpacesGrpc.class) {
        if ((getPutMethod = ReplicatedTupleSpacesGrpc.getPutMethod) == null) {
          ReplicatedTupleSpacesGrpc.getPutMethod = getPutMethod =
              io.grpc.MethodDescriptor.<pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.PutRequest, pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.PutResponse>newBuilder()
              .setType(io.grpc.MethodDescriptor.MethodType.UNARY)
              .setFullMethodName(generateFullMethodName(SERVICE_NAME, "put"))
              .setSampledToLocalTracing(true)
              .setRequestMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.PutRequest.getDefaultInstance()))
              .setResponseMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.PutResponse.getDefaultInstance()))
              .setSchemaDescriptor(new ReplicatedTupleSpacesMethodDescriptorSupplier("put"))
              .build();
        }
      }
    }
    return getPutMethod;
  }

  private static volatile io.grpc.MethodDescriptor<pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReadRequest,
      pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReadResponse> getReadMethod;

  @io.grpc.stub.annotations.RpcMethod(
      fullMethodName = SERVICE_NAME + '/' + "read",
      requestType = pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReadRequest.class,
      responseType = pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReadResponse.class,
      methodType = io.grpc.MethodDescriptor.MethodType.UNARY)
  public static io.grpc.MethodDescriptor<pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReadRequest,
      pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReadResponse> getReadMethod() {
    io.grpc.MethodDescriptor<pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReadRequest, pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReadResponse> getReadMethod;
    if ((getReadMethod = ReplicatedTupleSpacesGrpc.getReadMethod) == null) {
      synchronized (ReplicatedTupleSpacesGrpc.class) {
        if ((getReadMethod = ReplicatedTupleSpacesGrpc.getReadMethod) == null) {
          ReplicatedTupleSpacesGrpc.getReadMethod = getReadMethod =
              io.grpc.MethodDescriptor.<pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReadRequest, pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReadResponse>newBuilder()
              .setType(io.grpc.MethodDescriptor.MethodType.UNARY)
              .setFullMethodName(generateFullMethodName(SERVICE_NAME, "read"))
              .setSampledToLocalTracing(true)
              .setRequestMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReadRequest.getDefaultInstance()))
              .setResponseMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReadResponse.getDefaultInstance()))
              .setSchemaDescriptor(new ReplicatedTupleSpacesMethodDescriptorSupplier("read"))
              .build();
        }
      }
    }
    return getReadMethod;
  }

  private static volatile io.grpc.MethodDescriptor<pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.TakeRequest,
      pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.TakeResponse> getTakeMethod;

  @io.grpc.stub.annotations.RpcMethod(
      fullMethodName = SERVICE_NAME + '/' + "take",
      requestType = pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.TakeRequest.class,
      responseType = pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.TakeResponse.class,
      methodType = io.grpc.MethodDescriptor.MethodType.UNARY)
  public static io.grpc.MethodDescriptor<pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.TakeRequest,
      pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.TakeResponse> getTakeMethod() {
    io.grpc.MethodDescriptor<pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.TakeRequest, pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.TakeResponse> getTakeMethod;
    if ((getTakeMethod = ReplicatedTupleSpacesGrpc.getTakeMethod) == null) {
      synchronized (ReplicatedTupleSpacesGrpc.class) {
        if ((getTakeMethod = ReplicatedTupleSpacesGrpc.getTakeMethod) == null) {
          ReplicatedTupleSpacesGrpc.getTakeMethod = getTakeMethod =
              io.grpc.MethodDescriptor.<pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.TakeRequest, pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.TakeResponse>newBuilder()
              .setType(io.grpc.MethodDescriptor.MethodType.UNARY)
              .setFullMethodName(generateFullMethodName(SERVICE_NAME, "take"))
              .setSampledToLocalTracing(true)
              .setRequestMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.TakeRequest.getDefaultInstance()))
              .setResponseMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.TakeResponse.getDefaultInstance()))
              .setSchemaDescriptor(new ReplicatedTupleSpacesMethodDescriptorSupplier("take"))
              .build();
        }
      }
    }
    return getTakeMethod;
  }

  private static volatile io.grpc.MethodDescriptor<pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReleaseRequest,
      pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReleaseResponse> getReleaseMethod;

  @io.grpc.stub.annotations.RpcMethod(
      fullMethodName = SERVICE_NAME + '/' + "release",
      requestType = pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReleaseRequest.class,
      responseType = pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReleaseResponse.class,
      methodType = io.grpc.MethodDescriptor.MethodType.UNARY)
  public static io.grpc.MethodDescriptor<pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReleaseRequest,
      pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReleaseResponse> getReleaseMethod() {
    io.grpc.MethodDescriptor<pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReleaseRequest, pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReleaseResponse> getReleaseMethod;
    if ((getReleaseMethod = ReplicatedTupleSpacesGrpc.getReleaseMethod) == null) {
      synchronized (ReplicatedTupleSpacesGrpc.class) {
        if ((getReleaseMethod = ReplicatedTupleSpacesGrpc.getReleaseMethod) == null) {
          ReplicatedTupleSpacesGrpc.getReleaseMethod = getReleaseMethod =
              io.grpc.MethodDescriptor.<pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReleaseRequest, pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReleaseResponse>newBuilder()
              .setType(io.grpc.MethodDescriptor.MethodType.UNARY)
              .setFullMethodName(generateFullMethodName(SERVICE_NAME, "release"))
              .setSampledToLocalTracing(true)
              .setRequestMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReleaseRequest.getDefaultInstance()))
              .setResponseMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReleaseResponse.getDefaultInstance()))
              .setSchemaDescriptor(new ReplicatedTupleSpacesMethodDescriptorSupplier("release"))
              .build();
        }
      }
    }
    return getReleaseMethod;
  }

  private static volatile io.grpc.MethodDescriptor<pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReqRequest,
      pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReqResponse> getRequestMethod;

  @io.grpc.stub.annotations.RpcMethod(
      fullMethodName = SERVICE_NAME + '/' + "request",
      requestType = pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReqRequest.class,
      responseType = pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReqResponse.class,
      methodType = io.grpc.MethodDescriptor.MethodType.UNARY)
  public static io.grpc.MethodDescriptor<pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReqRequest,
      pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReqResponse> getRequestMethod() {
    io.grpc.MethodDescriptor<pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReqRequest, pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReqResponse> getRequestMethod;
    if ((getRequestMethod = ReplicatedTupleSpacesGrpc.getRequestMethod) == null) {
      synchronized (ReplicatedTupleSpacesGrpc.class) {
        if ((getRequestMethod = ReplicatedTupleSpacesGrpc.getRequestMethod) == null) {
          ReplicatedTupleSpacesGrpc.getRequestMethod = getRequestMethod =
              io.grpc.MethodDescriptor.<pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReqRequest, pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReqResponse>newBuilder()
              .setType(io.grpc.MethodDescriptor.MethodType.UNARY)
              .setFullMethodName(generateFullMethodName(SERVICE_NAME, "request"))
              .setSampledToLocalTracing(true)
              .setRequestMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReqRequest.getDefaultInstance()))
              .setResponseMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReqResponse.getDefaultInstance()))
              .setSchemaDescriptor(new ReplicatedTupleSpacesMethodDescriptorSupplier("request"))
              .build();
        }
      }
    }
    return getRequestMethod;
  }

  private static volatile io.grpc.MethodDescriptor<pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.GetTupleSpacesStateRequest,
      pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.GetTupleSpacesStateResponse> getGetTupleSpacesStateMethod;

  @io.grpc.stub.annotations.RpcMethod(
      fullMethodName = SERVICE_NAME + '/' + "getTupleSpacesState",
      requestType = pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.GetTupleSpacesStateRequest.class,
      responseType = pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.GetTupleSpacesStateResponse.class,
      methodType = io.grpc.MethodDescriptor.MethodType.UNARY)
  public static io.grpc.MethodDescriptor<pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.GetTupleSpacesStateRequest,
      pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.GetTupleSpacesStateResponse> getGetTupleSpacesStateMethod() {
    io.grpc.MethodDescriptor<pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.GetTupleSpacesStateRequest, pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.GetTupleSpacesStateResponse> getGetTupleSpacesStateMethod;
    if ((getGetTupleSpacesStateMethod = ReplicatedTupleSpacesGrpc.getGetTupleSpacesStateMethod) == null) {
      synchronized (ReplicatedTupleSpacesGrpc.class) {
        if ((getGetTupleSpacesStateMethod = ReplicatedTupleSpacesGrpc.getGetTupleSpacesStateMethod) == null) {
          ReplicatedTupleSpacesGrpc.getGetTupleSpacesStateMethod = getGetTupleSpacesStateMethod =
              io.grpc.MethodDescriptor.<pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.GetTupleSpacesStateRequest, pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.GetTupleSpacesStateResponse>newBuilder()
              .setType(io.grpc.MethodDescriptor.MethodType.UNARY)
              .setFullMethodName(generateFullMethodName(SERVICE_NAME, "getTupleSpacesState"))
              .setSampledToLocalTracing(true)
              .setRequestMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.GetTupleSpacesStateRequest.getDefaultInstance()))
              .setResponseMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.GetTupleSpacesStateResponse.getDefaultInstance()))
              .setSchemaDescriptor(new ReplicatedTupleSpacesMethodDescriptorSupplier("getTupleSpacesState"))
              .build();
        }
      }
    }
    return getGetTupleSpacesStateMethod;
  }

  /**
   * Creates a new async stub that supports all call types for the service
   */
  public static ReplicatedTupleSpacesStub newStub(io.grpc.Channel channel) {
    io.grpc.stub.AbstractStub.StubFactory<ReplicatedTupleSpacesStub> factory =
      new io.grpc.stub.AbstractStub.StubFactory<ReplicatedTupleSpacesStub>() {
        @java.lang.Override
        public ReplicatedTupleSpacesStub newStub(io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
          return new ReplicatedTupleSpacesStub(channel, callOptions);
        }
      };
    return ReplicatedTupleSpacesStub.newStub(factory, channel);
  }

  /**
   * Creates a new blocking-style stub that supports unary and streaming output calls on the service
   */
  public static ReplicatedTupleSpacesBlockingStub newBlockingStub(
      io.grpc.Channel channel) {
    io.grpc.stub.AbstractStub.StubFactory<ReplicatedTupleSpacesBlockingStub> factory =
      new io.grpc.stub.AbstractStub.StubFactory<ReplicatedTupleSpacesBlockingStub>() {
        @java.lang.Override
        public ReplicatedTupleSpacesBlockingStub newStub(io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
          return new ReplicatedTupleSpacesBlockingStub(channel, callOptions);
        }
      };
    return ReplicatedTupleSpacesBlockingStub.newStub(factory, channel);
  }

  /**
   * Creates a new ListenableFuture-style stub that supports unary calls on the service
   */
  public static ReplicatedTupleSpacesFutureStub newFutureStub(
      io.grpc.Channel channel) {
    io.grpc.stub.AbstractStub.StubFactory<ReplicatedTupleSpacesFutureStub> factory =
      new io.grpc.stub.AbstractStub.StubFactory<ReplicatedTupleSpacesFutureStub>() {
        @java.lang.Override
        public ReplicatedTupleSpacesFutureStub newStub(io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
          return new ReplicatedTupleSpacesFutureStub(channel, callOptions);
        }
      };
    return ReplicatedTupleSpacesFutureStub.newStub(factory, channel);
  }

  /**
   */
  public static abstract class ReplicatedTupleSpacesImplBase implements io.grpc.BindableService {

    /**
     */
    public void put(pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.PutRequest request,
        io.grpc.stub.StreamObserver<pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.PutResponse> responseObserver) {
      io.grpc.stub.ServerCalls.asyncUnimplementedUnaryCall(getPutMethod(), responseObserver);
    }

    /**
     */
    public void read(pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReadRequest request,
        io.grpc.stub.StreamObserver<pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReadResponse> responseObserver) {
      io.grpc.stub.ServerCalls.asyncUnimplementedUnaryCall(getReadMethod(), responseObserver);
    }

    /**
     */
    public void take(pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.TakeRequest request,
        io.grpc.stub.StreamObserver<pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.TakeResponse> responseObserver) {
      io.grpc.stub.ServerCalls.asyncUnimplementedUnaryCall(getTakeMethod(), responseObserver);
    }

    /**
     */
    public void release(pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReleaseRequest request,
        io.grpc.stub.StreamObserver<pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReleaseResponse> responseObserver) {
      io.grpc.stub.ServerCalls.asyncUnimplementedUnaryCall(getReleaseMethod(), responseObserver);
    }

    /**
     */
    public void request(pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReqRequest request,
        io.grpc.stub.StreamObserver<pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReqResponse> responseObserver) {
      io.grpc.stub.ServerCalls.asyncUnimplementedUnaryCall(getRequestMethod(), responseObserver);
    }

    /**
     */
    public void getTupleSpacesState(pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.GetTupleSpacesStateRequest request,
        io.grpc.stub.StreamObserver<pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.GetTupleSpacesStateResponse> responseObserver) {
      io.grpc.stub.ServerCalls.asyncUnimplementedUnaryCall(getGetTupleSpacesStateMethod(), responseObserver);
    }

    @java.lang.Override public final io.grpc.ServerServiceDefinition bindService() {
      return io.grpc.ServerServiceDefinition.builder(getServiceDescriptor())
          .addMethod(
            getPutMethod(),
            io.grpc.stub.ServerCalls.asyncUnaryCall(
              new MethodHandlers<
                pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.PutRequest,
                pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.PutResponse>(
                  this, METHODID_PUT)))
          .addMethod(
            getReadMethod(),
            io.grpc.stub.ServerCalls.asyncUnaryCall(
              new MethodHandlers<
                pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReadRequest,
                pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReadResponse>(
                  this, METHODID_READ)))
          .addMethod(
            getTakeMethod(),
            io.grpc.stub.ServerCalls.asyncUnaryCall(
              new MethodHandlers<
                pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.TakeRequest,
                pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.TakeResponse>(
                  this, METHODID_TAKE)))
          .addMethod(
            getReleaseMethod(),
            io.grpc.stub.ServerCalls.asyncUnaryCall(
              new MethodHandlers<
                pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReleaseRequest,
                pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReleaseResponse>(
                  this, METHODID_RELEASE)))
          .addMethod(
            getRequestMethod(),
            io.grpc.stub.ServerCalls.asyncUnaryCall(
              new MethodHandlers<
                pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReqRequest,
                pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReqResponse>(
                  this, METHODID_REQUEST)))
          .addMethod(
            getGetTupleSpacesStateMethod(),
            io.grpc.stub.ServerCalls.asyncUnaryCall(
              new MethodHandlers<
                pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.GetTupleSpacesStateRequest,
                pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.GetTupleSpacesStateResponse>(
                  this, METHODID_GET_TUPLE_SPACES_STATE)))
          .build();
    }
  }

  /**
   */
  public static final class ReplicatedTupleSpacesStub extends io.grpc.stub.AbstractAsyncStub<ReplicatedTupleSpacesStub> {
    private ReplicatedTupleSpacesStub(
        io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
      super(channel, callOptions);
    }

    @java.lang.Override
    protected ReplicatedTupleSpacesStub build(
        io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
      return new ReplicatedTupleSpacesStub(channel, callOptions);
    }

    /**
     */
    public void put(pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.PutRequest request,
        io.grpc.stub.StreamObserver<pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.PutResponse> responseObserver) {
      io.grpc.stub.ClientCalls.asyncUnaryCall(
          getChannel().newCall(getPutMethod(), getCallOptions()), request, responseObserver);
    }

    /**
     */
    public void read(pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReadRequest request,
        io.grpc.stub.StreamObserver<pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReadResponse> responseObserver) {
      io.grpc.stub.ClientCalls.asyncUnaryCall(
          getChannel().newCall(getReadMethod(), getCallOptions()), request, responseObserver);
    }

    /**
     */
    public void take(pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.TakeRequest request,
        io.grpc.stub.StreamObserver<pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.TakeResponse> responseObserver) {
      io.grpc.stub.ClientCalls.asyncUnaryCall(
          getChannel().newCall(getTakeMethod(), getCallOptions()), request, responseObserver);
    }

    /**
     */
    public void release(pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReleaseRequest request,
        io.grpc.stub.StreamObserver<pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReleaseResponse> responseObserver) {
      io.grpc.stub.ClientCalls.asyncUnaryCall(
          getChannel().newCall(getReleaseMethod(), getCallOptions()), request, responseObserver);
    }

    /**
     */
    public void request(pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReqRequest request,
        io.grpc.stub.StreamObserver<pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReqResponse> responseObserver) {
      io.grpc.stub.ClientCalls.asyncUnaryCall(
          getChannel().newCall(getRequestMethod(), getCallOptions()), request, responseObserver);
    }

    /**
     */
    public void getTupleSpacesState(pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.GetTupleSpacesStateRequest request,
        io.grpc.stub.StreamObserver<pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.GetTupleSpacesStateResponse> responseObserver) {
      io.grpc.stub.ClientCalls.asyncUnaryCall(
          getChannel().newCall(getGetTupleSpacesStateMethod(), getCallOptions()), request, responseObserver);
    }
  }

  /**
   */
  public static final class ReplicatedTupleSpacesBlockingStub extends io.grpc.stub.AbstractBlockingStub<ReplicatedTupleSpacesBlockingStub> {
    private ReplicatedTupleSpacesBlockingStub(
        io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
      super(channel, callOptions);
    }

    @java.lang.Override
    protected ReplicatedTupleSpacesBlockingStub build(
        io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
      return new ReplicatedTupleSpacesBlockingStub(channel, callOptions);
    }

    /**
     */
    public pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.PutResponse put(pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.PutRequest request) {
      return io.grpc.stub.ClientCalls.blockingUnaryCall(
          getChannel(), getPutMethod(), getCallOptions(), request);
    }

    /**
     */
    public pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReadResponse read(pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReadRequest request) {
      return io.grpc.stub.ClientCalls.blockingUnaryCall(
          getChannel(), getReadMethod(), getCallOptions(), request);
    }

    /**
     */
    public pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.TakeResponse take(pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.TakeRequest request) {
      return io.grpc.stub.ClientCalls.blockingUnaryCall(
          getChannel(), getTakeMethod(), getCallOptions(), request);
    }

    /**
     */
    public pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReleaseResponse release(pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReleaseRequest request) {
      return io.grpc.stub.ClientCalls.blockingUnaryCall(
          getChannel(), getReleaseMethod(), getCallOptions(), request);
    }

    /**
     */
    public pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReqResponse request(pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReqRequest request) {
      return io.grpc.stub.ClientCalls.blockingUnaryCall(
          getChannel(), getRequestMethod(), getCallOptions(), request);
    }

    /**
     */
    public pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.GetTupleSpacesStateResponse getTupleSpacesState(pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.GetTupleSpacesStateRequest request) {
      return io.grpc.stub.ClientCalls.blockingUnaryCall(
          getChannel(), getGetTupleSpacesStateMethod(), getCallOptions(), request);
    }
  }

  /**
   */
  public static final class ReplicatedTupleSpacesFutureStub extends io.grpc.stub.AbstractFutureStub<ReplicatedTupleSpacesFutureStub> {
    private ReplicatedTupleSpacesFutureStub(
        io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
      super(channel, callOptions);
    }

    @java.lang.Override
    protected ReplicatedTupleSpacesFutureStub build(
        io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
      return new ReplicatedTupleSpacesFutureStub(channel, callOptions);
    }

    /**
     */
    public com.google.common.util.concurrent.ListenableFuture<pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.PutResponse> put(
        pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.PutRequest request) {
      return io.grpc.stub.ClientCalls.futureUnaryCall(
          getChannel().newCall(getPutMethod(), getCallOptions()), request);
    }

    /**
     */
    public com.google.common.util.concurrent.ListenableFuture<pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReadResponse> read(
        pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReadRequest request) {
      return io.grpc.stub.ClientCalls.futureUnaryCall(
          getChannel().newCall(getReadMethod(), getCallOptions()), request);
    }

    /**
     */
    public com.google.common.util.concurrent.ListenableFuture<pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.TakeResponse> take(
        pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.TakeRequest request) {
      return io.grpc.stub.ClientCalls.futureUnaryCall(
          getChannel().newCall(getTakeMethod(), getCallOptions()), request);
    }

    /**
     */
    public com.google.common.util.concurrent.ListenableFuture<pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReleaseResponse> release(
        pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReleaseRequest request) {
      return io.grpc.stub.ClientCalls.futureUnaryCall(
          getChannel().newCall(getReleaseMethod(), getCallOptions()), request);
    }

    /**
     */
    public com.google.common.util.concurrent.ListenableFuture<pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReqResponse> request(
        pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReqRequest request) {
      return io.grpc.stub.ClientCalls.futureUnaryCall(
          getChannel().newCall(getRequestMethod(), getCallOptions()), request);
    }

    /**
     */
    public com.google.common.util.concurrent.ListenableFuture<pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.GetTupleSpacesStateResponse> getTupleSpacesState(
        pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.GetTupleSpacesStateRequest request) {
      return io.grpc.stub.ClientCalls.futureUnaryCall(
          getChannel().newCall(getGetTupleSpacesStateMethod(), getCallOptions()), request);
    }
  }

  private static final int METHODID_PUT = 0;
  private static final int METHODID_READ = 1;
  private static final int METHODID_TAKE = 2;
  private static final int METHODID_RELEASE = 3;
  private static final int METHODID_REQUEST = 4;
  private static final int METHODID_GET_TUPLE_SPACES_STATE = 5;

  private static final class MethodHandlers<Req, Resp> implements
      io.grpc.stub.ServerCalls.UnaryMethod<Req, Resp>,
      io.grpc.stub.ServerCalls.ServerStreamingMethod<Req, Resp>,
      io.grpc.stub.ServerCalls.ClientStreamingMethod<Req, Resp>,
      io.grpc.stub.ServerCalls.BidiStreamingMethod<Req, Resp> {
    private final ReplicatedTupleSpacesImplBase serviceImpl;
    private final int methodId;

    MethodHandlers(ReplicatedTupleSpacesImplBase serviceImpl, int methodId) {
      this.serviceImpl = serviceImpl;
      this.methodId = methodId;
    }

    @java.lang.Override
    @java.lang.SuppressWarnings("unchecked")
    public void invoke(Req request, io.grpc.stub.StreamObserver<Resp> responseObserver) {
      switch (methodId) {
        case METHODID_PUT:
          serviceImpl.put((pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.PutRequest) request,
              (io.grpc.stub.StreamObserver<pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.PutResponse>) responseObserver);
          break;
        case METHODID_READ:
          serviceImpl.read((pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReadRequest) request,
              (io.grpc.stub.StreamObserver<pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReadResponse>) responseObserver);
          break;
        case METHODID_TAKE:
          serviceImpl.take((pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.TakeRequest) request,
              (io.grpc.stub.StreamObserver<pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.TakeResponse>) responseObserver);
          break;
        case METHODID_RELEASE:
          serviceImpl.release((pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReleaseRequest) request,
              (io.grpc.stub.StreamObserver<pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReleaseResponse>) responseObserver);
          break;
        case METHODID_REQUEST:
          serviceImpl.request((pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReqRequest) request,
              (io.grpc.stub.StreamObserver<pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.ReqResponse>) responseObserver);
          break;
        case METHODID_GET_TUPLE_SPACES_STATE:
          serviceImpl.getTupleSpacesState((pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.GetTupleSpacesStateRequest) request,
              (io.grpc.stub.StreamObserver<pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.GetTupleSpacesStateResponse>) responseObserver);
          break;
        default:
          throw new AssertionError();
      }
    }

    @java.lang.Override
    @java.lang.SuppressWarnings("unchecked")
    public io.grpc.stub.StreamObserver<Req> invoke(
        io.grpc.stub.StreamObserver<Resp> responseObserver) {
      switch (methodId) {
        default:
          throw new AssertionError();
      }
    }
  }

  private static abstract class ReplicatedTupleSpacesBaseDescriptorSupplier
      implements io.grpc.protobuf.ProtoFileDescriptorSupplier, io.grpc.protobuf.ProtoServiceDescriptorSupplier {
    ReplicatedTupleSpacesBaseDescriptorSupplier() {}

    @java.lang.Override
    public com.google.protobuf.Descriptors.FileDescriptor getFileDescriptor() {
      return pt.ulisboa.tecnico.tuplespaces.contract.ReplicatedTupleSpaces.ReplicatedTupleSpacesOuterClass.getDescriptor();
    }

    @java.lang.Override
    public com.google.protobuf.Descriptors.ServiceDescriptor getServiceDescriptor() {
      return getFileDescriptor().findServiceByName("ReplicatedTupleSpaces");
    }
  }

  private static final class ReplicatedTupleSpacesFileDescriptorSupplier
      extends ReplicatedTupleSpacesBaseDescriptorSupplier {
    ReplicatedTupleSpacesFileDescriptorSupplier() {}
  }

  private static final class ReplicatedTupleSpacesMethodDescriptorSupplier
      extends ReplicatedTupleSpacesBaseDescriptorSupplier
      implements io.grpc.protobuf.ProtoMethodDescriptorSupplier {
    private final String methodName;

    ReplicatedTupleSpacesMethodDescriptorSupplier(String methodName) {
      this.methodName = methodName;
    }

    @java.lang.Override
    public com.google.protobuf.Descriptors.MethodDescriptor getMethodDescriptor() {
      return getServiceDescriptor().findMethodByName(methodName);
    }
  }

  private static volatile io.grpc.ServiceDescriptor serviceDescriptor;

  public static io.grpc.ServiceDescriptor getServiceDescriptor() {
    io.grpc.ServiceDescriptor result = serviceDescriptor;
    if (result == null) {
      synchronized (ReplicatedTupleSpacesGrpc.class) {
        result = serviceDescriptor;
        if (result == null) {
          serviceDescriptor = result = io.grpc.ServiceDescriptor.newBuilder(SERVICE_NAME)
              .setSchemaDescriptor(new ReplicatedTupleSpacesFileDescriptorSupplier())
              .addMethod(getPutMethod())
              .addMethod(getReadMethod())
              .addMethod(getTakeMethod())
              .addMethod(getReleaseMethod())
              .addMethod(getRequestMethod())
              .addMethod(getGetTupleSpacesStateMethod())
              .build();
        }
      }
    }
    return result;
  }
}
