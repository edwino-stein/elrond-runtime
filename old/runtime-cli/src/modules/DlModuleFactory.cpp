#include "modules/DlModuleFactory.hpp"

#include <dlfcn.h>

using elrond::runtime::ModuleFactory;

using elrond::runtime::Exception;
using elrond::interface::Runtime;
using elrond::interface::Module;

DlModuleFactory::DlModuleFactory(elrond::String path, Runtime& app): ModuleFactory(path)
{
    try{

        this->dlHandle = dlopen(path.c_str(), RTLD_NOW | RTLD_GLOBAL);
        if(!this->dlHandle){
            throw Exception("Invalid shared object (.so) file", Exception(dlerror()));
        }

        ModSetAppT setAppInstance = (ModSetAppT) dlsym(
            this->dlHandle,
            ELROND_TO_STR(ELROND_MOD_SET_APP_FUNC_N)
        );
        if(setAppInstance == nullptr) throw dlerror();

        this->_newInstance = (ModNewT) dlsym(
            this->dlHandle,
            ELROND_TO_STR(ELROND_MOD_NEW_INST_FUNC_N)
        );
        if(this->_newInstance == nullptr) throw dlerror();

        this->_delInstance = (ModDeleteT) dlsym(
            this->dlHandle,
            ELROND_TO_STR(ELROND_MOD_DEL_INST_FUNC_N)
        );
        if(this->_delInstance == nullptr) throw dlerror();

        ModNumHandleT getApiVersion = (ModNumHandleT) dlsym(
            this->dlHandle,
            ELROND_TO_STR(ELROND_MOD_API_VER_FUNC_N)
        );
        if(getApiVersion == nullptr) throw dlerror();

        ModStringHandleT getMainClass = (ModStringHandleT) dlsym(
            this->dlHandle,
            ELROND_TO_STR(ELROND_MOD_MAIN_CLASS_FUNC_N)
        );
        if(getMainClass == nullptr) throw dlerror();

        setAppInstance(&app);
        this->_info.apiVer = getApiVersion();
        this->_info.mainClass = getMainClass();
    }
    catch(Exception &e){
        throw Exception("Unable to load the module from \"" + path + "\"", e);
    }

    ModStringHandleT getPrettyName = (ModStringHandleT) dlsym(
        this->dlHandle,
        ELROND_TO_STR(ELROND_MOD_PRETTY_NAME_FUNC_N)
    );

    ModStringHandleT getAuthorName = (ModStringHandleT) dlsym(
        this->dlHandle,
        ELROND_TO_STR(ELROND_MOD_AUTHOR_NAME_FUNC_N)
    );

    ModStringHandleT getAuthorEmail = (ModStringHandleT) dlsym(
        this->dlHandle,
        ELROND_TO_STR(ELROND_MOD_AUTHOR_EMAIL_FUNC_N)
    );

    ModStringHandleT getVersion = (ModStringHandleT) dlsym(
        this->dlHandle,
        ELROND_TO_STR(ELROND_MOD_VERSION_FUNC_N)
    );

    if(getPrettyName != nullptr) this->_info.prettyName = getPrettyName();
    if(getAuthorName != nullptr) this->_info.authorName = getAuthorName();
    if(getAuthorEmail != nullptr) this->_info.authorEmail = getAuthorEmail();
    if(getVersion != nullptr) this->_info.version = getVersion();
}

DlModuleFactory::~DlModuleFactory()
{
    if(this->dlHandle != nullptr){
        dlclose(this->dlHandle);
        this->dlHandle = nullptr;
    }

    this->_newInstance = nullptr;
    this->_delInstance = nullptr;
}

Module* DlModuleFactory::newInstance(elrond::String const& instName)
{ return this->_newInstance != nullptr ? this->_newInstance() : nullptr; }

void DlModuleFactory::deleteInstance(Module* inst)
{ if(this->_delInstance != nullptr) this->_delInstance(inst); }
