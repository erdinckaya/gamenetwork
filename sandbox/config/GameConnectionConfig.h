#ifndef GAMENETWORK__GAMECONNECTIONCONFIG_H_
#define GAMENETWORK__GAMECONNECTIONCONFIG_H_

#include <yojimbo.h>

// Basic configurations.
namespace GameConnection {
constexpr uint64_t PROTOCOL_ID = 0x11223344556677UL;
constexpr uint16_t SERVER_PORT = 40000;
constexpr const char *SERVER_ADDRESS = "127.0.0.1";
constexpr const char *CLIENT_ADDRESS = "0.0.0.0";

constexpr uint8_t DEFAULT_PRIVATE_KEY[yojimbo::KeyBytes] = {
    0x60, 0x6a, 0xbe, 0x6e, 0xc9, 0x19, 0x10, 0xea,
    0x9a, 0x65, 0x62, 0xf6, 0x6f, 0x2b, 0x30, 0xe4,
    0x43, 0x71, 0xd6, 0x2c, 0xd1, 0x99, 0x27, 0x26,
    0x6b, 0x3c, 0x60, 0xf4, 0xb7, 0x15, 0xab, 0xa1
};
}

// Channel types. We have two types of channels
// first one is reliable udp, which guarantees that this message will
// be received.
// Unreliable is simple udp and does not guarantees that message will be received.
enum class GameChannel
{
  RELIABLE,
  UNRELIABLE,
  COUNT
};

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

#endif //GAMENETWORK__GAMECONNECTIONCONFIG_H_
