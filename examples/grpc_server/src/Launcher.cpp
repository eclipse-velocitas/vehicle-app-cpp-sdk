#include "SeatsServiceImpl.h"
#include <sdk/middleware/Middleware.h>
#include <services/seats/SeatsServiceServerFactory.h>

#include "vehicle/Vehicle.hpp"

#include <memory>

using namespace velocitas;

int main(int argc, char** argv) {
    auto seatsImpl = std::make_shared<SeatsService>();

    velocitas::VehicleModelContext::getInstance().setVdbc(
        velocitas::IVehicleDataBrokerClient::createInstance("vehicledatabroker"));

    auto seatServer = SeatsServiceServerFactory::create(Middleware::getInstance(), seatsImpl);

    seatServer->Wait();

    std::cout << "Waiting!" << std::endl;
    return 0;
}
