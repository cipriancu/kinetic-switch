#pragma once

#include "esphome/core/component.h"
#include "esphome/components/remote_base/remote_base.h"
#include <vector>
#include <cstdint>

namespace esphome {
namespace kinetic {

class KineticComponent : public Component, public remote_base::RemoteReceiverListener {
 public:
  void setup() override {}
  bool on_receive(remote_base::RemoteReceiveData data) override;

 protected:
  // decode_kinetic primește vectorul de timpi "raw" (unsigned)
  // și returnează true + id_value dacă găsește un pachet valid de 25 biți.
  bool decode_kinetic(const std::vector<uint32_t> &raw, uint32_t &id_value);

  // parametri (poți ajusta dacă vezi nevoie)
  static constexpr int S_PULSE = 40;
  static constexpr int L_PULSE = 100;
  static constexpr int R_GAP = 1200;
  static constexpr int TOLERANCE = 30;  // ±30%
};

}  // namespace kinetic
}  // namespace esphome
