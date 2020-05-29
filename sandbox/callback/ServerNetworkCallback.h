#ifndef GAMENETWORK_SANDBOX_SERVERNETWORKCALLBACK_H_
#define GAMENETWORK_SANDBOX_SERVERNETWORKCALLBACK_H_
#ifdef SERVER
#include "../../network/abstraction/INetworkCallback.h"
#include "../../network/abstraction/Network.h"
// This class creates bridge between network and user.
// User must define a class something like this which derived from `INetworkCallback`.
struct ServerNetworkCallback : INetworkCallback {
  ~ServerNetworkCallback() override = default;

  void OnClientConnected(int t_clientIndex) override {
    std::cout << "[ServerNetworkCallback] Client has connected to server index " << t_clientIndex << std::endl;
    Network::Instance()
        .SendMessage(t_clientIndex, GameMessageType::TEST, GameChannel::UNRELIABLE, [](yojimbo::Message *message) {
          auto testMessage = dynamic_cast<TestMessage *>(message);
          char src[] = "SIYAH";
          strncpy(testMessage->m_text, src, 100);
        });
  }
  void OnClientDisconnected(int t_clientIndex) override {
    std::cout << "[ServerNetworkCallback] Client has disconnected from server index " << t_clientIndex << std::endl;
  }

  void OnMessageReceived(int t_channel, int t_clientIndex, std::shared_ptr<yojimbo::Message> &&t_message) override {
    std::cout << "[ServerNetworkCallback] Server received message from channel: " << t_channel << ", client: "
              << t_clientIndex << ", message_type: " << t_message->GetType() << std::endl;

    if ((GameMessageType) t_message->GetType() == GameMessageType::TEST) {
      auto aMessage = dynamic_cast<TestMessage *>(t_message.get());
      std::cout << "[ServerNetworkCallback] Test message has received from client: " << t_clientIndex << ", id: "
                << aMessage->m_id << ", text: " << aMessage->m_text << std::endl;
      Network::Instance().SendMessage(t_clientIndex,
                                      GameMessageType::TEST,
                                      static_cast<GameChannel>(t_channel),
                                      [aMessage](yojimbo::Message *message) {
                                        auto testMessage = dynamic_cast<TestMessage *>(message);
                                        char src[] = "SIYAH";
                                        testMessage->m_id = aMessage->m_id + 1;
                                        strncpy(testMessage->m_text, src, 100);
                                      });
    }
  }

  void OnServerStarted(bool success) override {
    std::cout << "[ServerNetworkCallback] Server start success " << success << std::endl;
  }
  void OnServerDisconnected() override {
    std::cout << "[ServerNetworkCallback] Server has disconnected" << std::endl;
  }
  void OnServerState() override {
//    std::cout << "[ServerNetworkCallback] On server state" << std::endl;
  }
};
#endif
#endif //GAMENETWORK_SANDBOX_SERVERNETWORKCALLBACK_H_
