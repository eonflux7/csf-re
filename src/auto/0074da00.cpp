#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, LookupObjectByIndex, 0x0064AD00, (int a1));
CSF_FUNC(void, __stdcall, NetGameDdl_FinalizeSpawnUpdate, 0x0064AF90, (int a1, int a2));
CSF_FUNC(void**, __fastcall, NetCogibleDdl_FindProperty, 0x006622F0, (void* self, void* edx, void** a2, uint8_t* a3));
CSF_FUNC(char, __fastcall, NetPlayerDdl_ApplyProperty, 0x0066CD80, (void* self, void* edx, int arg0, int a3, int a4, int a5, void* a6));

static int __fastcall NetGameDdl_ApplyPlayerSpawnProperties(
    char* self, void* /*edx*/, C_008622e8* update, int mode)
{
    static const int propertyOffsets[] = {
        68, 80, 92, 95, 100, 696, 708,
        714, 720, 726, 732, 744, 768
    };

    alignas(8) uint8_t propertyResultBytes[4];
    alignas(8) uint8_t propertyIdBytes[4];

    for (int propertyId = 1; propertyId <= 13; ++propertyId)
    {
        const int objectIndex =
            static_cast<int>(CSF_AT(uint32_t, self, 0x2C) >> 22);
        propertyIdBytes[0] = static_cast<uint8_t>(propertyId);

        const int object = LookupObjectByIndex(objectIndex);
        NetCogibleDdl_FindProperty(
            reinterpret_cast<uint8_t*>(object) + 0xC,
            nullptr,
            reinterpret_cast<void**>(propertyResultBytes),
            propertyIdBytes);

        void* property = CSF_AT(void*, propertyResultBytes, 0);
        void* handler = CSF_AT(void*, property, 0x10);

        NetPlayerDdl_ApplyProperty(
            handler,
            nullptr,
            reinterpret_cast<int>(self),
            reinterpret_cast<int>(self + propertyOffsets[propertyId - 1]),
            propertyId,
            mode,
            update);
    }

    NetGameDdl_FinalizeSpawnUpdate(reinterpret_cast<int>(update), mode);
    // The final callee's supplied binding has no defined return value.
    return 0;
}

HOOK(0x0074DA00, NetGameDdl_ApplyPlayerSpawnProperties, "auto");
