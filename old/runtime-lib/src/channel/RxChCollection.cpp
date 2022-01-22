#include "channel/RxChCollection.hpp"

using elrond::runtime::RxChCollection;
using elrond::channel::RxChannel;

void RxChCollection::push(RxChannel* rx){ this->channels.push_back(rx); }

void RxChCollection::trigger(const elrond::word data)
{
    std::for_each(
        this->channels.begin(),
        this->channels.end(),
        [&data](RxChannel *rx){ rx->trigger(data); }
    );
}
