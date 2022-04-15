#if !defined  _ELROND_RUNTIME_DL_MODULE_FACTORY_STANDALONE_HPP
    #define _ELROND_RUNTIME_DL_MODULE_FACTORY_STANDALONE_HPP

    #include "elrond-runtime.hpp"

    class DlModuleFactory : public elrond::runtime::ModuleFactory {

        protected:

            using ModSetAppT = void (*)(ELROND_MOD_INFO_APP_P);
            using ModNewT = ELROND_MOD_INFO_MOD_P (*)();
            using ModDeleteT = void (*)(ELROND_MOD_INFO_MOD_P);
            using ModStringHandleT = ELROND_MOD_INFO_STR_T (*)();
            using ModNumHandleT = ELROND_MOD_INFO_NUM_T (*)();

            void* dlHandle = nullptr;
            ModNewT _newInstance = nullptr;
            ModDeleteT _delInstance = nullptr;

        public:

            DlModuleFactory(elrond::String path, elrond::interface::Runtime& app);
            virtual ~DlModuleFactory();

            elrond::interface::Module* newInstance(elrond::String const& instName) override;
            void deleteInstance(elrond::interface::Module* inst) override;
    };

#endif
