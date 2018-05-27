cmake_minimum_required(VERSION 3.5)

function(fail_if_library_not_found _LIB)
	unset(LIB_VAR CACHE)
	find_library(LIB_VAR ${_LIB})
	if(NOT LIB_VAR)
		message(FATAL_ERROR "Required Library ${_LIB} not found")
	else()
		message("Found ${_LIB}")
	endif()
endfunction()

set(PROJECT_NAME ppcbproto)
project(${PROJECT_NAME} C CXX)

SET(DEPENDENCIES dl grpc++_reflection grpc grpc++ protobuf)

SET (PROTO_SRC	telemetry_service.proto)
SET(SOURCES
	${CMAKE_BINARY_DIR}/telemetry_service.pb.cc
	${CMAKE_BINARY_DIR}/telemetry_service.grpc.pb.cc
	)
SET(PUBLIC_HEADER_FILES
	${CMAKE_BINARY_DIR}/telemetry_service.pb.h
	${CMAKE_BINARY_DIR}/telemetry_service.grpc.pb.h
	)
message("SOURCES: ${SOURCES}")
message("PUBLIC_HEADER_FILES: ${PUBLIC_HEADER_FILES}")

link_directories	(/usr/lib ${CMAKE_INSTALL_PREFIX}/lib)
include_directories	(/usr/include ${CMAKE_INSTALL_PREFIX}/include)

message("Finding dependencies")
	foreach(ARG ${DEPENDENCIES})
		fail_if_library_not_found(${ARG})
	endforeach()


message("Set Compiler and linker flags...")
	set(CMAKE_CXX_FLAGS " -std=c++11 -g -Wall -Werror ")

	set(CMAKE_C_FLAGS "-pthread")

	#set(CMAKE_EXE_LINKER_FLAGS, "")

function(PROTOBUF_GENERATE_GRPC	_PROTOBUF_FILES)
        # grpc cpp plubin will be /usr/local/bin/grpc_cpp_plugin
	execute_process(COMMAND which grpc_cpp_plugin 
		OUTPUT_VARIABLE GRPC_CPP_PLUGIN_LOC)
	string(REGEX REPLACE "\n$" "" GRPC_CPP_PLUGIN_LOC "${GRPC_CPP_PLUGIN_LOC}")
	execute_process(COMMAND protoc -I ${CMAKE_SOURCE_DIR} --grpc_out=${CMAKE_BINARY_DIR} --plugin=protoc-gen-grpc=${GRPC_CPP_PLUGIN_LOC} ${_PROTOBUF_FILES} )
endfunction()

INCLUDE(FindProtobuf)
FIND_PACKAGE(Protobuf REQUIRED)

PROTOBUF_GENERATE_GRPC(${PROTO_SRC})
PROTOBUF_GENERATE_CPP(_DC1 _DC2 ${PROTO_SRC})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

#INCLUDE_DIRECTORIES(${PROTOBUF_INCLUDE_DIR})
#ADD_LIBRARY(${PROJECT_NAME} ${PROTO_HEADER} ${PROTO_SRC})
ADD_LIBRARY(${PROJECT_NAME} SHARED ${PUBLIC_HEADER_FILES} ${SOURCES})
#foreach(f ${DEPENDENCIES})	
target_link_libraries(${PROJECT_NAME} PUBLIC ${DEPENDENCIES})
#endforeach()
INSTALL(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)
INSTALL(FILES ${PUBLIC_HEADER_FILES} DESTINATION include)