#pragma once

#include "esphome/core/component.h"
#include "esphome/components/remote_base/remote_base.h"

namespace esphome {
namespace kinetic {

class KineticComponent : public Component, public remote_base::RemoteReceiverListener {
 public:
  void setup() override;
  void dump_config() override;
  void on_receive(remote_base::RemoteReceiveData data) override;

 protected:
  void handle_kinetic_code(uint32_t code);
};

}  // namespace kinetic
}  // namespace esphome

