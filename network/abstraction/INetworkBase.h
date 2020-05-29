#ifndef GAMENETWORK_NETWORK_CLIENT_BASE_INETWORKBASE_H_
#define GAMENETWORK_NETWORK_CLIENT_BASE_INETWORKBASE_H_
#include "NetworkState.h"
#include <atomic>
// Network abstraction.
struct INetworkBase {
  virtual ~INetworkBase()  {
    ShutdownYojimbo();
  };
  virtual bool Connect() = 0;
  virtual void Run() = 0;
  virtual void Stop() = 0;
  virtual double Time() const = 0 ;

  bool IsConnected() const {return m_state > NetworkState::Premature && m_state < NetworkState::Dead;}
  bool CanSendMessage() const {return m_state == NetworkState::Alive;}
  const NetworkState GetNetworkState() const {return m_state;}
 protected:
  std::atomic<NetworkState> m_state;
};
#endif //GAMENETWORK_NETWORK_CLIENT_BASE_INETWORKBASE_H_
