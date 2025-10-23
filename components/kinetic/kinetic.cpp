#include "kinetic.h"
#include "esphome/core/log.h"

namespace esphome {
namespace kinetic {

static const char *const TAG = "kinetic";

bool KineticComponent::decode_kinetic(const std::vector<int32_t> &raw, uint32_t &id_value) {
  if (raw.size() < 10)
    return false;

  id_value = 0;
  for (size_t i = 0; i + 1 < raw.size(); i += 2) {
    uint32_t high = abs(raw[i]);
    uint32_t low = abs(raw[i + 1]);

    bool bit = (low < 300);  // prag ajustabil
    id_value = (id_value << 1) | (bit ? 1 : 0);
  }

  if (id_value == 0 || id_value == 0xFFFFFFFF)
    return false;

  return true;
}

void KineticComponent::on_receive(remote_base::RemoteReceiveData data) {
  auto raw = data.get_raw_data();

  ESP_LOGD(TAG, "Received %zu pulses (raw)", raw.size());
  for (size_t i = 0; i < raw.size(); i++) {
    ESP_LOGD(TAG, "  Raw[%zu] = %d", i, raw[i]);
  }

  uint32_t id = 0;
  if (decode_kinetic(raw, id)) {
    ESP_LOGI(TAG, "Decoded Kinetic ID: 0x%08X", id);
  } else {
    ESP_LOGD(TAG, "Kinetic decode failed or not matching (len=%zu)", raw.size());
  }
}

}  // namespace kinetic
}  // namespace esphome
