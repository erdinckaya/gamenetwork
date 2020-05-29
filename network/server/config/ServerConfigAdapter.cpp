#include "ServerConfigAdapter.h"

void ServerConfigAdapter::OnServerClientConnected(int t_clientIndex) {
  m_callback.OnClientConnected(t_clientIndex);
}
void ServerConfigAdapter::OnServerClientDisconnected(int t_clientIndex) {
  m_callback.OnClientDisconnected(t_clientIndex);

}