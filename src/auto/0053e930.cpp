#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callees, bound by address.
CSF_FUNC(uint8_t, __cdecl, IsGameStateReady, 0x0044FB00, ());
CSF_FUNC(void*, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(void*, __fastcall, GetGlobalDefaultEntry, 0x004FE4A0, (void* self, void* edx));
CSF_FUNC(int, __stdcall, CMenuMissionInfo_AllListedEntriesAreFive, 0x0053E1C0, (int a1));

// Refreshes the cached "all listed entries are five" flag of a mission-info panel
// (self) for the target object a2, and returns the resulting low byte of the flag.
static char __fastcall CMenuMissionInfo_RefreshAllFiveFlag(CMenuMissionInfo* self, void* /*edx*/, int a2)
{
    if (self->field_0090 >= self->field_008c)
    {
        return static_cast<char>(self->field_0090);
    }

    uint8_t gameStateReady = IsGameStateReady();
    if (gameStateReady == 0)
    {
        return static_cast<char>(gameStateReady);
    }

    void* globalObject = GetGlobalValue_8B0568();
    void* firstEntry = GetGlobalDefaultEntry(globalObject, nullptr);
    if (firstEntry == nullptr)
    {
        return 0;
    }

    void* globalObjectAgain = GetGlobalValue_8B0568();
    void* secondEntry = GetGlobalDefaultEntry(globalObjectAgain, nullptr);
    if (CSF_AT(uint32_t, secondEntry, 0x1C) != 0)
    {
        return static_cast<char>(reinterpret_cast<uintptr_t>(secondEntry));
    }

    void* target = reinterpret_cast<void*>(static_cast<uintptr_t>(a2));
    uint8_t alreadyComputed = CSF_AT(uint8_t, target, 0x44);
    if (alreadyComputed != 0)
    {
        return static_cast<char>(alreadyComputed);
    }

    uint8_t allFive = static_cast<uint8_t>(CMenuMissionInfo_AllListedEntriesAreFive(a2));
    self->field_00a5 = allFive;
    CSF_AT(uint8_t, target, 0x44) = allFive;
    return static_cast<char>(allFive);
}

HOOK(0x0053E930, CMenuMissionInfo_RefreshAllFiveFlag, "auto");
