#ifndef GAMENETWORK_NETWORK_ABSTRACTION_NETWORKSTATE_H_
#define GAMENETWORK_NETWORK_ABSTRACTION_NETWORKSTATE_H_
enum class NetworkState {
  Premature,        // Connected but not started.
  Alive,            // Started and healthy. It can work full power.
  VegetativeState,  // Is shutting down itself.
  Dead,             // Totally useless, already shut down.
};
#endif //GAMENETWORK_NETWORK_ABSTRACTION_NETWORKSTATE_H_
