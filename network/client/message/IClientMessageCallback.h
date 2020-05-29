#ifndef GAMENETWORK_NETWORK_CLIENT_ABSTRACTION_ICLIENTMESSAGEEVENT_H_
#define GAMENETWORK_NETWORK_CLIENT_ABSTRACTION_ICLIENTMESSAGEEVENT_H_
#include <yojimbo.h>
#include <memory>
struct IClientMessageCallback {
  virtual ~IClientMessageCallback() = default;
  virtual void OnMessageReceived(int t_channel, std::shared_ptr<yojimbo::Message>&& t_message) = 0;
};

#endif //GAMENETWORK_NETWORK_CLIENT_ABSTRACTION_ICLIENTMESSAGEEVENT_H_
