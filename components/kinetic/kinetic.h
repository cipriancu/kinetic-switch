#pragma once

#include "esphome/core/component.h"
#include "esphome/components/remote_base/remote_base.h" // NOU!
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace kinetic {

class KineticComponent : public Component, public remote_base::RemoteReceiverListener { // Implementam interfața
 public:
  // Functia necesara din RemoteReceiverListener
  void on_receive(const remote_base::RemoteReceiveData &data) override; 
  
  void set_id_sensor(sensor::Sensor *sens) { this->id_sensor_ = sens; }
  void loop() override {} // Nu mai avem nevoie de logica in loop

 protected:
  void publish_id(uint32_t id_value);

  sensor::Sensor *id_sensor_{nullptr};
  uint32_t last_id_{0};
};

}  // namespace kinetic
}  // namespace esphome
