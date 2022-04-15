#include "modules/Serial.hpp"

#include <cstring>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

using elrond::interface::ConfigMap;
using elrond::channel::BaseChannelManager;

Serial::~Serial(){}

void Serial::onInit(ConfigMap& cfg, elrond::LoopControl& lc)
{
    if(!cfg.isString("path")) elrond::error("Invalid or missing key \"path\".");
    elrond::String path = cfg.asString("path");

    if(!cfg.isInt("speed")) elrond::error("Invalid or missing key \"speed\".");
    speed_t speed = Serial::getSpeed(cfg.asInt("speed"));
    if(speed == B0) elrond::error("Invalid or missing key \"speed\".");

    memset(&(this->tty), 0, sizeof (this->tty));
    this->port = open(path.c_str(), O_RDWR);
    if(tcgetattr(this->port, &(this->tty)) != 0) elrond::error(strerror(errno));

    this->tty.c_cflag &= ~PARENB;           // Clear parity bit, disabling parity (most common)
    this->tty.c_cflag &= ~CSTOPB;           // Clear stop field, only one stop bit used in communication (most common)
    this->tty.c_cflag |= CS8;               // 8 bits per byte (most common)
    this->tty.c_cflag &= ~CRTSCTS;          // Disable RTS/CTS hardware flow control (most common)
    this->tty.c_cflag |= CREAD | CLOCAL;    // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    this->tty.c_lflag &= ~ICANON;
    this->tty.c_lflag &= ~ECHO;             // Disable echo
    this->tty.c_lflag &= ~ECHOE;            // Disable erasure
    this->tty.c_lflag &= ~ECHONL;           // Disable new-line echo
    this->tty.c_lflag &= ~ISIG;             // Disable interpretation of INTR, QUIT and SUSP
    this->tty.c_iflag &= ~(IXON | IXOFF | IXANY);   // Turn off s/w flow ctrl
    this->tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);  // Disable any special handling of received bytes

    this->tty.c_oflag &= ~OPOST;            // Prevent special interpretation of output bytes (e.g. newline chars)
    this->tty.c_oflag &= ~ONLCR;            // Prevent conversion of newline to carriage return/line feed
    // tty.c_oflag &= ~OXTABS;                 // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
    // tty.c_oflag &= ~ONOEOT;                 // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

    this->tty.c_cc[VTIME] = 10;             // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    this->tty.c_cc[VMIN] = 0;

    // Set in/out baud rate
    cfsetispeed(&(this->tty), speed);
    cfsetospeed(&(this->tty), speed);

    lc.enable = true;
    lc.ownThread = true;
    lc.interval = 10;
}

void Serial::onStart()
{
    if(tcsetattr(this->port, TCSANOW, &(this->tty)) != 0)
        elrond::error(strerror(errno));
}

void Serial::onStop()
{ close(this->port); }

void Serial::loop()
{
    if(this->cm == nullptr) return;

    const elrond::sizeT length = this->cm->getRxBufferSize();
    elrond::byte buffer[length];

    const elrond::sizeT received = read(this->port, &buffer, length);
    if(received == length) this->cm->onReceive(buffer, length);

    tcflush(this->port, TCIOFLUSH);
}

void Serial::send(elrond::byte data[], const elrond::sizeT length)
{ write(this->port, data, length); }

void Serial::setChannelManager(BaseChannelManager* cm)
{ if(this->cm == nullptr) this->cm = cm; }

speed_t Serial::getSpeed(unsigned int speed)
{
    switch (speed) {
        case 50: return B50;
        case 75: return B75;
        case 110: return B110;
        case 134: return B134;
        case 150: return B150;
        case 300: return B300;
        case 600: return B600;
        case 1200: return B1200;
        case 1800: return B1800;
        case 2400: return B2400;
        case 4800: return B4800;
        case 9600: return B9600;
        case 19200: return B19200;
        case 38400: return B38400;
        case 57600: return B57600;
        case 115200: return B115200;
        case 230400: return B230400;
        case 460800: return B460800;
    }

    return B0;
}

ELROND_DEFINE_INTER_MOD(
    elrond::runtime::Serial,
    "Serial/UART Transport",
    "Edwino Stein",
    "edwino.stein@gmail.com"
)
