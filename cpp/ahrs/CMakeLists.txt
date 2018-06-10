cmake_minimum_required (VERSION 2.6)
set(PROJECT_NAME bbahrs)

###### Project properties and lib dependencies ###### 
project (${PROJECT_NAME} C CXX)

set(SOURCES 
	MadgwickAHRS.c 
	MahonyAHRS.c 
	madgwick_mahony_common.c)

set(PUBLIC_HEADER_FILES 
	MadgwickAHRS.h 
	MahonyAHRS.h 
	madgwick_mahony_common.h)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

message("Set Compiler and linker flags...")
	set(CMAKE_CXX_FLAGS "-fno-rtti -fno-exceptions -ffunction-sections -fdata-sections \
        -Os -Wall -W -pthread -DNDEBUG \
        -DQP_API_VERSION=9999")

	set(CMAKE_C_FLAGS "-ffunction-sections -fdata-sections \
        -Os -Wall -W -pthread -DNDEBUG \
        -DQP_API_VERSION=9999")

	set(CMAKE_EXE_LINKER_FLAGS, "-Wl,-Map,${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.map,--cref,--gc-sections")

install(TARGETS ${PROJECT_NAME}  LIBRARY DESTINATION lib)
install(FILES ${PUBLIC_HEADER_FILES} DESTINATION include/bbahrs)

set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "2")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_DESCRIPTION "Configure UExhibitor and launch missions")
set(CPACK_PACKAGE_CONTACT "Adrien BARRAL aba@robopec.com")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)
