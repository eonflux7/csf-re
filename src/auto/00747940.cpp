#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Helpers the original calls, bound by address.
CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(int, __cdecl, HashTemporaryString, 0x0066B590, (std::uint32_t tempObjectWord));
CSF_FUNC(void*, __fastcall, FindOrCreateConfigEntry, 0x0064AD60, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GameConfig_WriteDwordField, 0x006563F0, (void* self, void* edx, char a2));
CSF_FUNC(char, __fastcall, AddConfigEntry, 0x0064CEB0, (void* self, void* edx, char a2, void* a3));

// Builds a temporary string keyed on this object, hashes it, finds/creates the config entry,
// writes three config dwords into it, then registers the entry with AddConfigEntry(this, 0, entry).
// __thiscall (ret 0xC): ecx = this, three stack arguments.
static char __fastcall C_00858180_ApplyInteraction(C_00858180* self, void* /*edx*/, char a2, char a3, char a4)
{
    // Temporary string object on the stack; its first dword holds `this`.
    alignas(8) uint8_t tempStringBytes[0xC];
    *reinterpret_cast<C_00858180**>(tempStringBytes) = self;

    InitializeTemporaryString(tempStringBytes, nullptr, CSF_ADDR_DATA(0x8679EC));
    int configKey = HashTemporaryString(*reinterpret_cast<std::uint32_t*>(tempStringBytes));

    void* configEntry = FindOrCreateConfigEntry(self, nullptr, (int)&configKey);

    GameConfig_WriteDwordField(configEntry, nullptr, a2);
    GameConfig_WriteDwordField(configEntry, nullptr, a3);
    GameConfig_WriteDwordField(configEntry, nullptr, a4);

    return AddConfigEntry(self, nullptr, 0, configEntry);
}

HOOK(0x747940, C_00858180_ApplyInteraction, "auto");
