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

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __fastcall, IsPropsHandlePairReady, 0x006B8A30, (void* self, void* edx));
CSF_FUNC(int, __fastcall, RwEngineReleaseSlot, 0x006B96C0, (void* self, void* edx, int* a2));

CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);
CSF_GLOBAL(uint32_t, g_dwRwEngineSlotOffset, 0x008BB474);

static void __fastcall ReleaseObjectPropsRwHandlePair(S_006b8020* self, void* /*edx*/)
{
    int* handlePair = reinterpret_cast<int*>(self->field_0010);
    if (handlePair)
    {
        if (IsPropsHandlePairReady(reinterpret_cast<void*>(self->field_0010), nullptr))
        {
            RwEngineReleaseSlot(
                *reinterpret_cast<S_006b9130**>(reinterpret_cast<char*>(g_pRwEngineInstance) + g_dwRwEngineSlotOffset),
                nullptr,
                handlePair);
            RwEngineReleaseSlot(
                *reinterpret_cast<S_006b9130**>(reinterpret_cast<char*>(g_pRwEngineInstance) + g_dwRwEngineSlotOffset),
                nullptr,
                handlePair + 1);
            Mem_Free(handlePair);
            self->field_0010 = 0;
        }
    }
}

HOOK(0x006B97A0, ReleaseObjectPropsRwHandlePair, "auto");
