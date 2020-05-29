#ifndef GAMENETWORK_NETWORK_ABSTRACTION_ICLIENTNETWORKCALLBACK_H_
#define GAMENETWORK_NETWORK_ABSTRACTION_ICLIENTNETWORKCALLBACK_H_

#include "message/IClientMessageCallback.h"
#include "config/IClientConnectionCallback.h"

struct IClientNetworkCallback : IClientConnectionCallback, IClientMessageCallback {

};
#endif //GAMENETWORK_NETWORK_ABSTRACTION_ICLIENTNETWORKCALLBACK_H_
