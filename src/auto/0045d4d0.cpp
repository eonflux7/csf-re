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

// Callees (bound by address)
CSF_FUNC(char, __fastcall, Sub467CC0, 0x00467CC0, (void* self, void* edx, unsigned int a2));
CSF_FUNC(int, __fastcall, Sub468750, 0x00468750, (void* self, void* edx, int a2));

// Globals
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Vtable references from the original call sites
[[maybe_unused]] static const void* const kSceneLookupVtbl = CSF_VTBL(0x00468750);
[[maybe_unused]] static const void* const kSceneEntryVtbl = CSF_VTBL(0x00467CC0);

static int __fastcall ResolveSceneEntryAndArm(void* self, void* /*edx*/, int a2, int a3, void* a4)
{
    (void)a2;
    (void)a3;

    // Scene child at g_pCurrentScene->[+0xBC]
    void* sceneChild = CSF_AT(void*, g_pCurrentScene, 0xBC);

    // Key: **(a4)->[+4]->[+4]
    void* first = *static_cast<void**>(a4);
    void* second = CSF_AT(void*, first, 4);
    int lookupKey = static_cast<int>(reinterpret_cast<uintptr_t>(CSF_AT(void*, second, 4)));

    int result = Sub468750(sceneChild, nullptr, lookupKey);
    void* entry = reinterpret_cast<void*>(static_cast<uintptr_t>(result));

    if (entry != nullptr)
    {
        result = CSF_AT(int, entry, 0x18);
        if (result == 0)
        {
            Sub467CC0(entry, nullptr, 0);
            result = CSF_AT(int, entry, 0x18);
            if (result != 0)
            {
                CSF_AT(void*, self, 0x70) = entry;
                CSF_AT(int, self, 0x18) = 3;
                CSF_AT(uint8_t, self, 0x82) = 0;
            }
        }
    }

    return result;
}

HOOK(0x0045D4D0, ResolveSceneEntryAndArm, "auto");
