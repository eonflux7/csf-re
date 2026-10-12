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

// Callee at 0x69DB80: ecx = object, three stack args, returns char in al.
CSF_FUNC(char, __fastcall, C_0086c2b4_SetupFieldBuffer, 0x0069DB80,
         (void* self, void* edx, int a2, int a3, unsigned int a4));

// Dispatches to the callee with the field either by address (when the counter is below 0x10)
// or by value (when the counter is >= 0x10), passing the embedded sub-object at +8 and flag 1.
static char __fastcall C_008449a0_DispatchByCounterThreshold(C_008449a0* self, void* /*edx*/)
{
    if (self->field_0158 < 0x10u)
    {
        // Below threshold: pass the address of field_0144.
        int fieldAddress = static_cast<int>(reinterpret_cast<std::uintptr_t>(&self->field_0144));
        int bufferAddress = static_cast<int>(reinterpret_cast<std::uintptr_t>(static_cast<std::uint8_t*>(static_cast<void*>(self)) + 8));
        return C_0086c2b4_SetupFieldBuffer(self, nullptr, fieldAddress, bufferAddress, 1u);
    }

    // At or above threshold: pass the value of field_0144.
    int fieldValue = static_cast<int>(self->field_0144);
    int bufferAddress = static_cast<int>(reinterpret_cast<std::uintptr_t>(static_cast<std::uint8_t*>(static_cast<void*>(self)) + 8));
    return C_0086c2b4_SetupFieldBuffer(self, nullptr, fieldValue, bufferAddress, 1u);
}

HOOK(0x55AB70, C_008449a0_DispatchByCounterThreshold, "auto");
