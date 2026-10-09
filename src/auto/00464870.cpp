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

CSF_FUNC(int, __fastcall, ProcessTableEntry, 0x00472940, (void* self, void* edx, int a2));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Original is __thiscall ret 12 (ecx = owner object, 3 stack args).
// Modeled as __fastcall with 3 stack args: same stack layout, same callee pop (12).
static int __fastcall DispatchEntryWithArg(void* self, void* /*edx*/, int a2, int a3, int a4)
{
    (void)a2;

    void* argPtr = reinterpret_cast<void*>(static_cast<uintptr_t>(a4));
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

    return ProcessTableEntry(entry, nullptr, static_cast<int>(targetArg));
}

HOOK(0x00464870, DispatchEntryWithArg, "auto");
