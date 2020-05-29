#include "ServerMessageAdapter.h"
// This class handles all message related staffs, which are sending creating and freeing messages.
ServerMessageAdapter::ServerMessageAdapter(Server &t_server, const INetworkBase &t_network)
    : m_server(t_server), m_network(t_network) {}

// This function is used only internally. In order to keep track of the life time of the messages,
// it creates a wrapper class for messages.
ServerMessageAdapter::MessagePtr ServerMessageAdapter::CreateMessage(int t_clientIndex, Message *t_message) {
  if (!m_network.CanSendMessage()) {
    return nullptr;
  }
  Guard guard(m_serverMutex);
  MessagePtr message(t_message, [this, t_clientIndex](Message *t_deletedMessage) {
    Guard guard(m_messagesMutex);
    m_messages.emplace_back(t_clientIndex, t_deletedMessage);
  });
  return message;
}

// Sends message to client.
// Since yojimbo does not handle the synchronization and also life time of messages.
// GameNetwork has to care about it. Another drawback of yojimbo is message factory provides base message
// therefore user needs to initialize them manually.
void ServerMessageAdapter::SendMessage(int t_clientIndex,
                                       GameMessageType t_type,
                                       GameChannel t_channel,
                                       MessageInitFunc &t_callback) {
  if (!m_network.CanSendMessage()) {
    return;
  }
  // Lock message factory.
  Guard guard(m_serverMutex);
  auto message = m_server.CreateMessage(t_clientIndex, static_cast<int>(t_type));
  // Let user init messages.
  // Opening critical object to outside generally is wrong approach but yojimbo message factory does not
  // provide derived objects, that is why messages must have default constructors.
  // This problem can be solved by forwarding message constructor parameters by using perfect forwarding.
  // And creating object in message factory by using parameters.
  try {
    t_callback(message);
  } catch (...) {
    // If anything goes wrong, send message to pool and exit.
    Guard messageGuard(m_messagesMutex);
    m_messages.emplace_back(t_clientIndex, message);
    return;
  }
  // check channel to send availability.
  auto channel = static_cast<int>(t_channel);
  if (m_server.IsClientConnected(t_clientIndex)
      && m_server.CanSendMessage(t_clientIndex, channel)) {
    m_server.SendMessage(t_clientIndex, channel, message);
  } else {
    // If user cannot send message, return message to pool
    Guard messageGuard(m_messagesMutex);
    m_messages.emplace_back(t_clientIndex, message);
  }
}

void ServerMessageAdapter::ReleaseMessages() {
  if (!m_network.CanSendMessage()) {
    return;
  }
  Guard networkGuard(m_serverMutex);
  Guard guard(m_messagesMutex);
  while (!m_messages.empty()) {
    auto messageData = m_messages.front();
    m_server.ReleaseMessage(messageData.clientIndex, messageData.message);
    m_messages.pop_front();
  }
}