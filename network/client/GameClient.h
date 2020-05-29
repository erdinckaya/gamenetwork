#ifndef GAMENETWORK__GAMECLIENT_H_
#define GAMENETWORK__GAMECLIENT_H_


#include <yojimbo.h>
#include "config/ClientConfigAdapter.h"
#include "../NetworkConfig.h"
#include "message/ClientMessageAdapter.h"
#include "base/ClientBase.h"
#include "message/IClientMessageSender.h"
#include "debug/IClientNetworkDebugger.h"

struct GameClient : ClientBase, IClientMessageSender, IClientNetworkDebugger {
 public:
  explicit GameClient(const NetworkConfig& t_networkConfig);

  GameClient(const GameClient &) = delete;
  GameClient& operator=(const GameClient &) = delete;
  ~GameClient() override = default;

  void SendMessage(GameMessageType t_messageType, GameChannel t_channel, MessageInitFunc &t_callback) override;
  void SetLatency(float t_milliseconds) override;
  void SetJitter(float t_milliseconds) override;
  void SetPacketLoss(float t_percent) override;
  void SetDuplicates(float t_percent) override;
  void GetNetworkInfo(yojimbo::NetworkInfo &t_networkInfo) const override;

 private:
  ClientConfigAdapter m_adapter;
  yojimbo::Client m_client;
};

#endif //GAMENETWORK__GAMECLIENT_H_
