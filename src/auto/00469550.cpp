#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, MissionScript_GrowEventRecords, 0x00413570,
         (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, MissionScript_DispatchToScript, 0x00468310,
         (void* self, void* edx, uint32_t* a2));
CSF_FUNC(unsigned int, __fastcall, CMissionScript_HandleEventSource, 0x00468A90,
         (void* self, void* edx, int a2));
CSF_FUNC(unsigned int, __fastcall, CMissionScript_CollectEventScripts, 0x00468D40,
         (void* self, void* edx, void* a2, int a3));
CSF_FUNC(unsigned int, __fastcall, CMissionScript_HandleEvent19, 0x00469190,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

[[maybe_unused]] static const auto g_EventRecordArrayVtable =
    CSF_VTBL(0x00413570);

static int __fastcall CMissionScript_DispatchEvent(
    C_008314f8* self, void* /*edx*/, uint32_t* event)
{
    if (CSF_AT(uint8_t, self, 0xAC) && event[1] == 1)
        CMissionScript_HandleEventSource(self, nullptr,
                                        static_cast<int>(event[9]));

    // Include the final initialized owner word in the callee-visible buffer.
    uint32_t scripts[5] = {
        0, 0, 0, 10, reinterpret_cast<uint32_t>(self)
    };
    CMissionScript_CollectEventScripts(
        self, nullptr, scripts, static_cast<int>(event[1]));

    const uint32_t scriptCount = scripts[2];
    if (scriptCount)
    {
        for (uint32_t index = 0; index < scriptCount; ++index)
        {
            auto entries = reinterpret_cast<void**>(scripts[0]);
            MissionScript_DispatchToScript(entries[index], nullptr, event);
        }
        self = reinterpret_cast<C_008314f8*>(scripts[4]);
    }

    if (event[1] == 8)
    {
        auto records = reinterpret_cast<uint8_t*>(self) + 0x84;
        if (CSF_AT(uint32_t, self, 0x8C) >=
            CSF_AT(uint32_t, self, 0x88))
        {
            MissionScript_GrowEventRecords(
                records, nullptr, CSF_AT(int, records, 0x0C));
        }

        const uint32_t index = CSF_AT(uint32_t, records, 0x08);
        auto data = CSF_AT(uint8_t*, records, 0x00);
        CSF_AT(uint32_t, records, 0x08) = index + 1;
        self = reinterpret_cast<C_008314f8*>(scripts[4]);

        auto entry = data + 12 * index;
        CSF_AT(uint32_t, entry, 0x04) = event[3];
        CSF_AT(uint32_t, entry, 0x08) = event[4];
    }

    if (event[1] == 19)
    {
        CMissionScript_HandleEvent19(
            self, nullptr, reinterpret_cast<int>(event + 2));
    }

    if (scripts[0])
        return Mem_Free(reinterpret_cast<void*>(scripts[0]));
    return 0;
}

HOOK(0x00469550, CMissionScript_DispatchEvent, "auto");
