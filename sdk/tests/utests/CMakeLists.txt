set(TARGET_NAME "sdk_utests")

add_executable(${TARGET_NAME}
    AsyncResult_tests.cpp
    AsyncSubscription_tests.cpp
    Job_tests.cpp
    Node_tests.cpp
    ScopedBoolInverter_tests.cpp
    ThreadPool_tests.cpp
    Utils_tests.cpp
    VehicleDataBrokerClient_tests.cpp
    #PubSub_tests.cpp
)

include_directories(
    ${PROJECT_SOURCE_DIR}/sdk/include
)

add_dependencies(${TARGET_NAME} 
    vehicle-app-sdk
)

target_link_libraries(${TARGET_NAME} 
    vehicle-app-sdk
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(${TARGET_NAME})