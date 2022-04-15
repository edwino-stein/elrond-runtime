#if !defined  _ELROND_RUNTIME_INTERNAL_MODULE_FACTORY_IPP
    #define _ELROND_RUNTIME_INTERNAL_MODULE_FACTORY_IPP

    #include "InternalModuleFactory.hpp"

    namespace elrond {
        namespace runtime {

            template<class T>
            InternalModuleFactory<T>::InternalModuleFactory():
            ModuleFactory(T::ELROND_MOD_MAIN_CLASS_FUNC_N())
            {
                this->_info.mainClass = T::ELROND_MOD_MAIN_CLASS_FUNC_N();
                this->_info.apiVer = T::ELROND_MOD_API_VER_FUNC_N();
                this->_info.prettyName = T::ELROND_MOD_PRETTY_NAME_FUNC_N();
                this->_info.authorName = T::ELROND_MOD_AUTHOR_NAME_FUNC_N();
                this->_info.authorEmail = T::ELROND_MOD_AUTHOR_EMAIL_FUNC_N();
                this->_info.version = T::ELROND_MOD_VERSION_FUNC_N();
            }

            template<class T>
            InternalModuleFactory<T>::~InternalModuleFactory(){}

            template<class T>
            elrond::interface::Module* InternalModuleFactory<T>::newInstance(elrond::String const& instName)
            { return new T(); }

            template<class T>
            void InternalModuleFactory<T>::deleteInstance(elrond::interface::Module* mod)
            { delete mod; }
        }
    }


#endif
