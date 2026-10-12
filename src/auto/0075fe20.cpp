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

// Calls SetupFieldBuffer with the embedded sub-object at +8 and flag 0. The field at +0x118 is
// passed by address when the counter at +0x12C is below 0x10, and by value otherwise.
static char __fastcall C_0086c2b4_DispatchFieldBufferByCounter(C_0086c2b4* self, void* /*edx*/)
{
    if (self->field_012c < 0x10u)
    {
        // Below threshold: pass the address of field_0118.
        int fieldAddress = static_cast<int>(reinterpret_cast<std::uintptr_t>(&self->field_0118));
        int bufferAddress = static_cast<int>(reinterpret_cast<std::uintptr_t>(static_cast<std::uint8_t*>(static_cast<void*>(self)) + 8));
        return C_0086c2b4_SetupFieldBuffer(self, nullptr, fieldAddress, bufferAddress, 0u);
    }

    // At or above threshold: pass the value of field_0118.
    int fieldValue = static_cast<int>(self->field_0118);
    int bufferAddress = static_cast<int>(reinterpret_cast<std::uintptr_t>(static_cast<std::uint8_t*>(static_cast<void*>(self)) + 8));
    return C_0086c2b4_SetupFieldBuffer(self, nullptr, fieldValue, bufferAddress, 0u);
}

HOOK(0x75FE20, C_0086c2b4_DispatchFieldBufferByCounter, "auto");
