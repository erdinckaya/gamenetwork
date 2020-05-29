#include "GameServer.h"

GameServer::GameServer(const NetworkConfig &t_networkConfig)
    : m_adapter(reinterpret_cast<IServerConnectionCallback &>(t_networkConfig.callback)),
      m_server(yojimbo::GetDefaultAllocator(),
               t_networkConfig.privateKey,
               t_networkConfig.address,
               t_networkConfig.connection,
               m_adapter,
               t_networkConfig.startTime),
      ServerBase(m_server, t_networkConfig) {
}
void GameServer::SendMessage(int t_clientIndex,
                             GameMessageType t_messageType,
                             GameChannel t_channel,
                             IServerMessageSender::MessageInitFunc &t_callback) {
  m_messageAdapter.SendMessage(t_clientIndex, t_messageType, t_channel, t_callback);
}
void GameServer::SetLatency(float t_milliseconds) {
  m_server.SetLatency(t_milliseconds);
}
void GameServer::SetJitter(float t_milliseconds) {
  m_server.SetJitter(t_milliseconds);
}
void GameServer::SetPacketLoss(float t_percent) {
  m_server.SetPacketLoss(t_percent);
}
void GameServer::SetDuplicates(float t_percent) {
  m_server.SetDuplicates(t_percent);
}
void GameServer::GetNetworkInfo(int t_clientIndex, yojimbo::NetworkInfo &t_networkInfo) const {
  m_server.GetNetworkInfo(t_clientIndex, t_networkInfo);
}

