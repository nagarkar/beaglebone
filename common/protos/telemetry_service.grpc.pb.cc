// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: telemetry_service.proto

#include "telemetry_service.pb.h"
#include "telemetry_service.grpc.pb.h"

#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/channel_interface.h>
#include <grpcpp/impl/codegen/client_unary_call.h>
#include <grpcpp/impl/codegen/method_handler_impl.h>
#include <grpcpp/impl/codegen/rpc_service_method.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/sync_stream.h>
namespace telemetry {

static const char* TelemetryService_method_names[] = {
  "/telemetry.TelemetryService/GetAttitudeQ",
  "/telemetry.TelemetryService/GetAttitudeStream",
  "/telemetry.TelemetryService/hitEvent",
  "/telemetry.TelemetryService/headingEvent",
  "/telemetry.TelemetryService/setTarget",
  "/telemetry.TelemetryService/setWaypoints",
};

std::unique_ptr< TelemetryService::Stub> TelemetryService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< TelemetryService::Stub> stub(new TelemetryService::Stub(channel));
  return stub;
}

TelemetryService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_GetAttitudeQ_(TelemetryService_method_names[0], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_GetAttitudeStream_(TelemetryService_method_names[1], ::grpc::internal::RpcMethod::SERVER_STREAMING, channel)
  , rpcmethod_hitEvent_(TelemetryService_method_names[2], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_headingEvent_(TelemetryService_method_names[3], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_setTarget_(TelemetryService_method_names[4], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_setWaypoints_(TelemetryService_method_names[5], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status TelemetryService::Stub::GetAttitudeQ(::grpc::ClientContext* context, const ::telemetry::AttitudeQRequest& request, ::telemetry::AttitudeQResponse* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_GetAttitudeQ_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::telemetry::AttitudeQResponse>* TelemetryService::Stub::AsyncGetAttitudeQRaw(::grpc::ClientContext* context, const ::telemetry::AttitudeQRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::telemetry::AttitudeQResponse>::Create(channel_.get(), cq, rpcmethod_GetAttitudeQ_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::telemetry::AttitudeQResponse>* TelemetryService::Stub::PrepareAsyncGetAttitudeQRaw(::grpc::ClientContext* context, const ::telemetry::AttitudeQRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::telemetry::AttitudeQResponse>::Create(channel_.get(), cq, rpcmethod_GetAttitudeQ_, context, request, false);
}

::grpc::ClientReader< ::telemetry::AttitudeQResponse>* TelemetryService::Stub::GetAttitudeStreamRaw(::grpc::ClientContext* context, const ::telemetry::AttitudeQRequest& request) {
  return ::grpc::internal::ClientReaderFactory< ::telemetry::AttitudeQResponse>::Create(channel_.get(), rpcmethod_GetAttitudeStream_, context, request);
}

::grpc::ClientAsyncReader< ::telemetry::AttitudeQResponse>* TelemetryService::Stub::AsyncGetAttitudeStreamRaw(::grpc::ClientContext* context, const ::telemetry::AttitudeQRequest& request, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc::internal::ClientAsyncReaderFactory< ::telemetry::AttitudeQResponse>::Create(channel_.get(), cq, rpcmethod_GetAttitudeStream_, context, request, true, tag);
}

::grpc::ClientAsyncReader< ::telemetry::AttitudeQResponse>* TelemetryService::Stub::PrepareAsyncGetAttitudeStreamRaw(::grpc::ClientContext* context, const ::telemetry::AttitudeQRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncReaderFactory< ::telemetry::AttitudeQResponse>::Create(channel_.get(), cq, rpcmethod_GetAttitudeStream_, context, request, false, nullptr);
}

::grpc::Status TelemetryService::Stub::hitEvent(::grpc::ClientContext* context, const ::telemetry::HitEvent& request, ::telemetry::EventResponse* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_hitEvent_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::telemetry::EventResponse>* TelemetryService::Stub::AsynchitEventRaw(::grpc::ClientContext* context, const ::telemetry::HitEvent& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::telemetry::EventResponse>::Create(channel_.get(), cq, rpcmethod_hitEvent_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::telemetry::EventResponse>* TelemetryService::Stub::PrepareAsynchitEventRaw(::grpc::ClientContext* context, const ::telemetry::HitEvent& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::telemetry::EventResponse>::Create(channel_.get(), cq, rpcmethod_hitEvent_, context, request, false);
}

::grpc::Status TelemetryService::Stub::headingEvent(::grpc::ClientContext* context, const ::telemetry::HeadingEvent& request, ::telemetry::EventResponse* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_headingEvent_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::telemetry::EventResponse>* TelemetryService::Stub::AsyncheadingEventRaw(::grpc::ClientContext* context, const ::telemetry::HeadingEvent& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::telemetry::EventResponse>::Create(channel_.get(), cq, rpcmethod_headingEvent_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::telemetry::EventResponse>* TelemetryService::Stub::PrepareAsyncheadingEventRaw(::grpc::ClientContext* context, const ::telemetry::HeadingEvent& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::telemetry::EventResponse>::Create(channel_.get(), cq, rpcmethod_headingEvent_, context, request, false);
}

::grpc::Status TelemetryService::Stub::setTarget(::grpc::ClientContext* context, const ::telemetry::Waypoint& request, ::telemetry::NoContent* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_setTarget_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::telemetry::NoContent>* TelemetryService::Stub::AsyncsetTargetRaw(::grpc::ClientContext* context, const ::telemetry::Waypoint& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::telemetry::NoContent>::Create(channel_.get(), cq, rpcmethod_setTarget_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::telemetry::NoContent>* TelemetryService::Stub::PrepareAsyncsetTargetRaw(::grpc::ClientContext* context, const ::telemetry::Waypoint& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::telemetry::NoContent>::Create(channel_.get(), cq, rpcmethod_setTarget_, context, request, false);
}

::grpc::Status TelemetryService::Stub::setWaypoints(::grpc::ClientContext* context, const ::telemetry::Waypoints& request, ::telemetry::NoContent* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_setWaypoints_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::telemetry::NoContent>* TelemetryService::Stub::AsyncsetWaypointsRaw(::grpc::ClientContext* context, const ::telemetry::Waypoints& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::telemetry::NoContent>::Create(channel_.get(), cq, rpcmethod_setWaypoints_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::telemetry::NoContent>* TelemetryService::Stub::PrepareAsyncsetWaypointsRaw(::grpc::ClientContext* context, const ::telemetry::Waypoints& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::telemetry::NoContent>::Create(channel_.get(), cq, rpcmethod_setWaypoints_, context, request, false);
}

TelemetryService::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      TelemetryService_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< TelemetryService::Service, ::telemetry::AttitudeQRequest, ::telemetry::AttitudeQResponse>(
          std::mem_fn(&TelemetryService::Service::GetAttitudeQ), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      TelemetryService_method_names[1],
      ::grpc::internal::RpcMethod::SERVER_STREAMING,
      new ::grpc::internal::ServerStreamingHandler< TelemetryService::Service, ::telemetry::AttitudeQRequest, ::telemetry::AttitudeQResponse>(
          std::mem_fn(&TelemetryService::Service::GetAttitudeStream), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      TelemetryService_method_names[2],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< TelemetryService::Service, ::telemetry::HitEvent, ::telemetry::EventResponse>(
          std::mem_fn(&TelemetryService::Service::hitEvent), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      TelemetryService_method_names[3],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< TelemetryService::Service, ::telemetry::HeadingEvent, ::telemetry::EventResponse>(
          std::mem_fn(&TelemetryService::Service::headingEvent), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      TelemetryService_method_names[4],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< TelemetryService::Service, ::telemetry::Waypoint, ::telemetry::NoContent>(
          std::mem_fn(&TelemetryService::Service::setTarget), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      TelemetryService_method_names[5],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< TelemetryService::Service, ::telemetry::Waypoints, ::telemetry::NoContent>(
          std::mem_fn(&TelemetryService::Service::setWaypoints), this)));
}

TelemetryService::Service::~Service() {
}

::grpc::Status TelemetryService::Service::GetAttitudeQ(::grpc::ServerContext* context, const ::telemetry::AttitudeQRequest* request, ::telemetry::AttitudeQResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status TelemetryService::Service::GetAttitudeStream(::grpc::ServerContext* context, const ::telemetry::AttitudeQRequest* request, ::grpc::ServerWriter< ::telemetry::AttitudeQResponse>* writer) {
  (void) context;
  (void) request;
  (void) writer;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status TelemetryService::Service::hitEvent(::grpc::ServerContext* context, const ::telemetry::HitEvent* request, ::telemetry::EventResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status TelemetryService::Service::headingEvent(::grpc::ServerContext* context, const ::telemetry::HeadingEvent* request, ::telemetry::EventResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status TelemetryService::Service::setTarget(::grpc::ServerContext* context, const ::telemetry::Waypoint* request, ::telemetry::NoContent* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status TelemetryService::Service::setWaypoints(::grpc::ServerContext* context, const ::telemetry::Waypoints* request, ::telemetry::NoContent* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace telemetry

