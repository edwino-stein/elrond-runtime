#include "CliConsoleAdapter.hpp"
#include "CliConsoleStreamAdapter.hpp"

#include <iomanip>
#include <chrono>
#include <ctime> 

using elrond::CliConsoleAdapter;
using elrond::CliConsoleStreamAdapter;
using elrond::interface::ConsoleStreamAdapter;
using elrond::runtime::Console;
using IConsole = elrond::interface::Console;

CliConsoleAdapter::CliConsoleAdapter(): coutMtx(), outputStreams({&std::cout}), tagWidth(15) {}

void CliConsoleAdapter::flush(const std::string& tag, elrond::SEVERITY severity, const std::string& msg)
{
    if (tag.size() > this->tagWidth) this->tagWidth = tag.size();
    std::ostringstream oss;
    oss << std::setw(this->tagWidth) << std::right << tag
        << ' ' << std::setw(7) << std::left << (severity == elrond::SEVERITY::INFO ? "[INFO]" : "[ERROR]") << ' '
        << std::left << msg;
    
    const std::lock_guard<std::mutex> lock(this->coutMtx);
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    for (auto o : this->outputStreams) (*o) << std::put_time(std::localtime(&now), "[%a %b %d %T]") << "  " << oss.str() << '\n';
}

elrond::pointer<IConsole> CliConsoleAdapter::makeConsole(const std::string& tag)
{
    return std::make_shared<Console>(this->makeConsoleStreamAdapter(tag));
}

elrond::pointer<ConsoleStreamAdapter> CliConsoleAdapter::makeConsoleStreamAdapter(const std::string& tag)
{
    return std::make_shared<CliConsoleStreamAdapter>(*this, tag);
}
