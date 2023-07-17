#include "application/ExternalFactoryAdapter.hpp"
#include "platform/DlHandle.ipp"

using elrond::application::ExternalFactoryAdapter;
using elrond::application::FactoryAdapter;
using elrond::platform::parseExternalFactoryFromDlHandle;

ExternalFactoryAdapter::ExternalFactoryAdapter(const std::string& path)
: FactoryAdapter(nullptr), _handle(path)
{ this->_factory = &(parseExternalFactoryFromDlHandle(_handle)); }
