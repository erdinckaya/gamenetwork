#ifndef GAMENETWORK_NETWORK_SERVER_MESSAGE_ISERVERMESSAGESENDER_H_
#define GAMENETWORK_NETWORK_SERVER_MESSAGE_ISERVERMESSAGESENDER_H_

#include <yojimbo.h>
enum class GameChannel;
enum class GameMessageType;

struct IServerMessageSender {
  using Message = yojimbo::Message;
  using MessagePtr = std::shared_ptr<Message>;
  using MessageInitFunc = const std::function<void(Message *)>;
  virtual void SendMessage(int t_clientIndex,
                           GameMessageType t_messageType,
                           GameChannel t_channel,
                           MessageInitFunc &t_callback) = 0;
};
#endif //GAMENETWORK_NETWORK_SERVER_MESSAGE_ISERVERMESSAGESENDER_H_
