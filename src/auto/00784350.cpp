#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __fastcall, AssignElement, 0x00784150, (void* self, void*, void* source));

static uint32_t __cdecl Assign_00784350(uint32_t begin, uint32_t end, uint32_t source)
{
    // The empty range leaves EAX untouched in the binary: no defined result.
    uint32_t result = 0;
    for (; begin != end; begin += 552u)
        result = reinterpret_cast<uint32_t>(AssignElement(reinterpret_cast<void*>(begin), nullptr, reinterpret_cast<void*>(source)));
    return result;
}
HOOK(0x00784350, Assign_00784350, "stl");
