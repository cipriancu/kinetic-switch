#include "kinetic.h"
#include "esphome/core/log.h"

namespace esphome {
namespace kinetic {

static const char *TAG = "kinetic";

// Parametri (bazat pe rtl_433 și ce-ai folosit)
static const int S_PULSE = 40;
static const int L_PULSE = 100;
static const int R_GAP = 1200;
static const int TOLERANCE = 30;  // ±30%

static bool matches(uint32_t a, uint32_t b) {
  return (a > (b * (100 - TOLERANCE) / 100)) && (a < (b * (100 + TOLERANCE) / 100));
}

void KineticComponent::setup() {
  ESP_LOGI(TAG, "Kinetic component setup");
}

void KineticComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "Kinetic component ready");
}

void KineticComponent::on_receive(const remote_base::RemoteReceiveData &data) {
  // data.timings este vector<uint32_t> cu duratele
  uint32_t id_value = 0;
  if (decode_kinetic(data.timings, id_value)) {
    ESP_LOGI(TAG, "Decoded Kinetic ID: 0x%07X (%u)", id_value, id_value);
    if (id_sensor_ != nullptr) {
      // sensor::Sensor publish_state accepta float; convertim
      id_sensor_->publish_state(static_cast<float>(id_value));
    }
  } else {
    ESP_LOGD(TAG, "Kinetic decode failed (len=%u)", (unsigned)data.timings.size());
  }
}

bool KineticComponent::decode_kinetic(const std::vector<uint32_t> &timings, uint32_t &id_value) {
  // Verificare minimala: 25 biti * 2 timings/bit = 50 timpi
  if (timings.size() < 50) return false;

  id_value = 0;
  int bit_count = 0;

  // Iteram, cautand biti S+L (0) sau L+S (1)
  // Daca gasim R_GAP (preambul) resetam sincronizarea si continuam
  for (size_t i = 0; i + 1 < timings.size() && bit_count < 25; i += 2) {
    uint32_t pulse = timings[i];
    uint32_t gap = timings[i + 1];

    if (matches(pulse, S_PULSE) && matches(gap, L_PULSE)) {
      // bit 0
      id_value <<= 1;
      bit_count++;
    } else if (matches(pulse, L_PULSE) && matches(gap, S_PULSE)) {
      // bit 1
      id_value = (id_value << 1) | 1;
      bit_count++;
    } else if (matches(pulse, R_GAP) || matches(gap, R_GAP)) {
      // preambul -> reseteaza
      id_value = 0;
      bit_count = 0;
      // pastram indexul pentru a continua cautarea dupa preambul
    } else {
      // nu se potriveste niciun format - esuam
      return false;
    }
  }

  return (bit_count == 25);
}

}  // namespace kinetic
}  // namespace esphome
