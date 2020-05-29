#ifndef GAMENETWORK_NETWORK_SERVER_BASE_SERVERBASE_H_
#define GAMENETWORK_NETWORK_SERVER_BASE_SERVERBASE_H_

#include <yojimbo.h>
#include "../../abstraction/INetworkBase.h"
#include "../IServerNetworkCallback.h"
#include "../../NetworkConfig.h"
#include "../message/ServerMessageAdapter.h"
struct ServerBase : INetworkBase {
  using Server = yojimbo::Server;
  using Address = yojimbo::Address;

  explicit ServerBase(Server &t_server, const NetworkConfig &t_networkConfig);
  ~ServerBase() override = default;
  bool Connect() override;
  void Run() override;
  void Stop() override;
  double Time() const override;
 private:
  bool Update(double t_deltaTime);
  void ProcessMessages();

 protected:
  ServerMessageAdapter m_messageAdapter;

 private:
  Server &m_server;
  const uint8_t *const m_privateKey;
  Address m_serverAddress;
  const double m_fixedDeltaTime;
  const int m_numOfChannels;
  IServerNetworkCallback &m_callback;

  std::atomic<bool> m_running;
  double m_time;
  const int m_maxPlayer;

};

#endif //GAMENETWORK_NETWORK_SERVER_BASE_SERVERBASE_H_
