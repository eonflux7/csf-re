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

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, SceneLights_InitSetupObject, 0x004F1B60, (void* self, void* edx, int a2));

// Creates a 0x48-byte setup object when a1 is zero and hands it to the init routine with a2.
// Returns the init routine's result, or nullptr when allocation fails or a1 is non-zero.
static uint32_t* __cdecl SceneLights_CreateSetupObject(int a1, int a2)
{
    uint32_t* result = nullptr;
    if (a1 == 0)
    {
        C_0083f040* setupObject = static_cast<C_0083f040*>(Mem_Alloc(0x48));
        if (setupObject)
            return SceneLights_InitSetupObject(setupObject, nullptr, a2);
        return nullptr;
    }
    return result;
}

HOOK(0x004F0DC0, SceneLights_CreateSetupObject, "auto");
