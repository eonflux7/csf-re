#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void*, __fastcall, FindOrCreateConfigEntry, 0x0064AD60, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, AddConfigEntry, 0x0064CEB0, (void* self, void* edx, char a2, void* a3));
CSF_FUNC(void*, __fastcall, WriteConfigFlag, 0x00656380, (void* self, void* edx, char a2));
CSF_FUNC(int, __cdecl, HashTemporaryString, 0x0066B590, (std::uint32_t tempObjectWord));

static char __fastcall C_00858180_SetBombActivated(
    C_00858180* self, void* /*edx*/, char value)
{
    alignas(8) std::uint8_t temporaryStringBytes[4];
    *reinterpret_cast<std::uint32_t*>(temporaryStringBytes) =
        reinterpret_cast<std::uint32_t>(self);

    InitializeTemporaryString(
        temporaryStringBytes, nullptr,
        reinterpret_cast<std::int32_t>(
            CSF_STR(0x008681D8, "SetBombaActivada")));

    int key = HashTemporaryString(
        *reinterpret_cast<std::uint32_t*>(temporaryStringBytes));
    void* entry = FindOrCreateConfigEntry(
        self, nullptr, reinterpret_cast<int>(&key));
    WriteConfigFlag(entry, nullptr, value);
    return AddConfigEntry(self, nullptr, 0, entry);
}

HOOK(0x0074B730, C_00858180_SetBombActivated, "auto");
