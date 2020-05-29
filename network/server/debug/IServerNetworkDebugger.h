#ifndef GAMENETWORK_NETWORK_SERVER_DEBUG_ISERVERNETWORKDEBUGGER_H_
#define GAMENETWORK_NETWORK_SERVER_DEBUG_ISERVERNETWORKDEBUGGER_H_
#include <yojimbo.h>
#include "../../abstraction/INetworkDebuggerBase.h"
struct IServerNetworkDebugger : INetworkDebuggerBase {
  virtual void GetNetworkInfo(int t_clientIndex, yojimbo::NetworkInfo& t_networkInfo) const = 0;
};
#endif //GAMENETWORK_NETWORK_SERVER_DEBUG_ISERVERNETWORKDEBUGGER_H_
