#if !defined  _ELROND_JSON_CONFIGMAP_STANDALONE_HPP
    #define _ELROND_JSON_CONFIGMAP_STANDALONE_HPP

    #include "elrond-runtime.hpp"
    #include "json.hpp"

    class JsonConfigMap : public elrond::interface::ConfigMap {
        private:
            using StringPoolT = std::vector<elrond::String>;

        protected:
            nlohmann::json const& data;
            StringPoolT stringPool;

            StringPoolT& getStringPool();

            long decodeInt(nlohmann::json const& v) const;
            double decodeDouble(nlohmann::json const& v) const;
            bool decodeBool(nlohmann::json const& v) const;
            elrond::String decodeString(nlohmann::json const& v) const;

        public:

            JsonConfigMap(nlohmann::json const& data);

            int asInt(const char* key) const override;
            long asLong(const char* key) const override;
            bool asBool(const char* key) const override;
            char asChar(const char* key) const override;
            double asDouble(const char* key) const override;
            const char* asString(const char* key) const override;

            int asString(const char* key, char value[],
                         const elrond::sizeT len) const override;

            bool isInt(const char* key) const override;
            bool isLong(const char* key) const override;
            bool isDouble(const char* key) const override;
            bool isBool(const char* key) const override;
            bool isChar(const char* key) const override;
            bool isString(const char* key) const override;
            bool isNull(const char* key) const override;

    };

#endif
