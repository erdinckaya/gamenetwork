#ifndef GAMENETWORK_SANDBOX_CLIENTNETWORKCALLBACK_H_
#define GAMENETWORK_SANDBOX_CLIENTNETWORKCALLBACK_H_
#ifdef CLIENT
#include <iostream>
#include "../../network/abstraction/INetworkCallback.h"
#include "../../network/abstraction/Network.h"
// This class creates bridge between network and user.
// User must define a class something like this which derived from `INetworkCallback`.
struct ClientNetworkCallback : INetworkCallback {
  ~ClientNetworkCallback() override = default;
  void OnConnect(bool success) override {
    std::cout << "[ClientNetworkCallback] Client connect success " << success << std::endl;
  }
  void OnDisconnect() override {
    std::cout << "[ClientNetworkCallback] Client has disconnected" << std::endl;
  }

  // Another good side of GameNetwork you dont need to care about the life time of message, GameNetwork handles it.
  // Note that it uses shared ptr but there is not guarantee that message will be freed after this function.
  // GameNetwork waits for the end of the network frequency and frees all unused messages.
  void OnMessageReceived(int t_channel, std::shared_ptr<yojimbo::Message> &&t_message) override {
    std::cout << "[ClientNetworkCallback] Client received message from channel " << t_channel << ", message id "
              << t_message->GetType() << std::endl;

    if ((GameMessageType) t_message->GetType() == GameMessageType::TEST) {
      auto aMessage = dynamic_cast<TestMessage *>(t_message.get());
      std::cout << "[ClientNetworkCallback] Test message has received id: " << aMessage->m_id << ", text: "
                << aMessage->m_text << std::endl;
      Network::Instance().SendMessage(GameMessageType::TEST,
                                      static_cast<GameChannel>(t_channel),
                                      [aMessage](yojimbo::Message *message) {
                                        auto testMessage = dynamic_cast<TestMessage *>(message);
                                        char src[] = "BEYAZ!";
                                        testMessage->m_id = aMessage->m_id + 1;
                                        strncpy(testMessage->m_text, src, 100);
                                      });
    }
  }
};
#endif
#endif //GAMENETWORK_SANDBOX_CLIENTNETWORKCALLBACK_H_
