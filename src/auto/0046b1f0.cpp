#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, MissionScript_ResetEntryRange, 0x00469710, (int a1, int a2));
CSF_FUNC(int, __fastcall, MissionScript_ClearResourceCollection, 0x00470D10, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, MissionScript_DestroyScript, 0x00471130, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall CMissionScript_Clear(CMissionScript* self, void* /*edx*/)
{
    for (uint32_t index = 0; index < CSF_AT(uint32_t, self, 0x20); ++index)
    {
        void* script = CSF_AT(void**, self, 0x18)[index];
        if (script)
        {
            MissionScript_DestroyScript(script, nullptr);
            Mem_Free(script);
            CSF_AT(void**, self, 0x18)[index] = nullptr;
        }
    }

    void* scripts = CSF_AT(void*, self, 0x18);
    CSF_AT(uint32_t, self, 0x1C) = 0;
    CSF_AT(uint32_t, self, 0x20) = 0;
    if (scripts)
    {
        Mem_Free(scripts);
        CSF_AT(void*, self, 0x18) = nullptr;
    }

    int result = MissionScript_ClearResourceCollection(
        reinterpret_cast<uint8_t*>(self) + 0x40, nullptr);

    void* entries = CSF_AT(void*, self, 0x3C);
    if (entries)
    {
        int begin = CSF_AT(int, entries, 0x08);
        if (begin)
        {
            MissionScript_ResetEntryRange(begin, CSF_AT(int, entries, 0x0C));
            if (CSF_AT(void*, entries, 0x08))
                Mem_FreeDebug(CSF_AT(void*, entries, 0x08));
        }

        CSF_AT(uint32_t, entries, 0x08) = 0;
        CSF_AT(uint32_t, entries, 0x0C) = 0;
        CSF_AT(uint32_t, entries, 0x10) = 0;
        result = Mem_Free(entries);
        CSF_AT(void*, self, 0x3C) = nullptr;
    }

    return result;
}

HOOK(0x0046B1F0, CMissionScript_Clear, "auto");
