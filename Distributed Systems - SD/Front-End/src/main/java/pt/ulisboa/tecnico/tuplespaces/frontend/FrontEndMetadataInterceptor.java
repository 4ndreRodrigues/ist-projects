package pt.ulisboa.tecnico.tuplespaces.frontend;

import io.grpc.*;

public class FrontEndMetadataInterceptor implements ServerInterceptor {

    // Key to store metadata in gRPC Context
    public static final Context.Key<String> DELAY_CTX_KEY = Context.key("delays");

    @Override
    public <ReqT, RespT> ServerCall.Listener<ReqT> interceptCall(
            ServerCall<ReqT, RespT> call,
            Metadata headers,
            ServerCallHandler<ReqT, RespT> next) {

        // Extract the delay from the metadata
        String delays = headers.get(Metadata.Key.of("delay", Metadata.ASCII_STRING_MARSHALLER));

        if (delays != null) {
            System.out.println("[Frontend Interceptor] Received delays metadata: " + delays);
        } else {
            delays = "0,0,0"; // default to 0 if not sent
        }

        // Put the delay in the Context for later use in service methods
        Context contextWithDelay = Context.current().withValue(DELAY_CTX_KEY, delays);

        // Proceed with the call using the updated context
        return Contexts.interceptCall(contextWithDelay, call, headers, next);
    }
}