#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, AllocateEntryPayload, 0x004723D0, (int a1));
CSF_FUNC(void*, __fastcall, ProcessEntryPair, 0x00472B60, (void* self, void* edx, int a2));
CSF_FUNC(unsigned int, __fastcall, AppendEntryPointer, 0x00473B70, (void* self, void* edx, uint32_t* a3));
CSF_FUNC(int, __fastcall, MissionScript_ProcessSectionTable, 0x0069B460, (void* self, void* edx));

static int __fastcall MissionScript_CreateAndAppendEntry(void* self, void* /*edx*/, int entryData)
{
    const uint32_t begin = CSF_AT(uint32_t, self, 0x10);
    CSF_AT(int32_t, self, 0x08) = begin
        ? static_cast<int32_t>(CSF_AT(uint32_t, self, 0x14) - begin) >> 2
        : 0;

    C_008313f8* entry = reinterpret_cast<C_008313f8*>(
        MissionScript_ProcessSectionTable(
            reinterpret_cast<void*>(CSF_ADDR_DATA(0x008B02E8)), nullptr));
    if (entry)
    {
        entry->field_0004 = 0;
        *reinterpret_cast<void***>(entry) = CSF_VTBL(0x008313F8);
        entry->field_0008 = 0xFFFFFFFFu;
    }

    const int payloadSize = CSF_AT(int, self, 0x04);
    uint32_t entryPointer = reinterpret_cast<uint32_t>(entry);
    auto* payload = reinterpret_cast<uint8_t*>(AllocateEntryPayload(payloadSize));
    entry->field_0004 = reinterpret_cast<uint32_t>(payload);
    if (payload)
        *payload = static_cast<uint8_t>(payloadSize);

    ProcessEntryPair(entry, nullptr, entryData);
    return static_cast<int>(AppendEntryPointer(
        static_cast<uint8_t*>(self) + 0x0C, nullptr, &entryPointer));
}

HOOK(0x00473E00, MissionScript_CreateAndAppendEntry, "auto");
