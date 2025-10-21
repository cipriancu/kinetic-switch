#include "kinetic.h"
#include "esphome/core/log.h"

namespace esphome {
namespace kinetic {

static const char *const TAG = "kinetic";

// Timings (aprox)
static const int S_PULSE = 40;
static const int L_PULSE = 100;
static const int TOLERANCE = 30;

bool matches(uint32_t a, uint32_t b) {
  return (a > (b * (100 - TOLERANCE) / 100)) && (a < (b * (100 + TOLERANCE) / 100));
}

bool decode_kinetic(const std::vector<uint32_t> &timings, uint32_t &id_value) {
  if (timings.size() < 50) return false;

  id_value = 0;
  int bit_count = 0;
  for (size_t i = 0; i < timings.size() - 1 && bit_count < 25; i += 2) {
    uint32_t pulse = timings[i];
    uint32_t gap = timings[i + 1];

    if (matches(pulse, S_PULSE) && matches(gap, L_PULSE)) {
      id_value <<= 1;
    } else if (matches(pulse, L_PULSE) && matches(gap, S_PULSE)) {
      id_value = (id_value << 1) | 1;
    } else {
      return false;
    }
    bit_count++;
  }
  return bit_count == 25;
}

void KineticComponent::loop() {
  if (!this->rcvr_) return;

  auto data = this->rcvr_->get_raw_data();
  if (!data.has_value()) return;

  uint32_t id_value = 0;
  if (decode_kinetic(data.value().timings, id_value)) {
    if (id_value != this->last_id_) {
      this->last_id_ = id_value;
      ESP_LOGI(TAG, "Decoded Kinetic ID: 0x%07X", id_value);
      publish_id(id_value);
    }
  }
}

void KineticComponent::publish_id(uint32_t id_value) {
  // TODO: publish to a sensor if needed
  ESP_LOGD(TAG, "Publishing ID: %u", id_value);
}

}  // namespace kinetic
}  // namespace esphome
