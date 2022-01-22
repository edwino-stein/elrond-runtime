#if !defined  _ELROND_RUNTIME_MODULE_HANDLE_HPP
    #define _ELROND_RUNTIME_MODULE_HANDLE_HPP

    #include "rtTypes.hpp"

    namespace elrond {
        namespace runtime {

            class ModuleHandle {

                protected:

                    elrond::String _name;
                    ModuleFactoryP _factory;
                    elrond::interface::Module* _instance = nullptr;

                    bool running = false;
                    std::thread thread;
                    unsigned long timout = 0;

                    static void entryPoint(ModuleHandle* const mh);

                public:

                    elrond::String const& name;
                    elrond::interface::Module* const& instance;
                    ModuleFactoryP const& factory;
                    bool started = false;
                    elrond::LoopControl lc;

                    ModuleHandle(String name, elrond::runtime::ModuleFactoryP factory);
                    virtual ~ModuleHandle();

                    void asyncRun();
                    void asyncStop(const bool join = true);
                    void syncLoop();
            };
        }
    }

#endif
