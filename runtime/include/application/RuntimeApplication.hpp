#ifndef ELROND_RUNTIME_APPLICATION_RUNTIME_APPLICATION_HPP
    #define ELROND_RUNTIME_APPLICATION_RUNTIME_APPLICATION_HPP

    #include "application/BaseApplication.hpp"
    #include "application/ApplicationEvents.hpp"

    namespace elrond
    {
        namespace application
        {
            class RuntimeApplication :  public elrond::application::BaseApplication,
                                        public elrond::application::ApplicationEvents
            {
                protected:
                    elrond::application::ModuleFactoryPool* _factories;

                public:
                    ELROND_CLASS_SPECIAL_MEMBERS_CONSTUCTORS(RuntimeApplication, =delete, =delete, =delete, =delete, =delete)
                    virtual ~RuntimeApplication();
                    RuntimeApplication( elrond::application::ModuleFactoryPool& factories,
                                        elrond::interface::ConsoleAdapter& consoleAdapter);

                    void create(const std::string& name, const std::string& factory);

                    virtual void setup();
                    virtual std::future<void> start();
                    virtual void stop(const bool forced = false);
                    void reset();
                    bool isRunning() const;

                protected:
                    static void main(RuntimeApplication* self, std::vector<elrond::application::AsyncLoopTaskP> tasks);
            };
        }
    }

#endif
