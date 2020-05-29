#ifndef GAMENETWORK__ClientMessageAdapter_H_
#define GAMENETWORK__ClientMessageAdapter_H_

#include <yojimbo.h>
#include <deque>
#include <mutex>
#include <memory>
#include <functional>
#include "../../abstraction/INetworkBase.h"

enum class GameChannel;
enum class GameMessageType;

struct ClientMessageAdapter {
  using Message = yojimbo::Message;
  using MessagePtr = std::shared_ptr<Message>;
  using Client = yojimbo::Client;
  using Guard = std::lock_guard<std::mutex>;
  using MessageInitFunc = const std::function<void(Message *)>;

  explicit ClientMessageAdapter(Client &t_client, const INetworkBase& t_network);

  MessagePtr CreateMessage(Message *t_message);

  void SendMessage(GameMessageType t_messageType, GameChannel t_channel, MessageInitFunc &t_callback);

  void ReleaseMessages();

 private:
  mutable std::mutex m_clientMutex{};
  mutable std::mutex m_messagesMutex{};
  std::deque<Message *> m_messages;
  Client &m_client;
  const INetworkBase& m_network;
};

#endif //GAMENETWORK__ClientMessageAdapter_H_
