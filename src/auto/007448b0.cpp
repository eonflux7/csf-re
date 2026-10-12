#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void*, __fastcall, FindOrCreateConfigEntry, 0x0064AD60, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, AddConfigEntry, 0x0064CEB0, (void* self, void* edx, char a2, void* a3));
CSF_FUNC(int, __cdecl, HashTemporaryString, 0x0066B590, (std::uint32_t tempObjectWord));

static char __fastcall CMenuMpStats_SendResetCommand(CMenuMpStats* self, void* /*edx*/)
{
    alignas(8) std::uint8_t temporaryStringBytes[4];
    *reinterpret_cast<CMenuMpStats**>(temporaryStringBytes) = self;

    InitializeTemporaryString(
        temporaryStringBytes, nullptr,
        reinterpret_cast<std::int32_t>(CSF_STR(0x00862A48, "Reset")));

    int commandHash = HashTemporaryString(
        *reinterpret_cast<std::uint32_t*>(temporaryStringBytes));

    void* entry = FindOrCreateConfigEntry(
        self, nullptr,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(&commandHash)));

    return AddConfigEntry(self, nullptr, 1, entry);
}

HOOK(0x007448B0, CMenuMpStats_SendResetCommand, "auto");
