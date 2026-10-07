#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// 0x00724F40  CGuiButton::SetStateTime(int stateIndex, float time)   (ret 8)
//
// Writes into the four 28-byte (0x1C) state records laid out at +0x14C:
//   index 0 -> +0x14C REPOSO, 1 -> +0x168 ACTIVO, 2 -> +0x184 PULSADO,
//   3 -> +0x1A0 PERMANENTE.
// For the chosen record it stores `time` into the record's float time field
// (record + 4 + 4 = +0x154 relative to the record base) and marks the record's
// "timed" byte (record + 4 = +0x150) as 1. It then sets the button-wide
// m_bStateTimes flag (+0x1BD) and returns the byte offset of the record
// (stateIndex * 28).
static int __fastcall CGuiButton_SetStateTime(CGuiButton* self, void* /*edx*/, int stateIndex, float time)
{
    // (stateIndex + 12) * 0x1C == stateIndex * 0x1C + 0x150 : the "timed" byte of the record.
    uint8_t* const recordTimed = reinterpret_cast<uint8_t*>(self) + (stateIndex + 12) * 28;
    // stateIndex * 0x1C : base of the record (its quad pointer at +0x14C; time at +0x154).
    uint8_t* const recordBase = reinterpret_cast<uint8_t*>(self) + stateIndex * 28;

    *reinterpret_cast<float*>(recordBase + 0x154) = time;
    *recordTimed = 1;
    self->m_bStateTimes = 1;

    return stateIndex * 28;
}

HOOK(0x724F40, CGuiButton_SetStateTime, "auto");
