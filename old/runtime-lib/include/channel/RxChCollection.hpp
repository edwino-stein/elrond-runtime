#if !defined  _ELROND_RUNTIME_RX_CH_COLLECTION_HPP
    #define _ELROND_RUNTIME_RX_CH_COLLECTION_HPP

    #include "rtTypes.hpp"

    namespace elrond {
        namespace runtime {

            class RxChCollection {
                protected:
                    std::vector<elrond::channel::RxChannel*> channels;

                public:
                    void push(elrond::channel::RxChannel* rx);
                    void trigger(const elrond::word data);
            };
        }
    }

#endif
