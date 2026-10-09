#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, MissionScript_ApplyEntryRecord, 0x00467BC0, (void* self, void* edx, int16_t* a2, C_008313f8* a3));
CSF_FUNC(char, __fastcall, Sub467CC0, 0x00467CC0, (void* self, void* edx, unsigned int a2));
CSF_FUNC(void*, __cdecl, GetGlobalBlock8AFDA0, 0x0046ED80, ());
CSF_FUNC(int, __stdcall, GetDwordField4, 0x0046ED90, (int a1));

[[maybe_unused]] static void* const S_00467cc0_Vtable = CSF_VTBL(0x00467CC0);

static char __fastcall MissionScript_ProcessEntryListUntilStop(S_00467cc0* self, void* /*edx*/)
{
    char continueFlag = 1;

    uint8_t* listBase = reinterpret_cast<uint8_t*>(self->field_0010);
    uint32_t beginAddr = CSF_AT(uint32_t, listBase, 0x1C);
    int entryCount = 0;
    if (beginAddr != 0)
        entryCount = static_cast<int32_t>(CSF_AT(uint32_t, listBase, 0x20) - beginAddr) >> 3;

    if (entryCount <= 0) {
        Sub467CC0(self, nullptr, 0u);
        return continueFlag;
    }

    int entryIndex = 0;
    while (continueFlag) {
        uint8_t* currentList = reinterpret_cast<uint8_t*>(self->field_0010);
        uint32_t entryAddr = CSF_AT(uint32_t, currentList, 0x1C) + 8u * static_cast<uint32_t>(entryIndex);
        int16_t* entry = reinterpret_cast<int16_t*>(static_cast<uintptr_t>(entryAddr));

        GetGlobalBlock8AFDA0();
        C_008313f8* entryData = reinterpret_cast<C_008313f8*>(GetDwordField4(static_cast<int>(reinterpret_cast<uintptr_t>(entry))));

        MissionScript_ApplyEntryRecord(self, nullptr, entry, entryData);

        uint8_t* flagsPtr = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(entryData->field_0004));
        continueFlag = static_cast<char>(CSF_AT(uint8_t, flagsPtr, 1));

        ++entryIndex;
        if (entryIndex >= entryCount) {
            if (!continueFlag)
                return continueFlag;
            Sub467CC0(self, nullptr, 0u);
            return continueFlag;
        }
    }

    return continueFlag;
}

HOOK(0x004681B0, MissionScript_ProcessEntryListUntilStop, "auto");
