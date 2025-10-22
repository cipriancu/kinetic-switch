#include "kinetic.h"
#include "esphome/core/log.h"

namespace esphome {
namespace kinetic {

static const char *const TAG = "kinetic";

void KineticComponent::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Kinetic component...");
}

void KineticComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "Kinetic component active.");
}

void KineticComponent::on_receive(remote_base::RemoteReceiveData data) {
  if (data.rtt > 0 && data.raw.size() > 0) {
    uint32_t mock_code = 0;
    for (auto v : data.raw) {
      mock_code = (mock_code << 1) ^ (v > 1000 ? 1 : 0);
    }
    handle_kinetic_code(mock_code);
  }
}

void KineticComponent::handle_kinetic_code(uint32_t code) {
  ESP_LOGD(TAG, "Received kinetic code: 0x%08X", code);

  // Aici poți face ce vrei cu codul — momentan doar loghează.
  // Poți ulterior să-l mapezi la un binary_sensor custom.
}

}  // namespace kinetic
}  // namespace esphome
