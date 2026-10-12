#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Copies the 8-dword block at source offsets 0x1C..0x38 into every emitter record
// in [a1, a2), stepping 0x3C bytes per record. Each record first gets its string
// reassigned from a3 (AssignStringSubstring(record, a3, 0, -1)).
static C_0082d410* __cdecl ParticleEmitters_CopyEmitterRange(C_0082d410* a1, C_0082d410* a2, uint32_t* a3)
{
    C_0082d410* result = a2;
    if (a1 != a2) {
        uint8_t* record = reinterpret_cast<uint8_t*>(a1);
        uint8_t* end = reinterpret_cast<uint8_t*>(a2);
        do {
            AssignStringSubstring(record, nullptr, a3, 0, 0xFFFFFFFFu);
            for (std::size_t i = 0; i < 8; ++i) {
                const std::size_t off = 0x1C + i * 4;
                CSF_AT(uint32_t, record, off) = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(a3), off);
            }
            result = a2;
            record += 0x3C;
        } while (record != end);
    }
    return result;
}

HOOK(0x006E1940, ParticleEmitters_CopyEmitterRange, "auto");
