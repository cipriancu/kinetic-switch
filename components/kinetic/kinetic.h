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
  bool decode_kinetic(const std::vector<int32_t> &raw, uint32_t &id_value);


// CONSTANTE RECALIBRATE PE BAZA LOGURILOR (165/300)
  static constexpr int S_PULSE = 170;   // Puls Scurt (Aprox. 150-185)
  static constexpr int L_PULSE = 300;   // Puls Lung (Aprox. 280-320)
  
  // Am redenumit Gap-urile pentru a fi mai clare în logica decodării
  static constexpr int S_GAP_EXPECTED = 110; // Gap Scurt (Aprox. 75-125)
  static constexpr int L_GAP_EXPECTED = 450; // Gap Lung (Aprox. 350-550)
  
  static constexpr int R_GAP = 1700;    // Gap de Reset/Preambul
  static constexpr int TOLERANCE = 35;  // Toleranță crescută la ±40%
  static constexpr int TOLERANCE_WIDE = 50; // Toleranță pentru Gap-ul Lung (50%)
  
  // Numărul de biți de decodat pentru codul Kinetic (25 biți)
  static constexpr int KINETIC_BITS = 15;
  // Lungimea minimă a vectorului RAW necesară (KINETIC_BITS * 2)
  static constexpr int MIN_RAW_LEN = KINETIC_BITS * 2;
};

}  // namespace kinetic
}  // namespace esphome
