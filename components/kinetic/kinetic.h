#pragma once
#include "esphome/core/component.h"
#include "esphome/components/remote_base/remote_base.h"
#include "esphome/components/remote_receiver/remote_receiver.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace kinetic {

// Component care ascultă semnale radio și raportează ID-ul unui switch
class KineticComponent : public Component, public remote_base::RemoteReceiverListener {
 public:
  sensor::Sensor *id_sensor_{nullptr};

  void setup() override;
  void dump_config() override;
  void on_receive(remote_base::RemoteReceiveData data) override;
  void set_id_sensor(sensor::Sensor *sensor) { id_sensor_ = sensor; }
};

}  // namespace kinetic
}  // namespace esphome

