#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "telemetry_service.grpc.pb.h"
#include "CircularBuffer.h"
#include "Attitude.h"

using grpc::Server;
using grpc::ServerWriter;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using telemetry::AttitudeQRequest;
using telemetry::AttitudeQResponse;
using telemetry::TelemetryService;

class TelemetryServiceImpl final : public TelemetryService::Service {
private:
	CircularBuffer<Attitude> * const attitudeBuffer;
public:	
	TelemetryServiceImpl(CircularBuffer<Attitude> *buffer) : attitudeBuffer(buffer) {};
	Status GetAttitudeQ(ServerContext* context, const AttitudeQRequest* request, AttitudeQResponse* reply) override;
	Status GetAttitudeStream(ServerContext* context, const AttitudeQRequest* request, ServerWriter< ::telemetry::AttitudeQResponse>* writer) override;
	static std::unique_ptr<Server> RunServer(CircularBuffer<Attitude> * const buffer, bool wait);
};