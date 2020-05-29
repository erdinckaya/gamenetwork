#ifndef GAMENETWORK_NETWORK_ABSTRACTION_NETWORKDEBUG_H_
#define GAMENETWORK_NETWORK_ABSTRACTION_NETWORKDEBUG_H_
// Network debug data.
struct NetworkDebug {
  explicit NetworkDebug() : latency(0.0f), jitter(0.0f), duplicates(0.0f) {}

  float latency;
  float jitter;
  float duplicates;
};

#endif //GAMENETWORK_NETWORK_ABSTRACTION_NETWORKDEBUG_H_
