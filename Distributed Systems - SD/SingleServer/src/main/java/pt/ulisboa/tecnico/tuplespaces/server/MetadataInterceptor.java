package pt.ulisboa.tecnico.tuplespaces.server;


import io.grpc.*;

public class MetadataInterceptor implements ServerInterceptor {

    @Override
    public <ReqT, RespT> ServerCall.Listener<ReqT> interceptCall(
            ServerCall<ReqT, RespT> call,
            Metadata headers,
            ServerCallHandler<ReqT, RespT> next) {

        // Put the metadata into the gRPC context
        Context ctx = Context.current().withValue(TupleSpacesImpl.METADATA_KEY, headers);
        return Contexts.interceptCall(ctx, call, headers, next);
    }
}