#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Callee at 0x69DB80: __thiscall-style, ecx = object, three stack args, returns char in al.
CSF_FUNC(char, __fastcall, C_0086c2b4_SetupFieldBuffer, 0x0069DB80,
         (void* self, void* edx, int a2, int a3, unsigned int a4));

// Dispatches to the callee with the field either by value (when the counter is >= 0x10)
// or by address (when below 0x10), passing the embedded sub-object at +8 and flag 1.
static char __fastcall C_00869124_DispatchByFieldThreshold(C_00869124* self, void* /*edx*/)
{
    if (self->field_024c < 0x10)
    {
        // Below threshold: pass the address of field_0238.
        int fieldAddress = static_cast<int>(reinterpret_cast<std::uintptr_t>(&self->field_0238));
        int bufferAddress = static_cast<int>(reinterpret_cast<std::uintptr_t>(static_cast<std::uint8_t*>(static_cast<void*>(self)) + 8));
        return C_0086c2b4_SetupFieldBuffer(self, nullptr, fieldAddress, bufferAddress, 1u);
    }

    // At or above threshold: pass the value of field_0238.
    int fieldValue = static_cast<int>(self->field_0238);
    int bufferAddress = static_cast<int>(reinterpret_cast<std::uintptr_t>(static_cast<std::uint8_t*>(static_cast<void*>(self)) + 8));
    return C_0086c2b4_SetupFieldBuffer(self, nullptr, fieldValue, bufferAddress, 1u);
}

HOOK(0x753A60, C_00869124_DispatchByFieldThreshold, "auto");
