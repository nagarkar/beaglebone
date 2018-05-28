#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include <telemetry_service.grpc.pb.h>
#include "TelemetryServiceImpl.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using telemetry::AttitudeQRequest;
using telemetry::AttitudeQResponse;
using telemetry::TelemetryService;

Status TelemetryServiceImpl::GetAttitudeQ(ServerContext* context, const AttitudeQRequest* request, AttitudeQResponse* reply) {
	//std::string prefix("Hello ");
	reply->set_qc(this->Qc);
	return Status::OK;
}
void TelemetryServiceImpl::RunServer() {
	std::string server_address("0.0.0.0:50051");
	TelemetryServiceImpl service;

	ServerBuilder builder;
	// Listen on the given address without any authentication mechanism.
	builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
	// Register "service" as the instance through which we'll communicate with
	// clients. In this case it corresponds to an *synchronous* service.
	builder.RegisterService(&service);
	// Finally assemble the server.
	std::unique_ptr<Server> server(builder.BuildAndStart());

	std::cout << "Server listening on " << server_address << std::endl;
	// Wait for the server to shutdown. Note that some other thread must be
	// responsible for shutting down the server for this call to ever return.
	server->Wait();
	//server->Shutdown();
}
