#ifndef GAMENETWORK__GAMESERVER_H_
#define GAMENETWORK__GAMESERVER_H_
#include <yojimbo.h>
#include "config/ServerConfigAdapter.h"
#include "../NetworkConfig.h"
#include "base/ServerBase.h"
#include "message/IServerMessageSender.h"
#include "debug/IServerNetworkDebugger.h"

struct GameServer : ServerBase, IServerMessageSender, IServerNetworkDebugger {
 public:
  explicit GameServer(const NetworkConfig& t_networkConfig);

  GameServer(const GameServer &) = delete;
  GameServer& operator=(const GameServer &) = delete;
  ~GameServer() override = default;

  void SendMessage(int t_clientIndex,
                   GameMessageType t_messageType,
                   GameChannel t_channel,
                   MessageInitFunc &t_callback) override;

  void SetLatency(float t_milliseconds) override;
  void SetJitter(float t_milliseconds) override;
  void SetPacketLoss(float t_percent) override;
  void SetDuplicates(float t_percent) override;
  void GetNetworkInfo(int t_clientIndex, yojimbo::NetworkInfo &t_networkInfo) const override;

 private:
  ServerConfigAdapter m_adapter;
  yojimbo::Server m_server;
};
#endif //GAMENETWORK__GAMESERVER_H_
