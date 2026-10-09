#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(double, __stdcall, Hud_ComputeEntryMetric, 0x0042DDD0, (float* a1));
CSF_FUNC(int, __fastcall, EntryArray_Reserve, 0x0042E310, (void* self, void* edx, int a2));

static int __fastcall CHud_AppendEntryForActiveRecord(CHud* self, void* /*edx*/, int eventRecord)
{
    uint8_t* record = reinterpret_cast<uint8_t*>(eventRecord);
    int result = CSF_AT(int, record, 0x1c);
    if (result >= 0)
    {
        int resolvedOwner = CHandleTable_Resolve(g_pCurrentScene, nullptr, eventRecord + 0x14);
        result = static_cast<int>(GetSceneMenuContext(g_pCurrentScene, nullptr));
        if (resolvedOwner == result)
        {
            int entryTag = CSF_AT(int, reinterpret_cast<uint8_t*>(GetGlobalValue_8BC1E0()), 6352);
            float entryMetric = static_cast<float>(Hud_ComputeEntryMetric(reinterpret_cast<float*>(record + 0x4c)));

            uint8_t* entryList = reinterpret_cast<uint8_t*>(self) + 0x22c;
            if (self->field_0234 >= self->field_0230)
                EntryArray_Reserve(entryList, nullptr, CSF_AT(int, entryList, 0xC));

            // Original order: read count, read entry-array base, then store count + 1.
            uint32_t entryIndex = self->field_0234;
            uint32_t entryBase = CSF_AT(uint32_t, self, 0x22c);
            uint8_t* entry = reinterpret_cast<uint8_t*>(entryBase + entryIndex * 16);
            self->field_0234 = entryIndex + 1;
            result = static_cast<int>(reinterpret_cast<uintptr_t>(entry));

            CSF_AT(float, entry, 0x0) = entryMetric;
            CSF_AT(int, entry, 0x4) = entryTag;
            CSF_AT(int, entry, 0x8) = 0;
            CSF_AT(float, entry, 0xC) = 255.0f;
        }
    }
    return result;
}

HOOK(0x00431970, CHud_AppendEntryForActiveRecord, "auto");
