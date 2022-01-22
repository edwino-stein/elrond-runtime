#include "modules/VirtualGpio.hpp"

using elrond::module::BaseGpioModule;
using elrond::interface::ConfigMap;
using elrond::gpio::BaseGpioPin;
using elrond::gpio::DOutPin;
using elrond::gpio::ServoPin;
using elrond::gpio::PwmPin;

VirtualGpio::~VirtualGpio(){}

void VirtualGpio::onInit(ConfigMap& cfg, elrond::LoopControl& lc)
{
    lc.enable = false;
    lc.ownThread = false;
    lc.interval = 0;
}

void VirtualGpio::attach(BaseGpioPin& pin)
{
    switch (pin.getType()) {

        case elrond::GpioType::DOUT:

            pin.setReadHandle(
                [](BaseGpioPin &pin)
                { return ((DOutPin &) pin).value; }
            );

            pin.setWriteHandle(
                [](elrond::gpio::BaseGpioPin &pin, const elrond::word data)
                {
                    ((DOutPin &) pin).value = data;
                    std::cout << "#" << pin.getNumber() << " = " << (data > 0 ? "HIGH" : "LOW" ) << std::endl;
                }
            );

        break;

        case elrond::GpioType::PWM:

            pin.setReadHandle(
                [](BaseGpioPin &pin)
                { return ((elrond::gpio::PwmPin &) pin).value; }
            );

            pin.setWriteHandle(
                [](BaseGpioPin &pin, const elrond::word data)
                {
                    ((elrond::gpio::PwmPin &) pin).value = data;
                    std::cout << "#" << pin.getNumber() << " = " << data << std::endl;
                }
            );

        break;

        case elrond::GpioType::SERVO:

            pin.setReadHandle(
                [](BaseGpioPin &pin)
                { return ((elrond::gpio::ServoPin &) pin).value; }
            );

            pin.setWriteHandle(
                [](BaseGpioPin &pin, const elrond::word  data)
                {
                    ((elrond::gpio::ServoPin &) pin).value = data;
                    std::cout << "#" << pin.getNumber() << " = " << elrond::map(data, LOW_VALUE, HIGH_VALUE, 0, 180)  << std::endl;
                }
            );

        break;

        case elrond::GpioType::UNKNOWN:
        default:
            elrond::error("GPIO type error");
    }
}

ELROND_DEFINE_INTER_MOD(
    elrond::runtime::VirtualGpio,
    "Virtual Gpio",
    "Edwino Stein",
    "edwino.stein@gmail.com"
)
