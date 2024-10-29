# Vehicle App C++ SDK

![SDK CI Workflow](https://github.com/eclipse-velocitas/vehicle-app-cpp-sdk/actions/workflows/ci.yml/badge.svg)
[![License: Apache](https://img.shields.io/badge/License-Apache-yellow.svg)](http://www.apache.org/licenses/LICENSE-2.0)

The Vehicle App SDK for C++ allows to create `Vehicle Apps` from the [Velocitas Development Model](https://eclipse.dev/velocitas/docs/concepts/development_model/) in the C++ programming language.

## Folder structure

* 📁 `examples` - example vehicle apps showcasing the use of the SDK
    * 📁 `seat-adjuster` - an example application showing how to adjust the driver seat when receiving MQTT messages
    * 📁 `set-data-points` - an example application showing how to set single and multiple data points
    * 📁 `example_model` - a handwritten example model used by some of the examples.
    * 📁 `performance-subscribe` - an example application subscribing to signals and printing time and value when updates are received.
    * 📁 `grpc_client` - an example application implementing a gRPC client, matching the example in [Velocitas documentation](https://eclipse.dev/velocitas/docs/tutorials/grpc_service_generation/create_client/).
    * 📁 `grpc_server` - an example application implementing a gRPC server, matching the example in [Velocitas documentation](https://eclipse.dev/velocitas/docs/tutorials/grpc_service_generation/create_server/).
* 📁 `sdk`
    * 📁 `include` - the headers which need to be included by users of the SDK
    * 📁 `src` - contains the source code for the SDK from which the SDK library is built
    * 📁 `test` - contains the unit test code for the SDK

## Prerequisites

* Visual Studio Code with the Remote Containers extension
* Docker installation on the host

## Building

### Installing dependencies
Before a build can be started, all dependencies required by the SDK need to be installed. Issue the following command in the SDK root directory:
```bash
./install_dependencies.sh
```

### Proxy Issues
If you are working behind a corporate proxy, the `install_dependcies.sh` (which is also called during devcontainer build!) might probably fail
downloading 3rd party packages via https with a TLS/SSL certificate validation error (we actually saw this issue in the build of gRPC trying to
download the opencensus-proto package from storage.googleapis.com).

Please have a look at our ["working behind proxy" tutorial](https://eclipse.dev/velocitas/docs/tutorials/quickstart/behind_proxy/)
to get hints how to possibly overcome this.

### Building the SDK
To build the SDK (including examples), run the build script:
```bash
./build.sh
```

## Starting the runtime

Open the `Run Task` view in VSCode and select `Local Runtime - Up`.

## Launching the example
With the runtime running in the background, you can run the app.

### Without debugging

Open the `Run Task` view in VSCode and select `Local Runtime - Start SeatAdjuster`.

### With debugging
You can simply launch the example in the Debugging Tab. Make sure the `Example - <example of your choice>` is selected at the top. After the selection is done, you can also simply hit `F5`, to start the debugging session.

*Note: This launch task will also make sure to re-build the app if it has been modified!*

### Run App as Docker container
```bash
docker run --rm -it --net="host" -e SDV_MIDDLEWARE_TYPE="native" -e SDV_MQTT_ADDRESS="localhost:1883" -e SDV_VEHICLEDATABROKER_ADDRESS="localhost:55555" localhost:12345/vehicleapp:local
```

### Middleware configuration for the example apps

You can configure the middleware to be used (currently only `native` is supported) via environment variables of the app processs.

| Middleware | Environment Variable            | Default             | Meaning
|------------|---------------------------------|---------------------|------------------------------------
|            | `SDV_MIDDLEWARE_TYPE`           | `native`            | Defines the middleware to be used by the app (currently only `native` is supported)
|
| native     | `SDV_MQTT_ADDRESS`              | `localhost:1883`    | Address of the MQTT broker
|            | `SDV_VEHICLEDATABROKER_ADDRESS` | `localhost:55555`   | Address of the Kuksa (Vehicle) Data Broker

### Using MQTT Broker with authentication

If you have configured a MQTT Broker which requires authentication, you can also configure your app to properly connect to the broker. Currently the SDK supports authentication via credentials (username and password), tokens (depends on the broker) and certificates. In order to enable this feature you need to use the native middleware (set `SDV_MIDDLEWARE_TYPE` to `native`). To configure the authentication, you have to provide the details directly in the constructor of the app, see the example for username and password below:

```cpp
SampleApp::SampleApp()
    : VehicleApp(velocitas::IVehicleDataBrokerClient::createInstance("vehicledatabroker"),
                 velocitas::IPubSubClient::createInstance("localhost:1883", "SampleApp",
                                                          "username", "password")) {}
```

## Documentation
* [Velocitas Development Model](https://eclipse.dev/velocitas/docs/concepts/development_model/)
* [Vehicle App SDK Overview](https://eclipse.dev/velocitas/docs/concepts/development_model/vehicle_app_sdk/)

## Quickstart Tutorials
1. [Setup and Explore Development Environment](https://eclipse.dev/velocitas/docs/tutorials/quickstart/)
1. [Develop your own Vehicle Model](https://eclipse.dev/velocitas/docs/tutorials/vehicle_model_creation/)
1. [Develop your own Vehicle App](https://eclipse.dev/velocitas/docs/tutorials/vehicle_app_development/)

## Contribution

### Release Management

The SDK is installed into the Velocitas Development environment by the [Velocitas Package for DevContainer Setup](https://github.com/eclipse-velocitas/devenv-devcontainer-setup).
To release a new version re-run all GitHub actions manually and verify that they all pass.
Then create a new GitHub release and tag the commit used. No need to include any additional artifacts.

After creating the GitHub release, verify that the new release can be used by the [Vehicle App C++ Template](https://github.com/eclipse-velocitas/vehicle-app-cpp-template).
Update the [conanfile.txt](https://github.com/eclipse-velocitas/vehicle-app-cpp-template/blob/main/conanfile.txt) with the new version in the line

```cpp
vehicle-app-sdk/X.Y.Z
```

Then verify that the example application in the template can be successfully built.
After successful verification, consider creating a Pull Request in the [Vehicle App C++ Template repository](https://github.com/eclipse-velocitas/vehicle-app-cpp-template) tp update the SDK version.

### Resources

- [GitHub Issues](https://github.com/eclipse-velocitas/vehicle-app-cpp-sdk/issues)
- [Mailing List](https://accounts.eclipse.org/mailing-list/velocitas-dev)
- [Contribution](CONTRIBUTING.md)
