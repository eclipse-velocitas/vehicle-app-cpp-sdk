# Vehicle App C++ SDK

![SDK CI Workflow](https://github.com/eclipse-velocitas/vehicle-app-cpp-sdk/actions/workflows/ci.yml/badge.svg)
[![License: Apache](https://img.shields.io/badge/License-Apache-yellow.svg)](http://www.apache.org/licenses/LICENSE-2.0)

The Vehicle App SDK for C++ allows to create `Vehicle Apps` from the [Velocitas Development Model](https://eclipse.dev/velocitas/docs/concepts/development_model/) in the C++ programming language.

## Folder structure

* 📁 `examples` - example vehicle apps showcasing the use of the SDK
    * 📁 `seat-adjuster` - an example application showing how to adjust the driver seat when receiving MQTT messages
    * 📁 `set-data-points` - an example application showing how to set single and multiple data points
    * 📁 `vehicle_model` - a handwritten example model to be used by all examples
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
To build the SDK, run the build script:
```bash
./build.sh
```

## Starting the runtime

Open the `Run Task` view in VSCode and select `Local - Start VehicleApp runtime`.

## Launching the example
With the runtime running in the background, you can run the app.

### Without debugging

Open the `Run Task` view in VSCode and select `Local - VehicleApp (Dapr run)`.

### With debugging
You can simply launch the example in the Debugging Tab. Make sure the `Example - <example of your choice>` is selected at the top. After the selection is done, you can also simply hit `F5`, to start the debugging session. 

*Note: This launch task will also make sure to re-build the app if it has been modified!*

### Run App and Dapr-Sidecar as individual Docker containers
#### Sidecar
```bash
docker run --net="host" --mount type=bind,source="$(pwd)"/.dapr,target=/.dapr daprio/daprd:edge ./daprd -app-id vehicleapp -dapr-grpc-port 50001 -dapr-http-port 3500 -components-path /.dapr/components -config /.dapr/config.yaml -app-protocol grpc
```
#### App
```bash
docker run --rm -it --net="host" -e DAPR_GRPC_PORT=50001 -e DAPR_HTTP_PORT=3500 localhost:12345/vehicleapp:local
```

### Middleware configuration for the example apps

You can configure the middleware to be used (i.e. either `dapr` or `native`) via environment variables of the app processs.

| Middleware | Environment Variable            | Default             | Meaning
|------------|---------------------------------|---------------------|------------------------------------
|            | `SDV_MIDDLEWARE_TYPE`           | `dapr`              | Defines the middleware to be used by the app (either `dapr` or `native`)
|
| Dapr       | `SDV_MQTT_ADDRESS`              | `localhost:1883`    | Address of the MQTT broker - needs to be set explicitly as the C++ SDK does not support Dapr PubSub, yet
|            | `SEATSERVICE_DAPR_APP_ID`       | `seatservice`       | Application id used by Dapr to discover the application providing the seat service
|            | `VEHICLEDATABROKER_DAPR_APP_ID` | `vehicledatabroker` | Application id used by Dapr to discover the application providing the Kuksa (Vehicle) Data Broker
|            | (`DAPR_GRPC_PORT`)              | -                   | Usually, `DAPR_GRPC_PORT` and `DAPR_HTTP_PORT` don't need to be configured manually. They are set by `dapr run` when starting the app and its sidecar.
|            | (`DAPR_HTTP_PORT`)              | -                   | If you need to start your app separately, please make sure these environment variables are equally available to both the app and its sidecar with identical values (see above).
|            | 
| native     | `SDV_MQTT_ADDRESS`              | `localhost:1883`    | Address of the MQTT broker
|            | `SDV_SEATSERVICE_ADDRESS`       | -                   | Address of the seat service
|            | `SDV_VEHICLEDATABROKER_ADDRESS` | `localhost:55555`   | Address of the Kuksa (Vehicle) Data Broker

## Documentation
* [Velocitas Development Model](https://eclipse.dev/velocitas/docs/concepts/development_model/)
* [Vehicle App SDK Overview](https://eclipse.dev/velocitas/docs/concepts/development_model/vehicle_app_sdk/)

## Quickstart Tutorials
1. [Setup and Explore Development Environment](https://eclipse.dev/velocitas/docs/tutorials/quickstart/)
1. [Develop your own Vehicle Model](https://eclipse.dev/velocitas/docs/tutorials/vehicle_model_creation/)
1. [Develop your own Vehicle App](https://eclipse.dev/velocitas/docs/tutorials/vehicle_app_development/)

## Contribution
- [GitHub Issues](https://github.com/eclipse-velocitas/vehicle-app-cpp-sdk/issues)
- [Mailing List](https://accounts.eclipse.org/mailing-list/velocitas-dev)
- [Contribution](CONTRIBUTING.md)
