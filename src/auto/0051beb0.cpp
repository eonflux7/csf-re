#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, MissionScript_DestroyElementRange, 0x00469760, (int* first, int* last));

// Clears a buffer of script elements: frees the spare block when the capacity
// is at least 16 and it is non-null, resets the capacity to 15, then destroys
// the element range held at +8..+12 and frees its block. Returns the last
// result (0 when no element block was held).
static int __fastcall BddTables_ResetElementBuffer(void* self, void* /*edx*/) {
    if (CSF_AT(uint32_t, self, 0x40) >= 0x10u && CSF_AT(uint32_t, self, 0x2C) != 0) {
        Mem_FreeDebug(reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x2C)));
    }
    CSF_AT(uint32_t, self, 0x40) = 15;
    CSF_AT(uint32_t, self, 0x3C) = 0;
    CSF_AT(uint8_t, self, 0x2C) = 0;

    uint32_t firstElement = CSF_AT(uint32_t, self, 8);
    int result = static_cast<int>(firstElement);
    if (result != 0) {
        MissionScript_DestroyElementRange(
            reinterpret_cast<int*>(firstElement),
            reinterpret_cast<int*>(CSF_AT(uint32_t, self, 12)));
        result = static_cast<int>(CSF_AT(uint32_t, self, 8));
        if (result != 0) {
            result = Mem_FreeDebug(reinterpret_cast<void*>(CSF_AT(uint32_t, self, 8)));
        }
    }

    CSF_AT(uint32_t, self, 8) = 0;
    CSF_AT(uint32_t, self, 12) = 0;
    CSF_AT(uint32_t, self, 16) = 0;
    return result;
}

HOOK(0x0051BEB0, BddTables_ResetElementBuffer, "auto");
