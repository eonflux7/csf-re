#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, LookupObjectByIndex, 0x0064AD00, (int a1));
CSF_FUNC(void, __stdcall, FinalizeSpawnUpdate, 0x0064AF90, (int a1, int a2));
CSF_FUNC(void**, __fastcall, NetCogibleDdl_FindProperty, 0x006622F0, (void* self, void* edx, void** a2, uint8_t* a3));
CSF_FUNC(char, __fastcall, NetPlayerDdl_ApplyProperty, 0x0066CD80, (void* self, void* edx, int arg0, int a3, int a4, int a5, void* a6));

static int __fastcall NetPlayerListDdl_ApplySpawnProperties(
    void* self, void* /*edx*/, C_008622e8* update, int context)
{
    alignas(8) uint8_t firstPropertyBytes[0x4];
    alignas(8) uint8_t firstKeyBytes[0x4];
    alignas(8) uint8_t subsequentPropertyBytes[0x4];
    alignas(8) uint8_t subsequentKeyBytes[0x4];

    auto playerBytes = static_cast<uint8_t*>(self);
    auto firstPropertyOut = reinterpret_cast<void**>(firstPropertyBytes);
    auto subsequentPropertyOut =
        reinterpret_cast<void**>(subsequentPropertyBytes);

    CSF_AT(int, subsequentPropertyBytes, 0) = context;
    CSF_AT(C_008622e8*, subsequentKeyBytes, 0) = update;

    firstKeyBytes[0] = 1;
    int object = LookupObjectByIndex(
        static_cast<int>(CSF_AT(uint32_t, self, 0x2C) >> 22));
    NetCogibleDdl_FindProperty(
        reinterpret_cast<uint8_t*>(object) + 0xC,
        nullptr, firstPropertyOut, firstKeyBytes);
    NetPlayerDdl_ApplyProperty(
        CSF_AT(void*, CSF_AT(void*, firstPropertyBytes, 0), 0x10),
        nullptr, reinterpret_cast<int>(self),
        reinterpret_cast<int>(playerBytes + 0x44),
        1, context, update);

    subsequentKeyBytes[3] = 2;
    object = LookupObjectByIndex(
        static_cast<int>(CSF_AT(uint32_t, self, 0x2C) >> 22));
    NetCogibleDdl_FindProperty(
        reinterpret_cast<uint8_t*>(object) + 0xC,
        nullptr, subsequentPropertyOut, subsequentKeyBytes + 3);
    NetPlayerDdl_ApplyProperty(
        CSF_AT(void*, CSF_AT(void*, subsequentPropertyBytes, 0), 0x10),
        nullptr, reinterpret_cast<int>(self),
        reinterpret_cast<int>(playerBytes + 0x58),
        2, context, update);

    subsequentKeyBytes[3] = 3;
    object = LookupObjectByIndex(
        static_cast<int>(CSF_AT(uint32_t, self, 0x2C) >> 22));
    NetCogibleDdl_FindProperty(
        reinterpret_cast<uint8_t*>(object) + 0xC,
        nullptr, subsequentPropertyOut, subsequentKeyBytes + 3);
    char result = NetPlayerDdl_ApplyProperty(
        CSF_AT(void*, CSF_AT(void*, subsequentPropertyBytes, 0), 0x10),
        nullptr, reinterpret_cast<int>(self),
        reinterpret_cast<int>(playerBytes + 0xA8),
        3, context, update);

    FinalizeSpawnUpdate(reinterpret_cast<int>(update), context);
    return result;
}

HOOK(0x0074EEB0, NetPlayerListDdl_ApplySpawnProperties, "auto");
