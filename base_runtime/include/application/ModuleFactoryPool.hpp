#ifndef ELROND_BASE_RUNTIME_APPLICATION_MODULE_FACTORY_POOL_HPP
    #define ELROND_BASE_RUNTIME_APPLICATION_MODULE_FACTORY_POOL_HPP

    #include "elrond_base_runtime_types.hpp"
    #include "platform/InternalFactoryAdapter.ipp"

    #define ELROND_MK_INTERNAL_MOD_FACTORY(TYPE, ...) \
        std::make_shared<elrond::platform::InternalFactoryAdapter<TYPE>> \
        (elrond::platform::ModuleInfo{ __VA_ARGS__ })

    #define ELROND_DEF_INTERNAL_MOD_FACTORY(NAME, TYPE, ...) \
        std::make_pair(NAME, ELROND_MK_INTERNAL_MOD_FACTORY(TYPE, __VA_ARGS__))


    namespace elrond
    {
        namespace application
        {
            class ModuleFactoryPool
            {
                protected:
    
                    elrond::platform::FactoriesM _factories;

                public:
                    ModuleFactoryPool() = default;
                    ModuleFactoryPool(const elrond::platform::FactoriesM& factories);

                    virtual ~ModuleFactoryPool() = default;

                    elrond::platform::FactoryAdapterP get(const std::string& name) const;

                    ModuleFactoryPool& copyFrom(const ModuleFactoryPool& other);
                    ModuleFactoryPool& copyFrom(const ModuleFactoryPool& other, bool override);
                    ModuleFactoryPool& copyFrom(const elrond::platform::FactoriesM& factories);
                    ModuleFactoryPool& copyFrom(const elrond::platform::FactoriesM& factories, bool override);

                    ModuleFactoryPool& load(const std::string& name, const std::string& path);

                    template <class T>
                    ModuleFactoryPool& define(const std::string& name,
                                              const elrond::platform::ModuleInfo& info);

                    static ModuleFactoryPool createWithCommonInternals();
            };
        }
    }

#endif
