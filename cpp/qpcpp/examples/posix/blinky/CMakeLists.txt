cmake_minimum_required (VERSION 2.6)

option(QSPY_ENABLED "Should we pull QSPY sources?" OFF)

message("Starting CMake...")

project (blinky C CXX)

message("Find required packages...")
# None

message("Find required libs")
find_library(ROBOTICSCAPE roboticscape required)
message("Found roboticscape library here: ${ROBOTICSCAPE}")


set(QPCPP "${CMAKE_SOURCE_DIR}/../../../")
message("Set QP locations starting from ${QPCPP}")
set(QP_PORT_SRC_DIR ${QPCPP}/ports/posix)

# Robotics Cape directories
#set(RC_LIB_SRC_DIR "~/workspace/beaglebone/Robotics_Cape_Installer/library/")


if(QSPY_ENABLED)
  set(QS_SRC_DIR ${QPCPP}/src/qs)
  add_definitions(-DQ_SPY)
endif()

set(QP_SRC_DIR ${QPCPP}/src/qf)

message("Set Compiler and linker flags...")
set(CMAKE_CXX_FLAGS "-fno-rtti -fno-exceptions -ffunction-sections -fdata-sections \
        -Os -Wall -W -pthread -DNDEBUG \
        -DQP_API_VERSION=9999")

set(CMAKE_C_FLAGS "-ffunction-sections -fdata-sections \
        -Os -Wall -W -pthread -DNDEBUG \
        -DQP_API_VERSION=9999")

set(CMAKE_EXE_LINKER_FLAGS, "-Wl,-Map,${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.map,--cref,--gc-sections")

message("Include directories...")
include_directories(${QPCPP}/include)
#include_directories(${RC_LIB_SRC_DIR}/include)
include_directories(${QP_PORT_SRC_DIR})
include_directories(.)
include_directories(${QPCPP}/src)

message("Identify all sources...")

set (SOURCES blinky.cpp bsp.cpp main.cpp)

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
#add_executable(blinky ${SOURCES} ${QP_SRCS} ${QS_SRCS})
add_executable(blinky ${SOURCES})
target_link_libraries(blinky PUBLIC roboticscape rt m)
