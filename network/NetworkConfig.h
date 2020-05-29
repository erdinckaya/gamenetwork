#ifndef GAMENETWORK__NETWORKCONFIG_H_
#define GAMENETWORK__NETWORKCONFIG_H_
#include <yojimbo.h>
#include "abstraction/INetworkCallback.h"

struct NetworkConfig {

  explicit NetworkConfig(const yojimbo::Address &t_address,
                         const yojimbo::Address &t_serverAddress,
                         const yojimbo::ClientServerConfig &t_config,
                         const uint8_t *const t_privateKey,
                         const double t_fixedDeltaTime,
                         const double t_startTime,
                         const int t_maxPlayer,
                         const int t_logLevel,
                         INetworkCallback &t_callback)
      : address(t_address),
        serverAddress(t_serverAddress),
        connection(t_config),
        privateKey(t_privateKey),
        fixedDeltaTime(t_fixedDeltaTime),
        startTime(t_startTime),
        maxPlayer(t_maxPlayer),
        logLevel(t_logLevel),
        callback(t_callback) {}

  yojimbo::Address address;
  yojimbo::Address serverAddress;
  yojimbo::ClientServerConfig connection;
  const uint8_t *const privateKey;
  double fixedDeltaTime;
  double startTime;
  int maxPlayer;
  int logLevel;
  INetworkCallback &callback;
};

#endif //GAMENETWORK__NETWORKCONFIG_H_
