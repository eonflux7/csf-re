#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, Sub_64B250, 0x0064B250, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_74B200_SetFlagAndReturnTrue, 0x0074B200, (uint8_t* self, void*));

static char __fastcall MpSpawnPoints_ResetSpecialObject(uint8_t* self, void* /*edx*/)
{
    uint8_t* linkedObject =
        reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x14));

    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, linkedObject, 0)[0x6C / 4])(
            linkedObject, nullptr, 1);

    linkedObject =
        reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x14));
    CSF_AT(uint8_t, self, 0x40) = 0;
    CSF_AT(uint8_t, self, 0x5C) = 0;

    uint32_t statsLink = CSF_AT(uint32_t, linkedObject, 0x18);
    if (statsLink == 0)
        return 0;

    uint8_t* stats = reinterpret_cast<uint8_t*>(statsLink - 0x78);

    // Inline the continuation reached by the original tail jump.
    CSF_AT(uint8_t, stats, 0xBC) = 0;
    char result = static_cast<char>(Sub_64B250(stats, nullptr));
    if (result == 0)
        return result;

    uint8_t flags = CSF_AT(uint8_t, stats, 0x6F);
    CSF_AT(uint32_t, stats, 0xB8) = 0;
    CSF_AT(uint8_t, stats, 0x6F) = static_cast<uint8_t>(flags & 0xFD);
    CSF_AT(uint8_t, stats, 0x6C) = 0;

    C_74B200_SetFlagAndReturnTrue(stats + 0x6F, nullptr);
    return C_74B200_SetFlagAndReturnTrue(stats + 0x6C, nullptr);
}

HOOK(0x004D6AC0, MpSpawnPoints_ResetSpecialObject, "auto");
