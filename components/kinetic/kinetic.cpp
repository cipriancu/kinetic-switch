#include "kinetic.h"
#include "esphome/core/log.h"
#include "esphome/components/remote_base/remote_base.h" // NOU!

namespace esphome {
namespace kinetic {

static const char *const TAG = "kinetic";

// Timings (aprox)
static const int S_PULSE = 40;
static const int L_PULSE = 100;
static const int TOLERANCE = 30;

// ... (functia matches ramane neschimbata)

// ... (functia decode_kinetic ramane neschimbata)

// Implementarea on_receive care inlocuieste logica din loop
void KineticComponent::on_receive(const remote_base::RemoteReceiveData &data) {
  // Accesam timings din data
  const auto &timings = data.timings; 

  // In acest punct, datele brute vin direct de la remote_receiver.
  uint32_t id_value = 0;
  if (decode_kinetic(timings, id_value)) {
    if (id_value != this->last_id_) {
      this->last_id_ = id_value;
      ESP_LOGI(TAG, "Decoded Kinetic ID: 0x%07X", id_value);
      publish_id(id_value);
    }
  } else {
    // Mesaj de debugging in caz ca decodarea esueaza
    ESP_LOGD(TAG, "Kinetic decode failed (len=%u)", (unsigned)timings.size());
  }
}

// Functia de publicare, implementata sa publice in senzor
void KineticComponent::publish_id(uint32_t id_value) {
  if (this->id_sensor_) {
    // Convertim ID-ul numeric la un șir de caractere HEX pentru a-l publica ca senzor
    // NOTA: Schimbați tipul senzorului in YAML in "text_sensor" daca doriti HEX.
    char id_hex_str[9]; // 7 caractere Hex + null terminator
    snprintf(id_hex_str, sizeof(id_hex_str), "%07X", id_value); 
    
    // Publicam valoarea ca text (HEX)
    this->id_sensor_->publish_state(id_hex_str); 
    ESP_LOGD(TAG, "Published ID 0x%s to sensor", id_hex_str);
  }
}

}  // namespace kinetic
}  // namespace esphome
