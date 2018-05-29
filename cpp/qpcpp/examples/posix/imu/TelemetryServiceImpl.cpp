#include <iostream>
#include <memory>
#include <string>
#include <unistd.h>
#include <grpcpp/grpcpp.h>
#include <telemetry_service.grpc.pb.h>
#include "TelemetryServiceImpl.h"
#include "bsp.h"

using grpc::Server;
using grpc::ServerWriter;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using telemetry::AttitudeQRequest;
using telemetry::AttitudeQResponse;
using telemetry::TelemetryService;
using namespace std;


Status TelemetryServiceImpl::GetAttitudeQ(ServerContext* context, const AttitudeQRequest* request, AttitudeQResponse* reply) {
	//std::string prefix("Hello ");
	//cout << "GetAttitudeQ called" << endl;
	if (this->attitudeBuffer->empty()) {
		//cout << "attitude buffer empty" << endl;
		return Status::CANCELLED;
	}
	Attitude attitude = this->attitudeBuffer->get();
	reply->set_qc(attitude.Qc);
	reply->set_qx(attitude.Qx);
	reply->set_qy(attitude.Qy);
	reply->set_qz(attitude.Qz);
	reply->set_timestamp(attitude.timestamp);
	//reply->set_timestamp(tspec.tv_sec * 1000 * 1000 * 1000 + tspec.tv_nsec);
	return Status::OK;
}

Status TelemetryServiceImpl::GetAttitudeStream(ServerContext * context, const AttitudeQRequest * request, ServerWriter<::telemetry::AttitudeQResponse>* writer)
{
	AttitudeQResponse response;
	while (!this->attitudeBuffer->empty()) {
		Attitude attitude = this->attitudeBuffer->get();
		response.set_qc(attitude.Qc);
		response.set_qx(attitude.Qx);
		response.set_qy(attitude.Qy);
		response.set_qz(attitude.Qz);			
		//response.set_timestamp(tspec.tv_sec * 1000 * 1000 * 1000 + tspec.tv_nsec);
		response.set_timestamp(attitude.timestamp);
		if (!writer->Write(response)) {
			cout << "GRPC Stream closed...." << endl;
			return Status::CANCELLED;
		}
	}
	return Status::OK;
}

std::unique_ptr<Server> TelemetryServiceImpl::RunServer(CircularBuffer<Attitude> * const buffer, bool wait) {
	std::string server_address("0.0.0.0:50051");
	TelemetryServiceImpl service(buffer);

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
	if (wait) {
		server->Wait();
	}
	return server;
	//server->Shutdown();
}