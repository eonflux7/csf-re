#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void*, __fastcall, FindOrCreateConfigEntry, 0x0064AD60, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, AddConfigEntry, 0x0064CEB0, (void* self, void* edx, char a2, void* a3));
CSF_FUNC(void*, __fastcall, WriteConfigFlag, 0x00656380, (void* self, void* edx, std::int32_t a2));
CSF_FUNC(void*, __fastcall, GameConfig_WriteByteField, 0x006563B0, (void* self, void* edx, std::int32_t a2));
CSF_FUNC(int, __fastcall, GameConfig_WriteDwordField, 0x006563F0, (void* self, void* edx, std::int32_t a2));
CSF_FUNC(void*, __fastcall, WriteConfigPair, 0x00656480, (void* self, void* edx, std::int32_t a2, int a3));
CSF_FUNC(int, __cdecl, HashTemporaryString, 0x0066B590, (std::int32_t strObj));

// The original loads every stack argument as a full dword (mov reg,[ebp+N]; push reg), so the
// parameters are 32-bit: a char would drop the high bits before the push.
static char __fastcall NetCogibleDDL_WriteContacto(
    C_00858180* self, void* /*edx*/,
    std::int32_t byteField,
    std::int32_t d0, std::int32_t d1, std::int32_t d2, std::int32_t d3, std::int32_t d4, std::int32_t d5, std::int32_t d6, std::int32_t d7,
    std::int32_t d8, std::int32_t d9, std::int32_t d10, std::int32_t d11, std::int32_t d12, std::int32_t d13, std::int32_t d14,
    std::int32_t flag, std::int32_t pairA, int pairB)
{
    alignas(4) uint8_t strBytes[4];
    int key;

    // Temporary object at [ebp-0x10]: its first dword is `this` (the original pushes ecx there
    // before the string init), and the hash below reads that dword back.
    *reinterpret_cast<C_00858180**>(strBytes) = self;
    InitializeTemporaryString(strBytes, nullptr, reinterpret_cast<std::int32_t>(CSF_STR(0x008679E0, "Contacto")));
    key = HashTemporaryString(CSF_AT(std::int32_t, strBytes, 0));

    void* entry = FindOrCreateConfigEntry(self, nullptr, reinterpret_cast<int>(&key));

    GameConfig_WriteByteField(entry, nullptr, byteField);
    GameConfig_WriteDwordField(entry, nullptr, d0);
    GameConfig_WriteDwordField(entry, nullptr, d1);
    GameConfig_WriteDwordField(entry, nullptr, d2);
    GameConfig_WriteDwordField(entry, nullptr, d3);
    GameConfig_WriteDwordField(entry, nullptr, d4);
    GameConfig_WriteDwordField(entry, nullptr, d5);
    GameConfig_WriteDwordField(entry, nullptr, d6);
    GameConfig_WriteDwordField(entry, nullptr, d7);
    GameConfig_WriteDwordField(entry, nullptr, d8);
    GameConfig_WriteDwordField(entry, nullptr, d9);
    GameConfig_WriteDwordField(entry, nullptr, d10);
    GameConfig_WriteDwordField(entry, nullptr, d11);
    GameConfig_WriteDwordField(entry, nullptr, d12);
    GameConfig_WriteDwordField(entry, nullptr, d13);
    GameConfig_WriteDwordField(entry, nullptr, d14);
    WriteConfigFlag(entry, nullptr, flag);
    WriteConfigPair(entry, nullptr, pairA, pairB);

    return AddConfigEntry(self, nullptr, 1, entry);
}

HOOK(0x00742430, NetCogibleDDL_WriteContacto, "auto");
