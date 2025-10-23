#include "kinetic.h"
#include "esphome/core/log.h"

namespace esphome {
namespace kinetic {

static const char *const TAG = "kinetic";

bool KineticComponent::on_receive(remote_base::RemoteReceiveData data) {
  auto values = data.get_data();  // vector<uint16_t>
  ESP_LOGD(TAG, "Received %zu pulses", values.size());

  for (size_t i = 0; i < values.size() && i < 10; i++) {
    ESP_LOGD(TAG, "  Pulse[%zu] = %u", i, values[i]);
  }

  return true;  // semnal procesat
}

}  // namespace kinetic
}  // namespace esphome
