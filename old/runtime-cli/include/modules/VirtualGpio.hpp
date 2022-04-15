#if !defined  _ELROND_RUNTIME_VIRTUAL_GPIO_HPP
    #define _ELROND_RUNTIME_VIRTUAL_GPIO_HPP

    #include "elrond-runtime.hpp"

    class VirtualGpio : public elrond::module::BaseGpioModule {

        public:

            virtual ~VirtualGpio();

            void onInit(elrond::interface::ConfigMap& cfg, elrond::LoopControl& lc) override;
            void attach(elrond::gpio::BaseGpioPin& pin) override;

            ELROND_DEFINE_INTER_MOD_DEF_FUNCS;
    };

    namespace elrond {
        namespace runtime {
            using VirtualGpio = VirtualGpio;
        }
    }

#endif
