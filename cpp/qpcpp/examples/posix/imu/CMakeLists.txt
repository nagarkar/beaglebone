include(CheckIncludeFileCXX)

cmake_minimum_required (VERSION 2.6)
foreach(f CMAKE_INSTALL_PREFIX)
	message("${f}:${${f}}")
endforeach()
set(PROJECT_NAME imu)

function(fail_if_library_not_found _LIB)
	unset(LIB_VAR CACHE)
	find_library(LIB_VAR ${_LIB})
	if(NOT LIB_VAR)
		message(FATAL_ERROR "Required Library ${_LIB} not found")
	else()
		message("Found required Library ${_LIB}")
	endif()
endfunction()

###### Project Options #########
# Do you want to run QSPY on this application?
option(QSPY_ENABLED "Should we pull QSPY sources?" OFF)

###### Project properties and lib dependencies ###### 
project (${PROJECT_NAME} CXX)

set(SOURCES 
	active_objects.cpp 
	bsp.cpp	
	qp_plugs.cpp 
	CircularBuffer.cpp
	TelemetryServiceImpl.cpp)

set(DEPENDENCIES bbahrs roboticscape rt m ppcbproto dl grpc++_reflection grpc grpc++ protobuf)
link_directories	(/usr/lib ${CMAKE_INSTALL_PREFIX}/lib)
include_directories	(/usr/include ${CMAKE_INSTALL_PREFIX}/include)

message("Set Compiler and linker flags...")
	set(CMAKE_CXX_FLAGS " -std=c++11 -g -Wall -Werror -pthread -DNDEBUG \
        -DQP_API_VERSION=9999")

	set(CMAKE_C_FLAGS " -std=c++11 -g -Wall -Werror -pthread -DNDEBUG \
        -DQP_API_VERSION=9999")

	set(CMAKE_EXE_LINKER_FLAGS, "-Wl,-Map,${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.map,--cref,--gc-sections")


###### QP Configuration ###### 
# Fix QPCPP location
if(NOT EXISTS "$ENV{QPCPP}")
  # Change this to point to your local directory if required.
  message("Could not find QPCPP environment variable, using alternative...")
  set(QPCPP_SRC "${CMAKE_SOURCE_DIR}/../../../")
else()
  message("Found QPCPP environment variable $ENV{QPCPP}...")
  set(QPCPP_SRC $ENV{QPCPP})
endif()

message("Set QP source directories based on ${QPCPP_SRC}")
set(QP_PORT_SRC_DIR ${QPCPP_SRC}/ports/posix)
if(QSPY_ENABLED)
  set(QS_SRC_DIR ${QPCPP_SRC}/src/qs)
  add_definitions(-DQ_SPY)
endif()
set(QP_SRC_DIR ${QPCPP_SRC}/src/qf)

message("Set QP header includes")
include_directories(${QPCPP_SRC}/include)
include_directories(${QP_PORT_SRC_DIR})
include_directories(${QPCPP_SRC}/src)

if(QSPY_ENABLED)
  set(QS_SRCS
        ${QS_SRC_DIR}/qs.cpp
        ${QS_SRC_DIR}/qs_64bit.cpp
        ${QS_SRC_DIR}/qs_rx.cpp
        ${QS_SRC_DIR}/qs_fp.cpp)
endif()

set(QP_SRCS
        ${QP_SRC_DIR}/qep_hsm.cpp
        ${QP_SRC_DIR}/qep_msm.cpp
        ${QP_SRC_DIR}/qf_act.cpp
        ${QP_SRC_DIR}/qf_actq.cpp
        ${QP_SRC_DIR}/qf_defer.cpp
        ${QP_SRC_DIR}/qf_dyn.cpp
        ${QP_SRC_DIR}/qf_mem.cpp
        ${QP_SRC_DIR}/qf_ps.cpp
        ${QP_SRC_DIR}/qf_qact.cpp
        ${QP_SRC_DIR}/qf_qeq.cpp
        ${QP_SRC_DIR}/qf_qmact.cpp
        ${QP_SRC_DIR}/qf_time.cpp
        ${QP_PORT_SRC_DIR}/qf_port.cpp)

list(APPEND SOURCES
  ${QP_SRCS}
  ${QS_SRCS}
)

### Protobufs
#add_subdirectory("../../../../../common/protos" "protos/build")# "${CMAKE_CURRENT_BINARY_DIR}/common/protos")

###### Binaries and linking ###### 
message("Finding dependencies")
	foreach(f ${DEPENDENCIES})
		fail_if_library_not_found(${f})
	endforeach()
add_executable(${PROJECT_NAME} main.cpp ${SOURCES})
target_link_libraries(${PROJECT_NAME} PUBLIC ${DEPENDENCIES})

add_executable(TelemetryTestService TelemetryTestService.cpp ${SOURCES})
target_link_libraries(TelemetryTestService PUBLIC ${DEPENDENCIES})