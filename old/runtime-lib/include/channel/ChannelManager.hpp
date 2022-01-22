#if !defined  _ELROND_RUNTIME_CHANNEL_MANAGER_HPP
    #define _ELROND_RUNTIME_CHANNEL_MANAGER_HPP

    #include "rtTypes.hpp"
    #include "channel/RxChCollection.hpp"

    namespace elrond {
        namespace runtime {

            class ChannelManager : public elrond::channel::BaseChannelManager {

                protected:

                    const elrond::sizeT totalTx;
                    const elrond::sizeT totalRx;
                    std::unique_ptr<elrond::runtime::RxChCollectionP[]> rxChannels;

                    std::unique_ptr<elrond::byte[]> txBuffer;
                    std::mutex txBufferMtx;

                    void rxTrigger(const elrond::sizeT ch, elrond::word data) override;
                    elrond::byte* getTxBuffer() const override;

                    const int txFps;
                    bool running = false;
                    std::thread thread;

                    static void entryPoint(ChannelManager* cm);

                public:

                    ChannelManager(
                        elrond::module::BaseTransportModule& transport,
                        const elrond::sizeT totalTx,
                        const elrond::sizeT totalRx,
                        const unsigned int txFps
                    );

                    void addRxListener(
                        const elrond::sizeT ch,
                        elrond::channel::RxChannel* rx
                    ) override;

                    void txTrigger(const elrond::sizeT ch, elrond::word data) override;
                    bool txSync(const bool force) override;

                    elrond::sizeT getTotalTx() const override;
                    elrond::sizeT getTotalRx() const override;

                    void run();
                    void stop(const bool join);
            };
        }
    }

#endif
