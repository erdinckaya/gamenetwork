# GameNetwork

GameNetwork is a dedicated cross platform game networking project to help game developer or simulation developers.
Currently GameNetwork is tested in 64bits machines, in order to execute this project in 32bit you have rebuild bunch of libraries.
GameNetwork uses yojimbo for establishing connections in UDP and RUDP, also yojimbo has
very fast message serialization and deserialization mechanics which brings more performance as well. One of the powerful features of
GameNetwork project you can write your client and server code in same project, by using SERVER and CLIENT macroes to distinguish them.
Since GameNetwork handles most of the tricky parts of game networking you only need to deal with your game play code. You dont have to
take care about sending or acknowledging messages via UDP. There are some example codes which you can find under the `sandbox` folder.
This project has been tested successfully in `Macos Majove`, `Ubuntu 18.04` and `Windows 10`.
You can find detailed explanations in source code.
Note that this version does not provide secure connection for Clients, it will be added in later versions as a match making plugin.


## Building
GameNetwork uses `CMake` to build its source files. In order to build we need `yojimbo_cmake` you can find the source code [here](https://github.com/erdinckaya/yojimbo_cmake) .
Yojimbo is depending two main libraries which are `libsodium` and `mbedtls` for encription.
Since this project is using `vcpkg` as a package manager all you need to do is checkout `vcpkg` from github and follow the instructions. It should take your 5 mins with a
mediocre internet connection. You can find the `vcpkg` link [here](https://github.com/microsoft/vcpkg). Once you are installed `vcpkg`, we have to define vcpkg
as a tool chain file which can be done like this.
```
-DCMAKE_TOOLCHAIN_FILE=[vcpkg root]/scripts/buildsystems/vcpkg.cmake ..
```
Also we have to give `vcpkg root` path into our `CMakeLists.txt`, since our cmake file is taking care of installing packages.
```
-DVCPKG_ROOT=[vcpkg root]
```

On the whole our final build commands like this


In Unix
```
git clone --recursive https://github.com/erdinckaya/gamenetwork && cd gamenetwork
mkdir build && cd build
# For Server
cmake -DCMAKE_TOOLCHAIN_FILE=[vcpkg root]/scripts/buildsystems/vcpkg.cmake -DVCPKG_ROOT=[vcpkg root] -DSERVER=1 ..
# For Client
cmake -DCMAKE_TOOLCHAIN_FILE=[vcpkg root]/scripts/buildsystems/vcpkg.cmake -DVCPKG_ROOT=[vcpkg root] -DCLIENT=1 ..
make
./GameNetwork
```
In windows
```
git clone https://github.com/erdinckaya/gamenetwork
cd gamenetwork
mkdir build
cd build
# For Server
cmake -DCMAKE_TOOLCHAIN_FILE=[vcpkg root]/scripts/buildsystems/vcpkg.cmake -DVCPKG_ROOT=[vcpkg root] -DSERVER=1 ..
# For Client
cmake -DCMAKE_TOOLCHAIN_FILE=[vcpkg root]/scripts/buildsystems/vcpkg.cmake -DVCPKG_ROOT=[vcpkg root] -DCLIENT=1 ..
cmake --build . --target GameNetwork
.\Debug\GameNetwork.exe
```
It is gonna download and install all packages according to your operating system.

## Usage
You can find the detailed example in `sandbox/main.cpp`.

Lets deep dive into project and usage.

First of all we have to give some configurations into GameNetwork projects. We can do that by filling up `NetworkConfig` struct.
Lets see the example.

```
#ifdef SERVER
  ServerNetworkCallback callback;
  auto address = yojimbo::Address(GameConnection::SERVER_ADDRESS, GameConnection::SERVER_PORT);
#else
  ClientNetworkCallback callback;
  auto address = yojimbo::Address(GameConnection::CLIENT_ADDRESS);
#endif

  // Prepare configurations such as addresses, private keys, and callbacks.
  NetworkConfig network_config(address,
                               yojimbo::Address(GameConnection::SERVER_ADDRESS, GameConnection::SERVER_PORT),
                               GameConnectionConfig(),
                               GameConnection::DEFAULT_PRIVATE_KEY,
                               FixedDeltaTime,
                               StartTime,
                               MaxPlayers,
                               DebugLevel,
                               callback);
```

In order to use this configuration we have to define some classes and some variables. Please find the parameter details of NetworkConfig in oder below.
```
* Address of server or client
* Address of server
* Network config
* Frequency of network loop.
* Start time // default 0
* Maximum count of player that can join this server.
* Debug level flag for yojimbo.
* Callback class for GameNetwork.
```

There are 4 types of class that we must define to use it.
```
1 - Game Connection Config
2 - Game Message Factory
3 - Network Message
4 - Callback classes for server or client.
```
### Game ConnectionConfig
 GameConnectionConfig class defines the channels and protocolId for yojimbo. It must be derived from `yojimbo::ClientServerConfig` class.
 It is very simple and tiny class.
```
// the client and server config
struct GameConnectionConfig : public yojimbo::ClientServerConfig
{
  GameConnectionConfig()
  {
    protocolId = GameConnection::PROTOCOL_ID;
    numChannels = 2;
    channel[static_cast<int>(GameChannel::RELIABLE)].type = yojimbo::CHANNEL_TYPE_RELIABLE_ORDERED;
    channel[static_cast<int>(GameChannel::UNRELIABLE)].type = yojimbo::CHANNEL_TYPE_UNRELIABLE_UNORDERED;
  }
};
```

### Game Message Factory
This class defines your messages into yojimbo's message factory. It is suggested to use like this. I wanted to use enums to distinguish message types,
but it is not obligatory you can use integers.
```
// In order to use messages you have to define by using yojimbo macroes.
// They are basic macroes that overrides base message factory.
YOJIMBO_MESSAGE_FACTORY_START(GameMessageFactory, static_cast<int>(GameMessageType::COUNT))
    YOJIMBO_DECLARE_MESSAGE_TYPE((int) GameMessageType::TEST, TestMessage)
YOJIMBO_MESSAGE_FACTORY_FINISH()
```

### Network Message
They are simple data classes. There are two rules about network message classes. First one is your message class must be derived
from `yojimbo::Message` class, second one is it must have default constructor. Since yojimbo message factory does not supporting perfect forwarding
yet, we cannot create our messages without default constructors. This restriction might bring other problems as well. Therefore creating messages
with other constructors will be added by modifying yojimbo code in later versions.
Lets see example of Network Message
```
// Test message.
// All messages must derived from `yojimbo::Message` and also
// All messages must implement stream member function.
// Note that this class must have default constructor.
class TestMessage : public yojimbo::Message
{
 public:
  int m_id{0};
  char m_text[100]{};


  // This function is used for serialization and deserialization.
  // You can give limits your messages. Messages are serialized by bit by bit
  // so that it creates very tiny data to send. It is better than json.
  // Moreover you dont need to duplicate your message definition for client and server
  // thanks to GameNetwork design.
  // Only drawback of this serialization you have to know your string max limit.
  template <typename Stream>
  bool Serialize(Stream& stream)
  {
    yojimbo_serialize_int(stream, m_id, 0, std::numeric_limits<int>::max());
    yojimbo_serialize_string(stream, m_text, 512);
    return true;
  }

  YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS()
};
```

### Callback classes for server or client.
In order to get some events from GameNetwork you have to define callback classes. These classes must be derived from `INetworkCallback`


Lets see the client callback example in `sandbox/callback/ClientNetworkCallback.h`
You can find an example how to send and receive message in `OnMessageReceived` function.


CLIENT
```
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
  // Note that it uses shared ptr but there is no guarantee that message will be freed after this function.
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
```


`OnConnect` Fired when user connected to server


`OnDisconnect` Fired when user disconnected from server


`OnMessageReceived` Fired when user received message from server. Note that you dont need to deal with message life time and synchronization of
messages. GameNetwork handles all of it for you.



SERVER
```
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
```


`OnClientConnected` Fired when client is connected to server


`OnClientDisconnected` Fired when client is disconnected from server


`OnMessageReceived` Fired when server received message from client.


`OnServerStarted` Fired when server started successfully.


`OnServerDisconnected` Fired when server disconnected successfully.


`OnServerState` This will be called by GameNetwork itself. If you want to send state of the server or game you must fill up this function to
send the state of the game.


## Debugging
GameNetwork provides a handy debugging tools to debug your network. You can find the details in `GameClient` or `GameServer`
You can play with `latency`, `jitter`, `package loss` and `duplicate packages`. Also you can get network info by using `GetNetworkInfo` function.
```
void SetLatency(float t_milliseconds) override;
void SetJitter(float t_milliseconds) override;
void SetPacketLoss(float t_percent) override;
void SetDuplicates(float t_percent) override;
void GetNetworkInfo(yojimbo::NetworkInfo &t_networkInfo) const override;
// for Server
// fetches the clients network info.
void GetNetworkInfo(int t_clientIndex, yojimbo::NetworkInfo &t_networkInfo) const override;
```

