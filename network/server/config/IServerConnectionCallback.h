#ifndef GAMENETWORK_NETWORK_SERVER_ADAPTER_ISERVERCONNECTIONCALLBACK_H_
#define GAMENETWORK_NETWORK_SERVER_ADAPTER_ISERVERCONNECTIONCALLBACK_H_
struct IServerConnectionCallback {
  virtual ~IServerConnectionCallback() = default;
  virtual void OnServerStarted(bool success) = 0;
  virtual void OnServerDisconnected() = 0;
  virtual void OnClientConnected(int t_clientIndex) = 0;
  virtual void OnClientDisconnected(int t_clientIndex) = 0;
};
#endif //GAMENETWORK_NETWORK_SERVER_ADAPTER_ISERVERCONNECTIONCALLBACK_H_
