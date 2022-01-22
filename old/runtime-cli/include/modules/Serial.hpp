#if !defined  _ELROND_SERIAL_TRANSPORT_STANDALONE_MODULE_HPP
    #define _ELROND_SERIAL_TRANSPORT_STANDALONE_MODULE_HPP

    #include "elrond-runtime.hpp"
    #include <termios.h>

    class Serial : public elrond::module::BaseTransportModule {

        protected:

            struct termios tty;
            int port = -1;
            elrond::channel::BaseChannelManager* cm = nullptr;

            static speed_t getSpeed(unsigned int speed);

        public:

            virtual ~Serial();

            void onInit(elrond::interface::ConfigMap& cfg, elrond::LoopControl& lc) override;
            void onStart() override;
            void onStop() override;
            void loop() override;

            void send(elrond::byte data[], const elrond::sizeT length) override;
            void setChannelManager(elrond::channel::BaseChannelManager* cm) override;

            ELROND_DEFINE_INTER_MOD_DEF_FUNCS;
    };

    namespace elrond {
        namespace runtime {
            using Serial = Serial;
        }
    }

#endif
