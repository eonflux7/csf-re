#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, sub_452230, 0x00452230, (void* self, void* edx, int a2));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __stdcall DispatchSceneLookupFromArg(int /*a1*/, int /*a2*/, int a3)
{
    void* argPtr = reinterpret_cast<void*>(a3);

    // a3 points at a pointer; the pointed-to object holds a sub-record at +4.
    uint32_t recordPtr = CSF_AT(uint32_t, argPtr, 0);
    uint32_t v3 = CSF_AT(uint32_t, reinterpret_cast<void*>(recordPtr), 4);

    uint32_t countOrKind = CSF_AT(uint32_t, reinterpret_cast<void*>(v3), 0x1C);
    uint32_t v5 = v3 + 4;

    if (countOrKind < 0x10u) {
        // Small value: pass the address of the embedded field at v5 + 4.
        return sub_452230(g_pCurrentScene, nullptr, static_cast<int>(v5 + 4));
    }

    // Large value: pass the value stored at v5 + 4.
    uint32_t storedValue = CSF_AT(uint32_t, reinterpret_cast<void*>(v5), 4);
    return sub_452230(g_pCurrentScene, nullptr, static_cast<int>(storedValue));
}

HOOK(0x00460890, DispatchSceneLookupFromArg, "auto");
