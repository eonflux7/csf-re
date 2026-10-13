#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void*, __fastcall, FindOrCreateConfigEntry, 0x0064AD60, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, AddConfigEntry, 0x0064CEB0, (void* self, void* edx, char a2, void* a3));
CSF_FUNC(int, __fastcall, GameConfig_WriteDwordField, 0x006563F0, (void* self, void* edx, char a2));
CSF_FUNC(int, __cdecl, HashTemporaryString, 0x0066B590, (std::uint32_t tempObjectWord));

static char __fastcall C_00858180_SetStoppedManipulating(
    C_00858180* self, void* /*edx*/, char value)
{
    alignas(8) std::uint8_t temporaryStringBytes[4];

    // The original reserves this word with push ecx, initially storing self.
    auto& temporaryStringWord =
        *reinterpret_cast<std::uint32_t*>(temporaryStringBytes);
    temporaryStringWord = reinterpret_cast<std::uint32_t>(self);

    InitializeTemporaryString(
        temporaryStringBytes, nullptr,
        reinterpret_cast<std::int32_t>(
            CSF_STR(0x008681B0, "DejoDeManipularte")));

    int key = HashTemporaryString(temporaryStringWord);
    void* entry = FindOrCreateConfigEntry(
        self, nullptr, reinterpret_cast<int>(&key));
    GameConfig_WriteDwordField(entry, nullptr, value);
    return AddConfigEntry(self, nullptr, 0, entry);
}

HOOK(0x0074B690, C_00858180_SetStoppedManipulating, "auto");
