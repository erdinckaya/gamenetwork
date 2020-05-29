#include <iostream>
#include "ClientBase.h"
ClientBase::ClientBase(Client &t_client, const NetworkConfig &t_networkConfig) :
    m_client(t_client),
    m_privateKey(t_networkConfig.privateKey),
    m_serverAddress(t_networkConfig.serverAddress),
    m_fixedDeltaTime(t_networkConfig.fixedDeltaTime),
    m_numOfChannels(t_networkConfig.connection.numChannels),
    m_messageAdapter(t_client, *this),
    m_callback(reinterpret_cast<IClientNetworkCallback &>(t_networkConfig.callback)),
    m_running(true),
    m_time(t_networkConfig.startTime) {
  m_state = NetworkState::Premature;
}
// Connects to server.
bool ClientBase::Connect() {
  if (!InsecureConnect()) {
    m_callback.OnConnect(false);
    m_state = NetworkState::Dead;
    return false;
  }
  m_callback.OnConnect(true);
  m_state = NetworkState::Alive;
  return true;
}
// Starts the life of client by given frequency.
// Blocks the thread until it is closed by another thread or error.
void ClientBase::Run() {

  while (m_running) {
    const double currentTime = yojimbo_time();
    if (m_time <= currentTime) {
      m_running = Update(m_fixedDeltaTime);
    } else {
      yojimbo_sleep(m_time - currentTime);
    }
  }
  m_state = NetworkState::VegetativeState;
  std::cout << "[GameClient] Disconnecting!!" << std::endl;

  m_messageAdapter.ReleaseMessages();
  m_client.Disconnect();
  m_state = NetworkState::Dead;
  m_callback.OnDisconnect();
}
// Stops the client.
void ClientBase::Stop() {
  m_running = false;

}
// Retrieves the current time of client.
double ClientBase::Time() const {
  return m_time;
}
// It creates a random client id and connects server with that id.
// Like its name tells use it is insecure connections. For secure connection.
// We need a match maker which will be available further versions.
bool ClientBase::InsecureConnect() {
  // Create client id.
  uint64_t clientId = 0;
  yojimbo::random_bytes(reinterpret_cast<uint8_t *>(&clientId), 8);
  std::cout << "[ClientBase] client id is " << clientId << std::endl;

  // Connect to server.
  m_client.InsecureConnect(m_privateKey, clientId, m_serverAddress);
  if (m_client.IsDisconnected()) {
    std::cerr << "[ClientBase] Client couldn't connect" << std::endl;
    return false;
  }

  char addressString[256];
  m_client.GetAddress().ToString(addressString, sizeof(addressString));
  std::cout << "[ClientBase] client address is " << addressString << std::endl;
  return true;
}
// Main loop of client.
bool ClientBase::Update(const double t_deltaTime) {
  if (m_client.IsDisconnected()) {
    return false;
  }

  // This order is very important.
  // You need to follow this order otherwise
  // You can not establish connection between server and client
  // Order is advance time, receive packet, process messages, send new packages.

  // Increase network time.
  m_client.AdvanceTime(m_time);
  m_time += t_deltaTime;

  // Receive packages.
  m_client.ReceivePackets();

  if (m_client.ConnectionFailed()) {
    std::cerr << "[ClientBase] Connection failed!!" << std::endl;
    m_callback.OnConnect(false);
    return false;
  }

  // Process received messages
  ProcessMessages();

  // Send new packages.
  m_client.SendPackets();

  return true;
}
void ClientBase::ProcessMessages() {
  if (m_client.IsConnected()) {
    for (int i = 0; i < m_numOfChannels; i++) {
      yojimbo::Message *message = m_client.ReceiveMessage(i);

      while (message != nullptr) {
        m_callback.OnMessageReceived(i, m_messageAdapter.CreateMessage(message));
        message = m_client.ReceiveMessage(i);
      }
    }
  }
}

