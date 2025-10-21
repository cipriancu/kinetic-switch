#include "esphome/core/component.h"
#include "esphome/components/remote_receiver/remote_receiver.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace kinetic {

// Definim clasa decodorului care mosteneste RemoteReceiveData
class MyKineticSwitchDecoder : public remote_receiver::RemoteReceiveData {
 public:
  bool decode(remote_receiver::RemoteReceiveData *data) override;
};

// Definim un senzor care va raporta ID-ul (il vom trata ca un senzor text)
class KineticSwitchSensor : public sensor::Sensor, public Component {
 public:
  void set_remote_receiver(remote_receiver::RemoteReceiverComponent *rcvr) { 
    rcvr->add_decoder(new MyKineticSwitchDecoder(this)); 
  }
  
  // Functie pentru a actualiza valoarea senzorului dupa o decodare reusita
  void publish_id(uint32_t id_value); 
};

} // namespace my_rf_decoder
} // namespace esphome
