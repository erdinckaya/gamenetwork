#ifndef GAMENETWORK_NETWORK_SERVER_ABSTRACTION_ISERVERMESSAGEEVENT_H_
#define GAMENETWORK_NETWORK_SERVER_ABSTRACTION_ISERVERMESSAGEEVENT_H_
#include <yojimbo.h>
#include <memory>

struct IServerMessageCallback {
  virtual ~IServerMessageCallback() = default;
  virtual void OnMessageReceived(int t_channel, int t_clientIndex, std::shared_ptr<yojimbo::Message>&& t_message) = 0;
  virtual void OnServerState() = 0;
};

#endif //GAMENETWORK_NETWORK_SERVER_ABSTRACTION_ISERVERMESSAGEEVENT_H_
