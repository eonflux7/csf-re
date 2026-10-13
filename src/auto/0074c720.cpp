#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void*, __fastcall, FindOrCreateConfigEntry, 0x0064AD60, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, AddConfigEntry, 0x0064CEB0, (void* self, void* edx, char a2, void* a3));
CSF_FUNC(int, __cdecl, HashTemporaryString, 0x0066B590, (std::uint32_t tempObjectWord));

static char __fastcall C_00858180_ResetRespawnMemoriesConfig(
    C_00858180* self, void* /*edx*/)
{
    alignas(8) std::uint8_t temporaryBytes[4];
    *reinterpret_cast<std::uint32_t*>(temporaryBytes) =
        reinterpret_cast<std::uint32_t>(self);

    InitializeTemporaryString(temporaryBytes, nullptr,
        reinterpret_cast<std::int32_t>(
            CSF_STR(0x008682E8, "ReseteaMemoriasPuntosRespawn")));

    // The existing temporary word on the stack is the cdecl argument.
    alignas(8) std::uint8_t hashBytes[4];
    *reinterpret_cast<int*>(hashBytes) =
        HashTemporaryString(*reinterpret_cast<std::uint32_t*>(temporaryBytes));

    void* configEntry = FindOrCreateConfigEntry(self, nullptr,
        reinterpret_cast<int>(hashBytes));

    return AddConfigEntry(self, nullptr, 1, configEntry);
}

HOOK(0x0074C720, C_00858180_ResetRespawnMemoriesConfig, "auto");
