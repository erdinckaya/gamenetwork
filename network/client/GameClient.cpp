#include "GameClient.h"

GameClient::GameClient(const NetworkConfig &t_networkConfig)
    : m_adapter(),
      m_client(yojimbo::GetDefaultAllocator(),
               t_networkConfig.address,
               t_networkConfig.connection,
               m_adapter,
               t_networkConfig.startTime),
      ClientBase(m_client, t_networkConfig) {
}
void GameClient::SendMessage(GameMessageType t_messageType,
                             GameChannel t_channel,
                             IClientMessageSender::MessageInitFunc &t_callback) {
  m_messageAdapter.SendMessage(t_messageType, t_channel, t_callback);
}
void GameClient::SetLatency(float t_milliseconds) {
  m_client.SetLatency(t_milliseconds);
}
void GameClient::SetJitter(float t_milliseconds) {
  m_client.SetJitter(t_milliseconds);
}
void GameClient::SetPacketLoss(float t_percent) {
  m_client.SetPacketLoss(t_percent);
}
void GameClient::SetDuplicates(float t_percent) {
  m_client.SetDuplicates(t_percent);
}
void GameClient::GetNetworkInfo(yojimbo::NetworkInfo &t_networkInfo) const {
  m_client.GetNetworkInfo(t_networkInfo);
}
