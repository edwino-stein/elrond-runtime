#ifndef ELROND_RUNTIME_APPLICATION_MODULE_FACTORY_POOL_HPP
    #define ELROND_RUNTIME_APPLICATION_MODULE_FACTORY_POOL_HPP

    #include "elrond_runtime_types.hpp"

    #define ELROND_MK_INTERNAL_MOD_FACTORY(TYPE, ...) \
        std::make_shared<elrond::application::InternalFactoryAdapter<TYPE>> \
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
    
                    elrond::application::FactoriesM _factories;
                    ModuleFactoryPool* _next;

                    void add(const std::string& name, elrond::application::FactoryP factory);

                public:
                    ModuleFactoryPool();
                    ModuleFactoryPool(ModuleFactoryPool& next);

                    ModuleFactoryPool(const elrond::application::FactoriesM& factories);
                    ModuleFactoryPool(const elrond::application::FactoriesM& factories, ModuleFactoryPool& next);

                    virtual ~ModuleFactoryPool() = default;

                    elrond::application::FactoryP get(const std::string& name) const;
                    bool exists(const std::string& name) const;
                    elrond::sizeT size() const;

                    elrond::application::FactoryP load(const std::string& name, const std::string& path);

                    template <class T>
                    elrond::application::FactoryP define(const std::string& name,
                                                         const elrond::platform::ModuleInfo& info);

                    static ModuleFactoryPool& internals();
            };
        }
    }

#endif
