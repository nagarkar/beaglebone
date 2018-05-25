cmake_minimum_required (VERSION 2.6)
set(PROJECT_NAME imu)

###### Project Options #########
# Do you want to run QSPY on this application?
option(QSPY_ENABLED "Should we pull QSPY sources?" OFF)

###### Project properties and lib dependencies ###### 
project (${PROJECT_NAME} C CXX)
set(SOURCES active_objects.cpp bsp.cpp main.cpp qp_plugs.cpp)

message("Finding required libs")
	find_library(ROBOTICSCAPE roboticscape required)
	find_library(RT rt required)
	find_library(M m required)
	message("Found roboticscape library here: ${ROBOTICSCAPE}")
	message("Found librt library here: ${RT}")
	message("Found libm library here: ${M}")

message("Set Compiler and linker flags...")
	set(CMAKE_CXX_FLAGS "-fno-rtti -fno-exceptions -ffunction-sections -fdata-sections \
        -Os -Wall -W -pthread -DNDEBUG \
        -DQP_API_VERSION=9999")

	set(CMAKE_C_FLAGS "-ffunction-sections -fdata-sections \
        -Os -Wall -W -pthread -DNDEBUG \
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

###### Binaries and linking ###### 
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PUBLIC roboticscape rt m)
