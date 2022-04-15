#include "Stacktrace.hpp"

#include <execinfo.h>
#include <cxxabi.h>

Stacktrace::Stacktrace(){}

Stacktrace::StackSymbol::StackSymbol(void* addr, elrond::sizeT id, elrond::String backtrace):
addr(addr), id(id), backtrace(backtrace), name(""), offset(""), path("")
{}

Stacktrace::StackSymbol::StackSymbol(const StackSymbol& s):
addr(s.addr), id(s.id), backtrace(s.backtrace), name(s.name), offset(s.offset), path(s.path)
{}

void Stacktrace::parseName(Stacktrace::StackSymbol& symbol,
                           const elrond::sizeT beginName,
                           const elrond::sizeT beginOffset)
{
    char name[beginOffset - beginName];
    symbol.backtrace.copy(name, (beginOffset - beginName) - 1, beginName + 1);
    name[beginOffset - beginName - 1] = '\0';

    int status;
    elrond::sizeT nameLen = 1024;
    char demangledName[nameLen];

    abi::__cxa_demangle(name, &demangledName[0], &nameLen, &status);

    if(status == 0){
        symbol.name.append(demangledName);
    }
    else{
        symbol.name.append(name);
        if(symbol.name.size() > 0) symbol.name.append("()");
    }
}

void Stacktrace::parseOffset(Stacktrace::StackSymbol& symbol,
                             const elrond::sizeT beginOffset,
                             const elrond::sizeT endOffset)
{
    char offset[endOffset - beginOffset + 1];
    symbol.backtrace.copy(offset, (endOffset - beginOffset), beginOffset);
    offset[endOffset - beginOffset] = '\0';
    symbol.offset.append(offset);
}

void Stacktrace::parsePath(StackSymbol& symbol, const elrond::sizeT beginName)
{
    char path[beginName];
    symbol.backtrace.copy(path, beginName, 0);
    path[beginName] = '\0';
    symbol.path.append(path);
}

void Stacktrace::parseSymbol(Stacktrace::StackSymbol& symbol)
{
    elrond::sizeT i = 0;
    elrond::sizeT beginName = 0;
    elrond::sizeT beginOffset = 0;
    elrond::sizeT endOffset = 0;

    std::for_each(
        symbol.backtrace.begin(),
        symbol.backtrace.end(),
        [&i, &beginName, &beginOffset, &endOffset]
        (char const &c)
        {
            switch (c) {
                case '(': beginName = i; break;
                case '+': beginOffset = i; break;
                case ')': if(beginOffset != 0) endOffset = i; break;
            }
            ++i;
        }
    );

    if(beginName == 0 || beginOffset == 0 || endOffset == 0 || beginName >= beginOffset)
        return;

    Stacktrace::parseName(symbol, beginName, beginOffset);
    Stacktrace::parseOffset(symbol, beginOffset, endOffset);
    Stacktrace::parsePath(symbol, beginName);
}

bool Stacktrace::each(Stacktrace::eachSymbolCallbackT handle,
                      const elrond::sizeT depth,
                      const elrond::sizeT skip)
{
    void* addrs[depth + 1];
    const elrond::sizeT addrLen = backtrace(addrs, sizeof(addrs) / sizeof(void*));

    if(addrLen == 0) return false;

    std::unique_ptr<char*[]> symbols(backtrace_symbols(addrs, addrLen));

    for(elrond::sizeT i = skip; i < addrLen; ++i)
    {
        Stacktrace::StackSymbol symbol(addrs[i], i, symbols[i]);
        Stacktrace::parseSymbol(symbol);
        handle(symbol);
    }

    return true;
}

void Stacktrace::dump(std::ostream& os,
                      const elrond::sizeT skip,
                      const elrond::sizeT depth)
{
    os << " ************* STACKTRACE ************" << std::endl;

    elrond::sizeT i = 0;
    Stacktrace::each(
        [&os, &i] (Stacktrace::StackSymbol &symbol)
        {
            os << "  #" << i++ << ": ";
            if(!symbol.name.empty()) os << symbol.name << ' ';
            os << '[' << symbol.offset << ", " << symbol.path << ']' << std::endl;
        },
        depth,
        skip + 2
    );
}

void Stacktrace::dump(Stacktrace::SymbolCollectionT& collection,
                      const elrond::sizeT skip,
                      const elrond::sizeT depth)
{
    Stacktrace::each(
        [&collection] (Stacktrace::StackSymbol &symbol)
        { collection.push_back(Stacktrace::StackSymbol(symbol)); },
        depth,
        skip + 2
    );
}

void Stacktrace::dump(std::ostream& os, const SymbolCollectionT& collection)
{
    os << " ************* STACKTRACE ************" << std::endl;

    elrond::sizeT i = 0;
    std::for_each(
        collection.begin(),
        collection.end(),
        [&os, &i](Stacktrace::StackSymbol symbol)
        {
            os << "  #" << i++ << ": ";
            if(!symbol.name.empty()) os << symbol.name << ' ';
            os << '[' << symbol.offset << ", " << symbol.path << ']' << std::endl;
        }
    );
}
