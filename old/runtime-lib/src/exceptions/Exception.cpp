#include "exceptions/Exception.hpp"
#include <sstream>

using elrond::runtime::Exception;

Exception::Exception():
_prev(nullptr), _message("Exception"), message(_message){}

Exception::Exception(const Exception &e):
_prev(nullptr), _message(e._message), message(_message){
    if(e._prev != nullptr) this->_prev.reset(new Exception(*e._prev));
}

Exception::Exception(elrond::String message, const Exception &prev):
_prev(new Exception(prev)), _message(message), message(_message){}

Exception::Exception(elrond::String message):
_prev(nullptr), _message(message), message(_message){}

Exception::Exception(const std::exception &e):
_prev(nullptr), _message(e.what()), message(_message){}

Exception::~Exception(){}

void Exception::what(std::ostream &oss) const {
    elrond::sizeT i = 0;
    const Exception *le = this;

    oss << std::endl << " ********** EXCEPTION STACK **********" << std::endl;
    Exception::dumpStack(oss, *this, i, le);
}

elrond::String Exception::what() const {
    std::ostringstream oss;
    this->what(oss);
    return oss.str();
}

void Exception::dumpStack(std::ostream &oss, const Exception &e, elrond::sizeT &i, const Exception *&le){
    if(e._prev != nullptr) Exception::dumpStack(oss, *(e._prev), i, le);
    else le = &e;
    oss << "  #" << i++ << ": " << e.message << std::endl;
}
