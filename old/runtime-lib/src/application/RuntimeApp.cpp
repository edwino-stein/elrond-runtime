#include "application/RuntimeApp.hpp"

#include "exceptions/Exception.hpp"
#include "modules/ModuleHandle.hpp"
#include "channel/ChannelManager.hpp"
#include "modules/InternalModuleFactory.ipp"

using elrond::runtime::RuntimeApp;
using elrond::runtime::ChannelManager;
using elrond::runtime::ChannelManagerP;
using elrond::runtime::ModuleHandle;
using elrond::runtime::ModuleHandleP;
using elrond::runtime::ModuleFactory;
using elrond::runtime::ModuleFactoryP;
using elrond::runtime::ModulesFactoriesV;
using elrond::runtime::ModuleInfo;
using elrond::runtime::InternalModuleFactory;
using elrond::runtime::Exception;

using elrond::interface::Runtime;
using elrond::interface::DebugOut;
using elrond::interface::ConfigMap;
using elrond::module::BaseGpioModule;
using elrond::module::BaseInputDriverModule;
using elrond::module::BaseTransportModule;
using elrond::channel::BaseChannelManager;

Runtime* ELROND_MOD_APP_VAR = nullptr;

RuntimeApp::RuntimeApp(DebugOut& dout): _dout(dout){ ELROND_MOD_APP_VAR = this; }
RuntimeApp::~RuntimeApp(){}

ModuleInfo const& RuntimeApp::defineModule(
    elrond::String name,
    elrond::String type,
    ModulesFactoriesV& factories
){
    ModuleFactoryP fac = RuntimeApp::findFactory(type, factories, this);
    this->instances.push_back(std::make_shared<ModuleHandle>(name, fac));
    return fac->info;
}

void RuntimeApp::initModule(elrond::String name, ConfigMap& cm) const
{
    try{
        ModuleHandleP mh = this->findModule(name);
        mh->instance->onInit(cm, mh->lc);
    }
    catch(Exception &e){
        throw Exception(
            "An error occurred when initializing the instance \"" + name +"\"",
            e
        );
    }
}

void RuntimeApp::startModules()
{
    std::for_each(
        this->instances.begin(),
        this->instances.end(),
        [](ModuleHandleP mh)
        {
            try {
                mh->instance->onStart();
            }
            catch(Exception &e){
                throw Exception(
                    "An error occurred when starting the instance \"" + mh->name +"\"",
                    e
                );
            }
            mh->started = true;
        }
    );
}

void RuntimeApp::stopModules()
{
    std::for_each(
        this->instances.begin(),
        this->instances.end(),
        [](ModuleHandleP mh)
        {
            try{
                if(mh->started){
                    mh->started = false;
                    mh->instance->onStop();
                }
            }
            catch(Exception &e){

                Exception exp(
                    "An error occurred when stopping the instance \"" + mh->name +"\"",
                    e
                );

                std::cout << " * An error occurred" << std::endl;
                std::cout << exp.what() << '\n';
            }
        }
    );
}

ModuleHandleP RuntimeApp::findModule(elrond::String name) const
{
    auto it = std::find_if(
        this->instances.begin(),
        this->instances.end(),
        [&name](ModuleHandleP mh){ return mh->name == name; }
    );

    if(it != this->instances.end()) return *it;
    throw Exception("No such module instance");
}

void RuntimeApp::start()
{
    std::for_each(
        this->chmgrs.begin(),
        this->chmgrs.end(),
        [](ChannelManagerP chm){ chm->run(); }
    );

    this->startModules();
}

void RuntimeApp::loop(ELROND_LAMBDA_FUNC(bool, void) continueHandle)
{
    std::vector<ModuleHandleP> syncLoopMods;

    std::for_each(
        this->instances.begin(),
        this->instances.end(),
        [&syncLoopMods](ModuleHandleP mh){

            if(!mh->lc.enable) return;

            if(mh->lc.ownThread){
                mh->asyncRun();
                return;
            }

            syncLoopMods.push_back(mh);
            mh->syncLoop();
        }
    );

    while(continueHandle()){
        std::for_each(
            syncLoopMods.begin(),
            syncLoopMods.end(),
            [](ModuleHandleP mh){ mh->syncLoop(); }
        );
    }
}

void RuntimeApp::stop(const bool force)
{
    this->stopModules();

    std::for_each(
        this->chmgrs.begin(),
        this->chmgrs.end(),
        [&force](ChannelManagerP chm){ chm->stop(!force); }
    );

    std::for_each(
        this->instances.begin(),
        this->instances.end(),
        [&force](ModuleHandleP mh){ mh->asyncStop(!force); }
    );
}

ChannelManagerP RuntimeApp::defineChannelManager(
    elrond::String transport,
    const elrond::sizeT tx,
    const elrond::sizeT rx,
    const elrond::sizeT fps
){
    ModuleHandleP mod = this->findModule(transport);
    if(mod->instance->getType() != elrond::ModuleType::TRANSPORT)
        throw Exception("Instance is not a transport module");

    ChannelManagerP chmgr = std::make_shared<ChannelManager>(
        (BaseTransportModule &) *(mod->instance),
        tx,
        rx,
        fps
    );

    chmgr->init();
    this->chmgrs.push_back(chmgr);
    return chmgr;
}

BaseGpioModule& RuntimeApp::getGpioService() const
{
    auto it = std::find_if(
        this->instances.begin(),
        this->instances.end(),
        [](ModuleHandleP mh)
        { return mh->instance->getType() == elrond::ModuleType::GPIO; }
    );

    if(it != this->instances.end())
        return *((BaseGpioModule*) (*it)->instance);

    throw Exception(
        "Invalid gpio service",
        Exception("No gpio service defined")
    );
}

BaseInputDriverModule& RuntimeApp::getInputService(const elrond::sizeT id) const
{
    elrond::sizeT i = 0;
    auto it = std::find_if(
        this->instances.begin(),
        this->instances.end(),
        [&id, &i](ModuleHandleP mod)
        {
            if(mod->instance->getType() != elrond::ModuleType::INPUT) return false;
            return (i++) == id;
        }
    );

    if(it != this->instances.end())
        return (BaseInputDriverModule&) *((*it)->instance);

    throw Exception(
        "Invalid input service",
        Exception("The input service with index " + std::to_string(id) + " not found")
    );
}

const DebugOut& RuntimeApp::dout() const { return (DebugOut&) this->_dout; }
void RuntimeApp::onError(const char* error){ throw Exception(error); }
void RuntimeApp::onError(elrond::String error){ throw Exception(error); }

BaseChannelManager& RuntimeApp::getChannelManager(const elrond::sizeT id) const
{
    if(id >= this->chmgrs.size()){
        throw Exception(
            "Invalid channel manager",
            Exception("The channel manager with index " + std::to_string(id) + " has not defined")
        );
    }

    return *(this->chmgrs[id]);
}

ModuleFactoryP RuntimeApp::findFactory(
    elrond::String& name,
    ModulesFactoriesV& factories,
    Runtime* app
){
    auto it = std::find_if(
        factories.begin(),
        factories.end(),
        [&name](ModuleFactoryP f){ return f->match(name); }
    );

    if(it != factories.end()) return *it;
    throw Exception("No module instance`s factory defined");
}

ModulesFactoriesV RuntimeApp::newModulesFactories()
{
    ModulesFactoriesV factories = {
        std::make_shared<InternalModuleFactory<elrond::Example>>(),
        std::make_shared<InternalModuleFactory<elrond::Loopback>>(),
        std::make_shared<InternalModuleFactory<elrond::InputToChannel>>(),
        std::make_shared<InternalModuleFactory<elrond::ChannelToChannel>>(),
        std::make_shared<InternalModuleFactory<elrond::DigitalLed>>(),
        std::make_shared<InternalModuleFactory<elrond::AnalogLed>>(),
        std::make_shared<InternalModuleFactory<elrond::Servo>>()
    };
    return std::move(factories);
}
