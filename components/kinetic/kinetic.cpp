#include "kinetic.h"
#include "esphome/core/log.h"
#include <cinttypes>

namespace esphome {
namespace kinetic {

static const char *const TAG = "kinetic";

static bool matches_t(uint32_t measured, uint32_t expected, int tol_pct) {
  // convert measured (unsigned) to signed int to handle negative encoded values
  int32_t s = static_cast<int32_t>(measured);
  uint32_t dur = (s < 0) ? static_cast<uint32_t>(-s) : static_cast<uint32_t>(s);
  uint32_t low = expected * (100 - tol_pct) / 100;
  uint32_t high = expected * (100 + tol_pct) / 100;
  return dur >= low && dur <= high;
}
bool KineticComponent::decode_kinetic(const std::vector<uint32_t> &raw, uint32_t &id_value) {
  if (raw.size() < 10) return false;

  id_value = 0;
  for (size_t i = 0; i + 1 < raw.size(); i += 2) {
    uint32_t high = raw[i];
    uint32_t low  = abs((int32_t)raw[i + 1]);
    // scurt = 1, lung = 0 (sau invers, după caz)
    bool bit = (low < 300);  // prag ~300 µs, ajustabil
    id_value = (id_value << 1) | (bit ? 1 : 0);
  }

  // Filtru simplu: ignoră zgomotul scurt
  if (id_value == 0 || id_value == 0xFFFFFFFF) return false;

  return true;
}

  // Need at least 25 bits * 2 = 50 timings
  if (timings.size() < 50) return false;

  id_value = 0;
  int bit_count = 0;

  // Căutăm 25 biți; bit -> (pulse + gap)
  for (size_t i = 0; i + 1 < timings.size() && bit_count < 25; i += 2) {
    uint32_t pulse = timings[i];
    uint32_t gap = timings[i + 1];

    if (matches_t(pulse, S_PULSE, TOLERANCE) && matches_t(gap, L_PULSE, TOLERANCE)) {
      // bit 0
      id_value <<= 1;
      bit_count++;
    } else if (matches_t(pulse, L_PULSE, TOLERANCE) && matches_t(gap, S_PULSE, TOLERANCE)) {
      // bit 1
      id_value = (id_value << 1) | 1;
      bit_count++;
    } else {
      // posibil preambul (R_GAP) -> resetați sincronizarea dacă detectați R
      if (matches_t(pulse, R_GAP, TOLERANCE) || matches_t(gap, R_GAP, TOLERANCE)) {
        id_value = 0;
        bit_count = 0;
        // nu incrementăm i altfel; continuăm
        continue;
      }
      // nu se potrivește; abandonăm
      return false;
    }
  }

  return (bit_count == 25);
}

bool KineticComponent::on_receive(remote_base::RemoteReceiveData data) {
  // obținem raw pulses (unsigned values); funcția get_raw_data() returnează vector<uint32_t>
  auto raw = data.get_raw_data();
  ESP_LOGD(TAG, "Received %zu pulses (raw)", raw.size());

  // debug: log primele câteva (opțional)
  for (size_t i = 0; i < raw.size() && i < 12; ++i) {
    int32_t sv = static_cast<int32_t>(raw[i]);
    ESP_LOGD(TAG, "  Raw[%zu] = %" PRId32, i, sv);
  }

  uint32_t id = 0;
  ESP_LOGI(TAG, "Raw pulses count: %d", raw.size());
for (size_t i = 0; i < raw.size(); i++) {
  ESP_LOGI(TAG, "Pulse[%d] = %d", i, raw[i]);
}

  // returnăm true ca să indicăm că am procesat (sau cel puțin încercat)
  return true;
}

}  // namespace kinetic
}  // namespace esphome
