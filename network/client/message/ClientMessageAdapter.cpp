#include <iostream>
#include "ClientMessageAdapter.h"
// This class handles all message related staffs, which are sending, creating and freeing messages.
ClientMessageAdapter::ClientMessageAdapter(Client &t_client, const INetworkBase &t_network)
    : m_client(t_client), m_network(t_network) {}

// This is used by only internally. It creates a wrapper for message class.
// In order to keep track of the life time of the messages.
ClientMessageAdapter::MessagePtr ClientMessageAdapter::CreateMessage(Message *t_message) {
  // Check state to send message.
  if (!m_network.CanSendMessage()) {
    std::cout << "[ClientMessageAdapter] Client cannot send message! [CreateMessage]" << std::endl;
    return nullptr;
  }
  // Lock the critical section.
  Guard guard(m_clientMutex);
  // Create wrapper.
  MessagePtr message(t_message, [this](Message *t_deletedMessage) {
    Guard guard(m_messagesMutex);
    m_messages.push_back(t_deletedMessage);
  });
  return message;
}

// Sends message to server.
// Since yojimbo does not handle the synchronization and also life time of messages.
// GameNetwork has to care about it. Another drawback is yojimbo message factory provides base message therefore
// User needs to init it.
void ClientMessageAdapter::SendMessage(GameMessageType t_messageType,
                                       GameChannel t_channel,
                                       MessageInitFunc &t_callback) {
  if (!m_network.CanSendMessage()) {
    std::cout << "[ClientMessageAdapter] Client cannot send message! [SendMessage]" << std::endl;
    return;
  }
  // Lock message factory.
  Guard guard(m_clientMutex);
  auto message = m_client.CreateMessage(static_cast<int>(t_messageType));
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
    m_messages.push_back(message);
    return;
  }
  // check channel to send availability.
  auto channel = static_cast<int>(t_channel);
  if (m_client.IsConnected() && m_client.CanSendMessage(channel)) {
    // Send message.
    m_client.SendMessage(channel, message);
  } else {
    // If user cannot send message, return message to pool
    Guard messageGuard(m_messagesMutex);
    m_messages.push_back(message);
  }
}
// This function is closed to used. It releases messages by using message factory.
void ClientMessageAdapter::ReleaseMessages() {
  if (!m_network.IsConnected()) {
    std::cout << "[ClientMessageAdapter] Client is not connected!" << std::endl;
    return;
  }
  Guard networkGuard(m_clientMutex);
  Guard guard(m_messagesMutex);
  while (!m_messages.empty()) {
    m_client.ReleaseMessage(m_messages.front());
    m_messages.pop_front();
  }
}