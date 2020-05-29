#ifndef GAMENETWORK_NETWORK_ABSTRACTION_NETWORK_H_
#define GAMENETWORK_NETWORK_ABSTRACTION_NETWORK_H_
#include <memory>
#include <iostream>
#include "INetworkBase.h"
#include "../client/GameClient.h"
#include "../server/GameServer.h"
// Singleton Main struct for GameNetwork. User can communicate with network
// by using this struct or callbacks.
struct Network :
#ifdef SERVER
    GameServer
#else
  GameClient
#endif
{

 private:
  static Network &InstanceImpl(NetworkConfig *const t_networkConfig = nullptr) {
    static Network instance{t_networkConfig};
    return instance;
  }

  Network(NetworkConfig *const t_networkConfig) :
#ifdef SERVER
      GameServer(*t_networkConfig)
#else
  GameClient(*t_networkConfig)
#endif
  {
    if (nullptr == t_networkConfig) {
      throw std::runtime_error{"Network not initialized"};
    }
  }

 public:

  static Network &Instance() {
    return InstanceImpl();
  }
  static void Init(NetworkConfig &t_networkConfig) {
    if (!InitializeYojimbo()) {
      std::cerr<<"Cannot init yojimbo!"<<std::endl;
    }
    yojimbo_log_level(t_networkConfig.logLevel);
    InstanceImpl(&t_networkConfig);
  }

  Network(Network const &) = delete;
  void operator=(Network const &) = delete;

  ~Network() override = default;
};
#endif //GAMENETWORK_NETWORK_ABSTRACTION_NETWORK_H_
