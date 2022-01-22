#if !defined  _ELROND_RUNTIME_MODULE_FACTORY_HPP
    #define _ELROND_RUNTIME_MODULE_FACTORY_HPP

    #include "rtTypes.hpp"

    namespace elrond {
        namespace runtime {

            class ModuleFactory {

                protected:

                    elrond::String _name;
                    elrond::runtime::ModuleInfo _info;

                public:

                    elrond::runtime::ModuleInfo const& info;

                    ModuleFactory(elrond::String name);
                    virtual ~ModuleFactory();

                    virtual elrond::interface::Module* newInstance(String const& instName)=0;
                    virtual void deleteInstance(elrond::interface::Module* inst)=0;
                    virtual bool match(elrond::String const& name) const;
            };
        }
    }

#endif
