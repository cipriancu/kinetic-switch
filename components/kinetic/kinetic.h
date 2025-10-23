#pragma once
#include "esphome/core/component.h"
#include "esphome/components/remote_base/remote_base.h"

namespace esphome {
namespace kinetic {

class KineticComponent : public Component, public remote_base::RemoteReceiverListener {
 public:
  void on_receive(remote_base::RemoteReceiveData data) override;
  bool decode_kinetic(const std::vector<int32_t> &raw, uint32_t &id_value);
};

}  // namespace kinetic
}  // namespace esphome
