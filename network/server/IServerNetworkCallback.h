#ifndef GAMENETWORK_NETWORK_ABSTRACTION_ISERVERNETWORKCALLBACK_H_
#define GAMENETWORK_NETWORK_ABSTRACTION_ISERVERNETWORKCALLBACK_H_

#include "message/IServerMessageCallback.h"
#include "config/IServerConnectionCallback.h"

struct IServerNetworkCallback : IServerConnectionCallback, IServerMessageCallback {

};
#endif //GAMENETWORK_NETWORK_ABSTRACTION_ISERVERNETWORKCALLBACK_H_
