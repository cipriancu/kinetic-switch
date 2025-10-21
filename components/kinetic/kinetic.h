#pragma once

#include "esphome/core/component.h"
#include "esphome/components/remote_receiver/remote_receiver.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace kinetic {

class KineticComponent : public Component {
 public:
  void set_remote_receiver(remote_receiver::RemoteReceiverComponent *rcvr) { rcvr_ = rcvr; }
  void loop() override;
  void publish_id(uint32_t id_value);

 protected:
  remote_receiver::RemoteReceiverComponent *rcvr_{nullptr};
  uint32_t last_id_{0};
};

}  // namespace kinetic
}  // namespace esphome
