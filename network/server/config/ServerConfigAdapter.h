#ifndef GAMENETWORK_NETWORK_ADAPTER_SERVERADAPTER_H_
#define GAMENETWORK_NETWORK_ADAPTER_SERVERADAPTER_H_

#include <yojimbo.h>
#include "../../../sandbox/factory/GameMessageFactory.h"
#include "IServerConnectionCallback.h"

class ServerConfigAdapter : public yojimbo::Adapter {
 public:
  explicit ServerConfigAdapter(IServerConnectionCallback &t_callback) : m_callback(t_callback) {}

  yojimbo::MessageFactory *CreateMessageFactory(yojimbo::Allocator &t_allocator) override {
    return YOJIMBO_NEW(t_allocator, GameMessageFactory, t_allocator);
  }

  void OnServerClientConnected(int t_clientIndex) override ;
  void OnServerClientDisconnected(int t_clientIndex) override;

 private:
  IServerConnectionCallback &m_callback;
};

#endif //GAMENETWORK_NETWORK_ADAPTER_SERVERADAPTER_H_
