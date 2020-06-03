#include <iostream>
#include "ServerBase.h"
ServerBase::ServerBase(Server &t_server, const NetworkConfig &t_networkConfig) :
    m_server(t_server),
    m_privateKey(t_networkConfig.privateKey),
    m_serverAddress(t_networkConfig.serverAddress),
    m_fixedDeltaTime(t_networkConfig.fixedDeltaTime),
    m_numOfChannels(t_networkConfig.connection.numChannels),
    m_maxPlayer(t_networkConfig.maxPlayer),
    m_messageAdapter(t_server, *this),
    m_callback(reinterpret_cast<IServerNetworkCallback &>(t_networkConfig.callback)),
    m_running(true),
    m_time(t_networkConfig.startTime) {
  m_state = NetworkState::Premature;
}
// Starts server.
bool ServerBase::Connect() {
  m_server.Start(m_maxPlayer);
  if (!m_server.IsRunning()) {
    m_state = NetworkState::Dead;
    std::cerr << "[ServerBase] Could not start server at port " << m_serverAddress.GetPort() << std::endl;
    m_callback.OnServerStarted(false);
    return false;
  }

  m_state = NetworkState::Alive;
  m_callback.OnServerStarted(true);
  char addressString[256];
  m_server.GetAddress().ToString(addressString, sizeof(addressString));
  std::cout << "[ServerBase] server address is " << addressString << std::endl;
  return true;
}
// Starts the life of server by given frequency.
// Blocks the thread until it is closed by another thread or error.
void ServerBase::Run() {

  while (m_running) {
    const double currentTime = yojimbo_time();
    if (m_time <= currentTime) {
      m_running = Update(m_fixedDeltaTime);
    } else {
      yojimbo_sleep(m_time - currentTime);
    }
  }
  m_state = NetworkState::VegetativeState;
  std::cout << "[ServerBase] Disconnecting!!" << std::endl;

  m_messageAdapter.ReleaseMessages();
  m_server.DisconnectAllClients();
  m_server.Stop();
  m_state = NetworkState::Dead;
  m_callback.OnServerDisconnected();
}
// Stops the server.
void ServerBase::Stop() {
  m_running = false;
}
// Retrieves the current time of server.
double ServerBase::Time() const {
  return m_time;
}
// Main loop of server.
bool ServerBase::Update(const double t_deltaTime) {
  if (!m_server.IsRunning()) {
    return false;
  }

  // This order is very important.
  // You need to follow this order otherwise
  // You can not establish connection between server and server
  // Order is advance time, receive packet, process messages, send new packages.

  // Increase network time.
  m_server.AdvanceTime(m_time);
  m_time += t_deltaTime;

  // Receive packages.
  m_server.ReceivePackets();

  // Process received messages
  ProcessMessages();

  m_callback.OnServerState();
  // Send new packages.
  m_server.SendPackets();

  return true;
}
void ServerBase::ProcessMessages() {
  for (int i = 0; i < m_maxPlayer; i++) {
    if (m_server.IsClientConnected(i)) {
      for (int j = 0; j < m_numOfChannels; j++) {
        auto message = m_messageAdapter.ReceiveMessage(i, static_cast<GameChannel>(j));
        while (message != nullptr) {
          m_callback.OnMessageReceived(j, i, std::move(message));
          message = m_messageAdapter.ReceiveMessage(i, static_cast<GameChannel>(j));
        }
      }
    }
  }
}
