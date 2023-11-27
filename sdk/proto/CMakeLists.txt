# Copyright (c) 2022-2023 Contributors to the Eclipse Foundation
#
# This program and the accompanying materials are made available under the
# terms of the Apache License, Version 2.0 which is available at
# https://www.apache.org/licenses/LICENSE-2.0.
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
# License for the specific language governing permissions and limitations
# under the License.
#
# SPDX-License-Identifier: Apache-2.0

file(GLOB_RECURSE PROTO_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.proto)

find_program(_gRPC_PROTOBUF_PROTOC_EXECUTABLE protoc REQUIRED 
    PATHS ${BUILD_TOOLS_PATH}
    NO_DEFAULT_PATH)
find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin REQUIRED 
    PATHS ${BUILD_TOOLS_PATH}
    NO_DEFAULT_PATH)

set(_gRPC_PROTO_GENS_DIR ${CMAKE_BINARY_DIR}/gens)
set(_gRPC_PROTO_SRCS_DIR ${CMAKE_CURRENT_SOURCE_DIR})
file(MAKE_DIRECTORY ${_gRPC_PROTO_GENS_DIR})

set(PROTO_SRCS "" CACHE INTERNAL "")
set(PROTO_HDRS "" CACHE INTERNAL "")

foreach(FILE ${PROTO_FILES})
    get_filename_component(ABS_FIL ${FILE} ABSOLUTE)
    get_filename_component(FIL_WE ${FILE} NAME_WE)
    get_filename_component(REL_DIR ${FILE} DIRECTORY)
    set(RELFIL_WE ${REL_DIR}/${FIL_WE})

    set(OUTPUT_FILE_PATH_GRPC_SRC "${_gRPC_PROTO_GENS_DIR}/${RELFIL_WE}.grpc.pb.cc")
    set(OUTPUT_FILE_PATH_GRPC_HDR "${_gRPC_PROTO_GENS_DIR}/${RELFIL_WE}.grpc.pb.h")
    set(OUTPUT_FILE_PATH_PB_SRC "${_gRPC_PROTO_GENS_DIR}/${RELFIL_WE}.pb.cc")
    set(OUTPUT_FILE_PATH_PB_HDR "${_gRPC_PROTO_GENS_DIR}/${RELFIL_WE}.pb.h")
    set(OUTPUT_FILE_PATH_GRPC_MOCK_HDR "${_gRPC_PROTO_GENS_DIR}/${RELFIL_WE}_mock.grpc.pb.h")
    set(OUTPUT_FILES
        ${OUTPUT_FILE_PATH_GRPC_SRC}
        ${OUTPUT_FILE_PATH_GRPC_HDR}
        ${OUTPUT_FILE_PATH_PB_SRC}
        ${OUTPUT_FILE_PATH_PB_HDR}
        ${OUTPUT_FILE_PATH_GRPC_MOCK_HDR}
    )

    set(GENERATE_GRPC_SOURCES FALSE)

    foreach(OUTPUT_FILE ${OUTPUT_FILES})
        if(NOT EXISTS ${OUTPUT_FILE})
            set(GENERATE_GRPC_SOURCES TRUE)
            message("File does not exist: " ${OUTPUT_FILE})
        else()
            # message("File already exists: " ${OUTPUT_FILE})
        endif()
    endforeach()

    if(${GENERATE_GRPC_SOURCES})
        add_custom_command(
            OUTPUT ${OUTPUT_FILES}
            COMMAND ${_gRPC_PROTOBUF_PROTOC_EXECUTABLE}
            ARGS --grpc_out=generate_mock_code=true:${_gRPC_PROTO_GENS_DIR}
            --cpp_out ${_gRPC_PROTO_GENS_DIR}
            -I "./"
            --plugin=protoc-gen-grpc=${_GRPC_CPP_PLUGIN_EXECUTABLE}
            "${FILE}"
            WORKING_DIRECTORY ${_gRPC_PROTO_SRCS_DIR}
            COMMENT "Running gRPC C++ protocol buffer compiler for ${FILE}"
            VERBATIM)
    else()
        message("Skipping generation of " ${FILE})
    endif()

    list(APPEND PROTO_SRCS ${OUTPUT_FILE_PATH_GRPC_SRC})
    list(APPEND PROTO_SRCS ${OUTPUT_FILE_PATH_PB_SRC})
    list(APPEND PROTO_HDRS ${OUTPUT_FILE_PATH_GRPC_HDR})
    list(APPEND PROTO_HDRS ${OUTPUT_FILE_PATH_PB_HDR})
endforeach()

include_directories(${CMAKE_BINARY_DIR}/gens)

set(TARGET_NAME "vehicle-app-sdk-generated-grpc")

add_library(${TARGET_NAME} STATIC
    ${PROTO_SRCS}
)

target_link_libraries(${TARGET_NAME}
    CONAN_PKG::grpc
)
