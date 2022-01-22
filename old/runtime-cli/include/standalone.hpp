#if !defined  _ELROND_RUNTIME_STANDALONE_HPP
    #define _ELROND_RUNTIME_STANDALONE_HPP

    #include "elrond-runtime.hpp"
    #include "json.hpp"

    using Json = nlohmann::json;

    void loadApplication(elrond::runtime::RuntimeApp& app,
                         const int argc,
                         char const* argv[]);

    void stopApplication(elrond::runtime::RuntimeApp& app, const int code);
    elrond::runtime::ModulesFactoriesV newStandaloneModulesFactorories();
    void parseModules(elrond::runtime::RuntimeApp& app, Json& cfg);
    void initModules(elrond::runtime::RuntimeApp& app, Json& cfg);
    void parseChmgrs(elrond::runtime::RuntimeApp& app, Json &cfg);
    void readJsonFromFile(elrond::String file, Json& json);
    int run(int argc, char const* argv[]);

#endif
