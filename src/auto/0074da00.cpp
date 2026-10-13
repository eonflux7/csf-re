#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, LookupObjectByIndex, 0x0064AD00, (int a1));
CSF_FUNC(void, __stdcall, FinalizeSpawnUpdate, 0x0064AF90, (int a1, int a2));
CSF_FUNC(void**, __fastcall, NetCogibleDdl_FindProperty, 0x006622F0, (void* self, void* edx, void** a2, uint8_t* a3));
CSF_FUNC(char, __fastcall, NetPlayerDdl_ApplyProperty, 0x0066CD80, (void* self, void* edx, int arg0, int a3, int a4, int a5, void* a6));

static void __fastcall NetGameDdl_ApplyPlayerSpawnProperties(
    char* self, void* /*edx*/, C_008622e8* context, int update)
{
    alignas(8) uint8_t propertyBytes[0x4];
    alignas(8) uint8_t keyBytes[0x4];

    const int objectIndex =
        static_cast<int>(CSF_AT(uint32_t, self, 0x2C) >> 22);
    keyBytes[0] = 1;
    const int object = LookupObjectByIndex(objectIndex);
    NetCogibleDdl_FindProperty(
        reinterpret_cast<uint8_t*>(object) + 0xC,
        nullptr, reinterpret_cast<void**>(propertyBytes), keyBytes);

    C_008622e8* const savedContext = context;
    const int savedUpdate = update;
    NetPlayerDdl_ApplyProperty(
        CSF_AT(void*, CSF_AT(void*, propertyBytes, 0), 0x10),
        nullptr, reinterpret_cast<int>(self),
        reinterpret_cast<int>(self + 68), 1, savedUpdate, savedContext);

#define APPLY_SPAWN_PROPERTY(ID, OFFSET)                                      \
    do {                                                                     \
        const int index =                                                    \
            static_cast<int>(CSF_AT(uint32_t, self, 0x2C) >> 22);              \
        reinterpret_cast<uint8_t*>(&context)[3] = ID;                        \
        const int owner = LookupObjectByIndex(index);                        \
        NetCogibleDdl_FindProperty(                                           \
            reinterpret_cast<uint8_t*>(owner) + 0xC, nullptr,                 \
            reinterpret_cast<void**>(&update),                               \
            reinterpret_cast<uint8_t*>(&context) + 3);                       \
        NetPlayerDdl_ApplyProperty(                                           \
            CSF_AT(void*, reinterpret_cast<void*>(update), 0x10),             \
            nullptr, reinterpret_cast<int>(self),                            \
            reinterpret_cast<int>(self + OFFSET), ID,                         \
            savedUpdate, savedContext);                                      \
    } while (false)

    APPLY_SPAWN_PROPERTY(2, 80);
    APPLY_SPAWN_PROPERTY(3, 92);
    APPLY_SPAWN_PROPERTY(4, 95);
    APPLY_SPAWN_PROPERTY(5, 100);
    APPLY_SPAWN_PROPERTY(6, 696);
    APPLY_SPAWN_PROPERTY(7, 708);
    APPLY_SPAWN_PROPERTY(8, 714);
    APPLY_SPAWN_PROPERTY(9, 720);
    APPLY_SPAWN_PROPERTY(10, 726);
    APPLY_SPAWN_PROPERTY(11, 732);
    APPLY_SPAWN_PROPERTY(12, 744);
    APPLY_SPAWN_PROPERTY(13, 768);

#undef APPLY_SPAWN_PROPERTY

    FinalizeSpawnUpdate(reinterpret_cast<int>(savedContext), savedUpdate);
}

HOOK(0x0074DA00, NetGameDdl_ApplyPlayerSpawnProperties, "auto");
