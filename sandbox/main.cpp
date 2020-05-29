#include <yojimbo.h>
#include "config/GameConnectionConfig.h"
#include "callback/ServerNetworkCallback.h"
#include "callback/ClientNetworkCallback.h"

int main() {
  // GameNetwork is easy to use and lightweight networking library which uses UDP and RUDP connections.
  // GameNetwork uses yojimbo for connections and connection reliability.
  // You can find further information here. https://github.com/erdinckaya/yojimbo
  // Only thing you need is making configurations and running the library.
  // One of the powerful feature of GameNetwork is you can write your client and server code.
  // In same project. It has two mods first one is client second one is server.
  // In order to create client build you have to define the macro CLIENT=1 and for server SERVER=1.

  // Define max player and our network frequency.
  const int MaxPlayers = 2;
  const double FixedDeltaTime = 1.0 / 60.0;
  // Start time is default 0. Note that client and server must start with same initial
  // time. Thanks to GameNetwork which provides client and server code in same project. You dont need to duplicate
  // code again and again.
  const double StartTime = 0.0;

  // This is yojimbo log level, it inits yojimbo logger, for now it is basic printf.
  // I will add log callback in later versions.
  const int DebugLevel = YOJIMBO_LOG_LEVEL_INFO;

  // Define our callbacks and network configurations you can find basic configurations below.
  // For now client is connecting server via insecure, namely basic connection. I will add a matchmaking
  // plugin in later versions.
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

  // Init connect and run.
  Network::Init(network_config);
  Network::Instance().Connect();
  Network::Instance().Run();
  // After this point you dont need to handle complex network algorithms
  // You can communicate via callbacks.
  // GameNetwork checks messages with given frequency and sends all messages at the end of the network frequency.
  // Further details please look at `ClientBase` or `ServerBase`.
  // Note that callbacks are belong to network thread so I suggest to
  // use thread safe queue to move data in game thread.
  return 0;
}
