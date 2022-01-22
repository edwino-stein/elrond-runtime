#if !defined  _ELROND_RUNTIME_UDP_MODULE_HPP
    #define _ELROND_RUNTIME_UDP_MODULE_HPP

    #include "elrond-runtime.hpp"
    #include <arpa/inet.h>

    class Udp : public elrond::module::BaseTransportModule {

        protected:

            class UdpSocket {

                protected:

                    elrond::String _host = "";
                    int _port = -1;

                    struct sockaddr_in serverAddr;
                    struct sockaddr_in clientAddr;
                    int sck = -1;

                public:

                    elrond::String const& host;
                    int const& port;

                    UdpSocket();

                    void init(const int port, elrond::String host);

                    void stop();

                    bool receive(elrond::byte data[], const elrond::sizeT len);
                    bool send(elrond::byte data[], const elrond::sizeT len);

                    bool isServer() const;
                    bool isRunnig() const;

                    static bool isValidIpv4(elrond::String ip);
                    static bool isValidPort(const int port);
            };

            elrond::channel::BaseChannelManager* cm = nullptr;
            UdpSocket socket;

        public:

            virtual ~Udp();

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
            using Udp = Udp;
        }
    }

#endif
