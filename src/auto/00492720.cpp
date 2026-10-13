#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Sets the stance value at +0x1D8. If it changed, notifies the embedded
// sub-object at +4 through its vtable slot 13 (offset 0x34) and returns that result.
static int __fastcall CoverAttack_SetStance(void* self, void* /*edx*/, int newStance)
{
    int result = newStance;
    if (newStance != CSF_AT(int, self, 0x1D8))
    {
        CSF_AT(int, self, 0x1D8) = newStance;
        void* subObject = reinterpret_cast<uint8_t*>(self) + 4;
        void** subVtbl = CSF_AT(void**, self, 4);
        return ((int(__fastcall*)(void*, void*))subVtbl[0x34 / 4])(subObject, nullptr);
    }
    return result;
}

HOOK(0x492720, CoverAttack_SetStance, "auto");
