#include "kinetic.h"
#include "esphome/core/log.h"
#include "esphome/components/remote_base/remote_base.h"
#include <cinttypes>
#include <vector>

namespace esphome {
namespace kinetic {

static const char *const TAG = "kinetic";

// Funcție ajutătoare pentru a verifica o durată (absolută) cu o toleranță
static bool matches_t(uint32_t measured, uint32_t expected, int tol_pct) {
  uint32_t low = expected * (100 - tol_pct) / 100;
  uint32_t high = expected * (100 + tol_pct) / 100;
  return measured >= low && measured <= high;
}

bool KineticComponent::decode_kinetic(const std::vector<int32_t> &raw, uint32_t &id_value) {
    // ... (verificare MIN_RAW_LEN OK) ...

    id_value = 0;
    int bit_count = 0;
    size_t start_index = 0;

    // 1. Căutăm punctul de start valid (primul puls pozitiv)
    while (start_index < raw.size() && raw[start_index] <= 0) {
        start_index++;
    }
    
    // 2. Acum, start_index e primul puls. Căutăm perechi Puls/Gap de la acest punct.
    for (size_t i = start_index; i + 1 < raw.size() && bit_count < KINETIC_BITS; i += 2) {
        int32_t pulse_raw = raw[i];
        int32_t gap_raw = raw[i + 1];

        // Verificare de bază a polarității
        if (pulse_raw <= 0 || gap_raw >= 0) {
            return false; // Structură coruptă
        }

        uint32_t abs_pulse = static_cast<uint32_t>(pulse_raw);
        uint32_t abs_gap = static_cast<uint32_t>(-gap_raw);

        // Resetare/Skip dacă detectăm R_GAP (gap mare de separare pachete)
        if (matches_t(abs_gap, R_GAP, TOLERANCE)) {
             id_value = 0;
             bit_count = 0;
             // Ajustăm i pentru a reîncepe căutarea de la elementul de după R_GAP
             i -= 1; 
             continue;
        }


        // Bit 0: Puls Scurt (S) + Gap Lung (L)
        // Aplicăm TOLERANCE_WIDE (50%) doar pentru L_GAP_EXPECTED
        if (matches_t(abs_pulse, S_PULSE, TOLERANCE) && matches_t(abs_gap, L_GAP_EXPECTED, TOLERANCE_WIDE)) {
            id_value <<= 1;
            bit_count++;
        } 
        // Bit 1: Puls Lung (L) + Gap Scurt (S)
        else if (matches_t(abs_pulse, L_PULSE, TOLERANCE) && matches_t(abs_gap, S_GAP_EXPECTED, TOLERANCE)) {
            id_value = (id_value << 1) | 1;
            bit_count++;
        } 
        else {
            // Nu se potrivește niciun bit valid: Abandonăm decodarea
            return false;
        }
    }

    return (bit_count == KINETIC_BITS);
}

bool KineticComponent::on_receive(remote_base::RemoteReceiveData data) {
  // get_raw_data() returnează un vector<int32_t> (pulse > 0, gap < 0)
  auto raw = data.get_raw_data();
  ESP_LOGD(TAG, "Received %zu pulses (raw)", raw.size());

  // Logare pentru debug (primele 12 elemente)
  for (size_t i = 0; i < raw.size() && i < 12; ++i) {
    ESP_LOGD(TAG, "  Raw[%zu] = %" PRId32, i, raw[i]);
  }

  uint32_t id = 0;
  if (decode_kinetic(raw, id)) {
    // Kinetic ID este de obicei 25 de biți, deci e afișat ca 7 caractere HEX (0x1FFFFFF)
    ESP_LOGI(TAG, "Kinetic ID decoded: 0x%07" PRIX32 " (%u)", id, id);
    // Dacă ai fi avut text_sensor, l-ai fi publicat aici.
    // Deoarece nu mai ai text_sensor, doar logăm.
  } else {
    ESP_LOGD(TAG, "Kinetic decode failed or not matching (len=%zu)", raw.size());
  }

  // Returnăm true pentru a indica că am procesat datele
  return true;
}

}  // namespace kinetic
}  // namespace esphome
