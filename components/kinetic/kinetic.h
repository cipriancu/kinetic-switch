#pragma once
#include "esphome/core/component.h"
#include "esphome/components/remote_base/remote_base.h"
#include "esphome/components/remote_receiver/remote_receiver.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace kinetic {

class KineticComponent : public Component, public remote_base::RemoteReceiverListener {
 public:
  sensor::Sensor *id_sensor_{nullptr};

  void setup() override;
  void dump_config() override;
  void on_receive(remote_base::RemoteReceiveData data) override;

  void set_id_sensor(sensor::Sensor *sensor) { id_sensor_ = sensor; }

 private:
  bool decode_kinetic(const std::vector<uint32_t> &timings, uint32_t &id_value);
};

}  // namespace kinetic
}  // namespace esphome
