#!/bin/bash
python -m grpc_tools.protoc  -I ../../../../../../common/protos --python_out=. --grpc_python_out=.   ../../../../../../common/protos/telemetry_service.proto
