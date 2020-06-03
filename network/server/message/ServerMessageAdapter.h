#ifndef GAMENETWORK__ServerMessageAdapter_H_
#define GAMENETWORK__ServerMessageAdapter_H_

#include <yojimbo.h>
#include <deque>
#include <mutex>
#include <memory>
#include <functional>
#include "../../abstraction/INetworkBase.h"

enum class GameChannel;
enum class GameMessageType;

struct ServerMessageAdapter {
  using Message = yojimbo::Message;
  using MessagePtr = std::shared_ptr<Message>;
  using Server = yojimbo::Server;
  using Guard = std::lock_guard<std::mutex>;
  using MessageInitFunc = const std::function<void(Message *)>;

  explicit ServerMessageAdapter(Server &t_server, const INetworkBase& t_network);
  MessagePtr ReceiveMessage(int t_clientIndex, GameChannel t_channel);
  void SendMessage(int t_clientIndex, GameMessageType t_type, GameChannel t_channel, MessageInitFunc &t_callback);

  void ReleaseMessages();

 private:
  struct MessageData {
    MessageData(int t_clientIndex, Message *t_message) : clientIndex(t_clientIndex), message(t_message) {}
    int clientIndex;
    Message *message;
  };

  mutable std::mutex m_serverMutex{};
  mutable std::mutex m_messagesMutex{};
  std::deque<MessageData> m_messages;
  Server &m_server;
  const INetworkBase& m_network;
};

#endif //GAMENETWORK__ServerMessageAdapter_H_
