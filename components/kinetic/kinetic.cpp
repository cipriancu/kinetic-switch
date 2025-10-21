#include "kinetic.h"
#include "esphome/core/log.h"

namespace esphome {
namespace kinetic {

static const char *TAG = "kinetic";

// Timings bazate pe rtl_433: s=40, l=100, r=1200
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

void KineticComponent::on_receive(remote_base::RemoteReceiveData data) {
  uint32_t id_value = 0;
  if (decode_kinetic(data.timings, id_value)) {
    ESP_LOGI(TAG, "Decoded Kinetic ID: 0x%07X", id_value);
    if (id_sensor_ != nullptr) {
      id_sensor_->publish_state(id_value);
    }
  }
}

bool KineticComponent::decode_kinetic(const std::vector<uint32_t> &timings, uint32_t &id_value) {
  if (timings.size() < 50)  // 25 bits * 2 timings/bit
    return false;

  id_value = 0;
  int bit_count = 0;

  for (size_t i = 0; i < timings.size() - 1 && bit_count < 25; i += 2) {
    uint32_t pulse = timings[i];
    uint32_t gap = timings[i + 1];

    if (matches(pulse, S_PULSE) && matches(gap, L_PULSE)) {
      id_value <<= 1;
      bit_count++;
    } else if (matches(pulse, L_PULSE) && matches(gap, S_PULSE)) {
      id_value = (id_value << 1) | 1;
      bit_count++;
    } else if (matches(pulse, R_GAP)) {
      // preambul — poți reseta sincronizarea aici dacă vrei
      bit_count = 0;
      id_value = 0;
    } else {
      return false;
    }
  }

  return bit_count == 25;
}

}  // namespace kinetic
}  // namespace esphome
