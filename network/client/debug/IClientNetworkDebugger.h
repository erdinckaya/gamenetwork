#ifndef GAMENETWORK_NETWORK_CLIENT_DEBUG_ICLIENTNETWORKDEBUGGER_H_
#define GAMENETWORK_NETWORK_CLIENT_DEBUG_ICLIENTNETWORKDEBUGGER_H_
#include <yojimbo.h>
#include "../../abstraction/INetworkDebuggerBase.h"
struct IClientNetworkDebugger : INetworkDebuggerBase {
  virtual void GetNetworkInfo(yojimbo::NetworkInfo& t_networkInfo) const = 0;
};
#endif //GAMENETWORK_NETWORK_CLIENT_DEBUG_ICLIENTNETWORKDEBUGGER_H_
