#ifndef GAMENETWORK_NETWORK_CLIENT_BASE_CLIENTBASE_H_
#define GAMENETWORK_NETWORK_CLIENT_BASE_CLIENTBASE_H_

#include <yojimbo.h>
#include "../../abstraction/INetworkBase.h"
#include "../IClientNetworkCallback.h"
#include "../../NetworkConfig.h"
#include "../message/ClientMessageAdapter.h"
struct ClientBase : INetworkBase {
  using Client = yojimbo::Client;
  using Address = yojimbo::Address;

  explicit ClientBase(Client &t_client, const NetworkConfig &t_networkConfig);
  ~ClientBase() override = default;
  bool Connect() override;
  void Run() override;
  void Stop() override;
  double Time() const override;
 private:
  bool InsecureConnect();
  bool Update(double t_deltaTime);
  void ProcessMessages();

 protected:
  ClientMessageAdapter m_messageAdapter;

 private:
  Client &m_client;
  const uint8_t *const m_privateKey;
  Address m_serverAddress;
  const double m_fixedDeltaTime;
  const int m_numOfChannels;
  IClientNetworkCallback &m_callback;

  std::atomic<bool> m_running;
  double m_time;

};

#endif //GAMENETWORK_NETWORK_CLIENT_BASE_CLIENTBASE_H_
