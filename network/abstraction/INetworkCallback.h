#ifndef GAMENETWORK_NETWORK_ABSTRACTION_INETWORKCALLBACK_H_
#define GAMENETWORK_NETWORK_ABSTRACTION_INETWORKCALLBACK_H_

#ifdef SERVER
#include "../server/IServerNetworkCallback.h"
#else
#include "../client/IClientNetworkCallback.h"
#endif
// Callback abstractions.
struct INetworkCallback :
#ifdef SERVER
    IServerNetworkCallback
#else
  IClientNetworkCallback
#endif
{
  virtual ~INetworkCallback() = default;
};

#endif //GAMENETWORK_NETWORK_ABSTRACTION_INETWORKCALLBACK_H_
