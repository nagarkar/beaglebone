#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "telemetry_service.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using telemetry::AttitudeQRequest;
using telemetry::AttitudeQResponse;
using telemetry::TelemetryService;

class TelemetryServiceImpl final : public TelemetryService::Service {
private:
	float Qc;
public:	
	TelemetryServiceImpl() : Qc(0.0) {};
	Status GetAttitudeQ(ServerContext* context, const AttitudeQRequest* request, AttitudeQResponse* reply) override;
	static void RunServer();
};