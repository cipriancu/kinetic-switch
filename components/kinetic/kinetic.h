#pragma once

#include "esphome/core/component.h"
#include "esphome/components/remote_base/remote_base.h"

namespace esphome {
namespace kinetic {

class KineticComponent : public Component, public remote_base::RemoteReceiverListener {
 public:
  void setup() override {}
  void loop() override {}
  bool on_receive(remote_base::RemoteReceiveData data) override;  // <== bool, nu void!
};

}  // namespace kinetic
}  // namespace esphome
