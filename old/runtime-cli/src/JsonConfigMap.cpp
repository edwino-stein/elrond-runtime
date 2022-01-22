#include "JsonConfigMap.hpp"

JsonConfigMap::JsonConfigMap(nlohmann::json const& data): data(data){}

int JsonConfigMap::asInt(const char* key) const
{
    nlohmann::json v = this->data[key];
    return this->decodeInt(v);
}

long JsonConfigMap::asLong(const char* key) const
{
    nlohmann::json v = this->data[key];
    return this->decodeInt(v);
}

bool JsonConfigMap::asBool(const char* key) const
{
    nlohmann::json v = this->data[key];
    return this->decodeBool(v);
}

char JsonConfigMap::asChar(const char* key) const
{
    nlohmann::json v = this->data[key];
    auto s = this->decodeString(v);
    return s.size() > 0 ? s[0] : 0;
}

double JsonConfigMap::asDouble(const char* key) const
{
    nlohmann::json v = this->data[key];
    return this->decodeDouble(v);
}

const char* JsonConfigMap::asString(const char* key) const
{
    nlohmann::json v = this->data[key];
    auto s = this->decodeString(v);

    StringPoolT& stringPool = ((JsonConfigMap*) this)->getStringPool();
    stringPool.push_back(s);

    return this->stringPool.back().c_str();
}

int JsonConfigMap::asString(const char* key,
                            char value[],
                            const elrond::sizeT len) const
{
    nlohmann::json v = this->data[key];
    auto s = this->decodeString(v);
    return s.copy(value, len, 0);
}

bool JsonConfigMap::isInt(const char* key) const
{ return this->data[key].is_number_integer(); }

bool JsonConfigMap::isLong(const char* key) const
{ return this->data[key].is_number_integer(); }

bool JsonConfigMap::isDouble(const char* key) const
{ return this->data[key].is_number_float(); }

bool JsonConfigMap::isBool(const char* key) const
{ return this->data[key].is_boolean(); }

bool JsonConfigMap::isChar(const char* key) const
{ return this->data[key].is_string(); }

bool JsonConfigMap::isString(const char* key) const
{ return this->data[key].is_string(); }

bool JsonConfigMap::isNull(const char* key) const
{
    nlohmann::json v = this->data[key];
    return v.is_null() || v.is_object() || v.is_array();
}

long JsonConfigMap::decodeInt(nlohmann::json const& v) const
{
    if(v.is_number()) return v.get<long>();
    if(v.is_boolean()) return this->decodeBool(v) ? 1 : 0;
    if(v.is_string()){
        auto s = this->decodeString(v);
        if(s.size() == 0) return 0;
        if(s.size() == 1) return s[0];
        try { return std::stoi(s); }
        catch(std::exception const& e){}
    }

    return 0;
}

double JsonConfigMap::decodeDouble(nlohmann::json const& v) const
{
    if(v.is_number()) return v.get<double>();
    if(v.is_boolean()) return this->decodeBool(v) ? 1 : 0;
    if(v.is_string()){
        auto s = this->decodeString(v);
        if(s.size() == 0) return 0;
        if(s.size() == 1) return s[0];
        try { return std::stof(s); }
        catch(std::exception const& e){}
    }

    return 0;
}

bool JsonConfigMap::decodeBool(nlohmann::json const& v) const
{
    if(v.is_boolean()) return v.get<bool>();
    if(v.is_number()) return this->decodeInt(v) != 0;
    if(v.is_string()){
        auto s = this->decodeString(v);
        if(s.size() == 0) return false;
        if(s.size() == 1) return s[0] == 't' || s[0] == 'T';
        return s == "true" || s == "True" || s == "TRUE";
    }

    return false;
}

elrond::String JsonConfigMap::decodeString(nlohmann::json const& v) const
{
    if(v.is_string()) return v.get<elrond::String>();
    if(v.is_number_integer()) return std::to_string(this->decodeInt(v));
    if(v.is_number_float()) return std::to_string(this->decodeDouble(v));
    if(v.is_boolean()) return this->decodeBool(v) ? "true" : "false";
    return "";
}

JsonConfigMap::StringPoolT& JsonConfigMap::getStringPool()
{ return this->stringPool; }
