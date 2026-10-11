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

// Game heap free, bound by address (original calls 0x691110).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Releases one entry of the RW engine sub-object list (__fastcall, one stack arg, ret 4).
CSF_FUNC(int, __fastcall, RwEngine_ReleaseEntry, 0x006B96C0, (void* self, void* edx, int* a2));

// Data: RW engine instance pointer and the byte offset of its sub-object slot.
CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);
CSF_GLOBAL(int, g_dwRwEngineSubOffset, 0x008BB474);

static char __fastcall GfxObjectProps_ReleaseResources(S_006b8020* self, void* /*edx*/)
{
    int* entryBlock = reinterpret_cast<int*>(self->field_0010);

    CSF_AT(int, self, 0x00) = -1;
    CSF_AT(int, self, 0x08) = 0;

    if (entryBlock)
    {
        S_006b9130* engineSub = *reinterpret_cast<S_006b9130**>(
            static_cast<uint8_t*>(g_pRwEngineInstance) + g_dwRwEngineSubOffset);
        RwEngine_ReleaseEntry(engineSub, nullptr, entryBlock);

        engineSub = *reinterpret_cast<S_006b9130**>(
            static_cast<uint8_t*>(g_pRwEngineInstance) + g_dwRwEngineSubOffset);
        RwEngine_ReleaseEntry(engineSub, nullptr, entryBlock + 1);

        Mem_Free(entryBlock);
        self->field_0010 = 0;
    }

    int trailingBlock = CSF_AT(int, self, 0x14);
    if (trailingBlock)
    {
        // Original keeps only the low byte of Mem_Free's result (LOBYTE(v3)).
        trailingBlock = Mem_Free(reinterpret_cast<void*>(trailingBlock));
        CSF_AT(int, self, 0x14) = 0;
    }
    return static_cast<char>(trailingBlock);
}

HOOK(0x006B9930, GfxObjectProps_ReleaseResources, "auto");
