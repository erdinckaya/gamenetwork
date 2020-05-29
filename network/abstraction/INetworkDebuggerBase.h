#ifndef GAMENETWORK_NETWORK_ABSTRACTION_INETWORKDEBUGGERBASE_H_
#define GAMENETWORK_NETWORK_ABSTRACTION_INETWORKDEBUGGERBASE_H_
#include "NetworkDebug.h"
// GameNetwork provides you debugging as well.
// You can change the network situations very easily.
struct INetworkDebuggerBase {
  virtual void SetLatency(float t_milliseconds) = 0;

  virtual void SetJitter(float t_milliseconds) = 0;

  virtual void SetPacketLoss(float t_percent) = 0;

  virtual void SetDuplicates(float t_percent) = 0;
};
#endif //GAMENETWORK_NETWORK_ABSTRACTION_INETWORKDEBUGGERBASE_H_
