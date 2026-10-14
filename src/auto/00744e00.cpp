#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void*, __fastcall, FindOrCreateConfigEntry, 0x0064AD60, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, AddConfigEntry, 0x0064CEB0, (void* self, void* edx, char a2, void* a3));
CSF_FUNC(int, __fastcall, GameConfig_WriteDwordField, 0x006563F0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, HashTemporaryString, 0x0066B590, (std::uint32_t tempObjectWord));

static int __fastcall C_00858180_SetStoppedInterrogating(
    C_00858180* self, void* /*edx*/, int value)
{
    std::uint32_t temporaryStringWord = reinterpret_cast<std::uint32_t>(self);
    InitializeTemporaryString(
        &temporaryStringWord, nullptr,
        reinterpret_cast<std::int32_t>(
            CSF_STR(0x00867D24, "DejoDeInterrogarte")));

    int key = HashTemporaryString(
        temporaryStringWord);
    void* entry = FindOrCreateConfigEntry(
        self, nullptr, reinterpret_cast<int>(&key));
    GameConfig_WriteDwordField(entry, nullptr, value);
    return AddConfigEntry(self, nullptr, 0, entry);
}

HOOK(0x00744E00, C_00858180_SetStoppedInterrogating, "auto");
