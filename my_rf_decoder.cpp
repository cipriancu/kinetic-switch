#include "my_rf_decoder.h"
#include "esphome/core/log.h"
#include <cstdio>

namespace esphome {
namespace my_rf_decoder {

static const char *const TAG = "my_rf_decoder";

// Timings bazate pe rtl_433: s=40, l=100, r=1200
static const int S_PULSE = 40;
static const int L_PULSE = 100;
static const int R_GAP = 1200;

// Toleranta (folosim o marja de 30%)
static const int TOLERANCE = 30; 

// Functie utilitara pentru compararea timing-urilor
bool matches(uint32_t a, uint32_t b) {
  return (a > (b * (100 - TOLERANCE) / 100)) && (a < (b * (100 + TOLERANCE) / 100));
}

bool MyKineticSwitchDecoder::decode(remote_receiver::RemoteReceiveData *data) {
    // Ne asiguram ca avem timings suficiente pentru a citi cel putin un pachet de 25 biti
    if (data->timings.size() < 50) { // 25 biti * 2 timings/bit = 50 timings
        return false;
    }

    uint32_t id_value = 0;
    int bit_count = 0;
    int data_start_index = 0; // Vom presupune ca datele incep de la index 0 pentru simplitate

    // Logica OOK_PWM: Un bit e o pereche (puls + gol)
    // Bitul 0: Puls Scurt (S) + Gol Lung (L)
    // Bitul 1: Puls Lung (L) + Gol Scurt (S)
    
    // Cautam secventa de 25 de biti (sau mai mult, e.g. 50 de timings)
    for (size_t i = data_start_index; i < data->timings.size() - 1 && bit_count < 25; i += 2) {
        uint32_t pulse = data->timings[i];
        uint32_t gap = data->timings[i+1];
        
        // Cauta un bit valid S+L sau L+S
        if (matches(pulse, S_PULSE) && matches(gap, L_PULSE)) {
            // Bit 0: S+L
            id_value <<= 1;
            id_value |= 0;
            bit_count++;
        } else if (matches(pulse, L_PULSE) && matches(gap, S_PULSE)) {
            // Bit 1: L+S
            id_value <<= 1;
            id_value |= 1;
            bit_count++;
        } else {
            // Daca nu se potriveste, semnalul nu este cel dorit sau am pierdut sincronizarea
            // Daca vrei o decodare mai robusta, ar trebui sa cauti un preambul/reset (R=1200) inainte de a incepe
            return false; 
        }
    }

    if (bit_count == 25) {
        ESP_LOGI(TAG, "Decodare reusita! ID: 0x%07X", id_value); // Afisam ID-ul in format Hex

        // NOTA: Aici trebuie sa trimiti ID-ul la senzorul din YAML
        // Asta necesita o referinta la clasa KineticSwitchSensor
        // Deoarece decoderele sunt adaugate, ar trebui sa pastrezi referinta la senzor.

        // TODO: In practica, trebuie sa obtii o referinta la KineticSwitchSensor si sa apelezi publish_id(id_value);
        // De dragul exemplului, vom returna true.
        return true; 
    }

    return false;
}
// ... Functia publish_id va converti uint32_t in string (hex) si va apela this->publish_state(id_hex_string);
// ...
} // namespace my_rf_decoder
} // namespace esphome
