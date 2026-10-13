#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(void*, __fastcall, C_00830438_Construct, 0x00712CC0, (void* self, void* edx));
CSF_GLOBAL(int, g_Value8B0668, 0x008B0668);

// Returns the lazily created C_00830438 singleton (allocated once and cached in g_Value8B0668).
static C_00830438* __cdecl C_00830438_GetOrCreateInstance()
{
    C_00830438* result = reinterpret_cast<C_00830438*>(static_cast<std::uintptr_t>(g_Value8B0668));
    if (g_Value8B0668 == 0)
    {
        int allocated = CoreMemory_AllocUntagged(40);
        result = reinterpret_cast<C_00830438*>(static_cast<std::uintptr_t>(allocated));
        if (allocated != 0)
        {
            uint8_t* obj = reinterpret_cast<uint8_t*>(static_cast<std::uintptr_t>(allocated));
            void* constructReturn = C_00830438_Construct(obj, nullptr);
            result = reinterpret_cast<C_00830438*>(constructReturn);

            *reinterpret_cast<void***>(obj) = CSF_VTBL(0x00840CA8);
            CSF_AT(int, obj, 0x18) = 0;
            CSF_AT(int, obj, 0x1C) = 0;
            CSF_AT(int, obj, 0x20) = 0;
            CSF_AT(int, obj, 0x0C) = 0;
            CSF_AT(int, obj, 0x10) = 0;
            CSF_AT(uint8_t, obj, 0x25) = 0;
            g_Value8B0668 = allocated;
        }
        else
        {
            g_Value8B0668 = 0;
        }
    }
    return result;
}

HOOK(0x005193B0, C_00830438_GetOrCreateInstance, "auto");
