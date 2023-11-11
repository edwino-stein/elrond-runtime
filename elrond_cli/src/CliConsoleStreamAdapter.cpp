#include "CliConsoleStreamAdapter.hpp"
#include "CliConsoleAdapter.hpp"

using elrond::CliConsoleAdapter;
using elrond::CliConsoleStreamAdapter;
using elrond::runtime::OStringStream;
using elrond::interface::Stream;

CliConsoleStreamAdapter::CliConsoleStreamAdapter(CliConsoleAdapter& adapter, const std::string& tag):
_adapter(&adapter), _tag(tag) {}

void CliConsoleStreamAdapter::flush(Stream& stream, elrond::SEVERITY severity)
{
    try {
        this->_adapter->flush(
            this->_tag,
            severity,
            reinterpret_cast<OStringStream&>(stream).oss().str()
        );
    }
    catch(const std::exception&){}
}

elrond::pointer<Stream> CliConsoleStreamAdapter::makeStream() const
{ return std::make_shared<OStringStream>(); }
