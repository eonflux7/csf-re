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

CSF_FUNC(unsigned int, __fastcall, sub_473E00, 0x00473E00, (void* self, void* edx, int a2));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Original is __stdcall ret 12 but reads ecx as an implicit input (owner object).
// Modeled as __fastcall with 3 stack args: same stack layout, same callee pop (12).
static int __fastcall DispatchWithFlagTableEntry(void* self, void* /*edx*/, int a1, int a2, int a3)
{
    (void)a1;
    (void)a2;

    void* argPtr = reinterpret_cast<void*>(static_cast<uintptr_t>(a3));
    void* header = *reinterpret_cast<void**>(argPtr);
    uint32_t flags = CSF_AT(uint32_t, header, 8);

    void* owner = CSF_AT(void*, self, 0x20);
    if (flags & 0x800000u)
    {
        void* scene = g_pCurrentScene;
        void* sceneSub = CSF_AT(void*, scene, 0xBC);
        owner = CSF_AT(void*, sceneSub, 0x38);
        flags &= 0xFF7FFFFFu;
    }

    uint32_t targetArg = CSF_AT(uint32_t, argPtr, 4);
    void* table = CSF_AT(void*, owner, 8);
    uint32_t index = flags * 3u;
    void* entry = CSF_AT(void*, table, index * 4u + 4u);

    return static_cast<int>(sub_473E00(entry, nullptr, static_cast<int>(targetArg)));
}

HOOK(0x00464580, DispatchWithFlagTableEntry, "auto");
