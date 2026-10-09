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

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, GetControlEntryByIndex, 0x0049ED10, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, IsControlReadyFlagSet, 0x00755880, (int a1));

// Resolves a handle to a control in the current scene, checks its ready flag,
// reads a float index from the argument chain, and if it is in range copies two
// dwords from the indexed table entry into the object at a1+4.
static int __stdcall CopyTableEntryFromHandleIndex(int a1, int /*a2*/, uint32_t* a3)
{
    uint32_t wrapper = a3[0];
    uint32_t infoBlock = CSF_AT(uint32_t, reinterpret_cast<void*>(wrapper), 4);
    int32_t ownerState = static_cast<int32_t>(CSF_AT(uint32_t, reinterpret_cast<void*>(infoBlock), 12));
    uint32_t handleAddress = infoBlock + 4;

    int32_t result = static_cast<int32_t>(handleAddress);

    if (ownerState >= 0)
    {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(handleAddress));
        C_008342e8* control = reinterpret_cast<C_008342e8*>(result);

        if (control != nullptr)
        {
            int controlFlags = static_cast<int>(CSF_AT(uint32_t, reinterpret_cast<void*>(control), 0x50));
            if (static_cast<uint8_t>(IsControlReadyFlagSet(controlFlags)) != 0)
            {
                uint32_t argTwo = a3[1];
                uint32_t keyframeHolder = CSF_AT(uint32_t, reinterpret_cast<void*>(argTwo), 4);
                float keyframeValue = CSF_AT(float, reinterpret_cast<void*>(keyframeHolder), 4);

                int64_t truncatedValue = static_cast<int64_t>(keyframeValue);
                uint32_t index = static_cast<uint32_t>(truncatedValue);
                result = static_cast<int32_t>(truncatedValue);

                if (index < static_cast<uint32_t>(control->field_01a0))
                {
                    uint32_t target = CSF_AT(uint32_t, reinterpret_cast<void*>(a1), 4);
                    uint32_t entry = static_cast<uint32_t>(
                        GetControlEntryByIndex(control, nullptr, static_cast<int>(index)));

                    CSF_AT(uint32_t, reinterpret_cast<void*>(target), 8) =
                        CSF_AT(uint32_t, reinterpret_cast<void*>(entry), 4);
                    result = static_cast<int32_t>(CSF_AT(uint32_t, reinterpret_cast<void*>(entry), 8));
                    CSF_AT(uint32_t, reinterpret_cast<void*>(target), 12) = static_cast<uint32_t>(result);
                }
            }
        }
    }

    return result;
}

HOOK(0x0045BE80, CopyTableEntryFromHandleIndex, "auto");
