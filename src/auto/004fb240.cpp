#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, DevConsole_GrowCommandArray, 0x004FAAB0, (void* self, void* edx, int a2));
CSF_FUNC(unsigned int, __cdecl, CopyTrucoStringToWide, 0x00695590, (const char* src, uint16_t* dst, int maxCount));
CSF_FUNC(int, __cdecl, MenuExtras_ParseDescriptionId, 0x00698D90, (const char* text));
CSF_FUNC(void*, __fastcall, C_00830438_Construct, 0x00712CC0, (void* self, void* edx));

CSF_GLOBAL(int, g_globalValue_8B0560, 0x008B0560);

static void DevConsole_AppendCommand(C_00830438* self, const uint8_t* command)
{
    if (CSF_AT(uint32_t, self, 0x150) >= CSF_AT(uint32_t, self, 0x14C))
    {
        DevConsole_GrowCommandArray(
            reinterpret_cast<uint8_t*>(self) + 0x148,
            nullptr,
            CSF_AT(int, self, 0x154));
    }

    const uint32_t count = CSF_AT(uint32_t, self, 0x150);
    const uint32_t destination =
        CSF_AT(uint32_t, self, 0x148) + 312 * count;
    CSF_AT(uint32_t, self, 0x150) = count + 1;
    std::memcpy(reinterpret_cast<void*>(destination), command, 0x138);
}

static int __fastcall C_00830438_ConstructDevConsole(
    C_00830438* self, void* /*edx*/)
{
    C_00830438_Construct(self, nullptr);
    self->vtbl = CSF_VTBL(0x0083F2F0);

    CSF_AT(uint32_t, self, 0x148) = 0;
    CSF_AT(uint32_t, self, 0x14C) = 0;
    CSF_AT(uint32_t, self, 0x150) = 0;
    CSF_AT(int, self, 0x154) = 10;
    CSF_AT(uint32_t, self, 0x16C) = 0;
    CSF_AT(uint32_t, self, 0x170) = 0;
    CSF_AT(uint32_t, self, 0x174) = 0;
    CSF_AT(int, self, 0x178) = 10;
    CSF_AT(uint32_t, self, 0x17C) = 0;
    CSF_AT(uint32_t, self, 0x180) = 0;
    CSF_AT(uint32_t, self, 0x184) = 0;
    CSF_AT(int, self, 0x188) = 10;
    CSF_AT(uint32_t, self, 0x194) = 0;
    CSF_AT(uint32_t, self, 0x198) = 0;
    CSF_AT(uint32_t, self, 0x19C) = 0;
    CSF_AT(int, self, 0x1A0) = 10;
    CSF_AT(uint32_t, self, 0x1C0) = 0;
    CSF_AT(int, self, 0x1C4) = 15;
    CSF_AT(uint8_t, self, 0x1B0) = 0;
    g_globalValue_8B0560 = 0;
    CSF_AT(uint32_t, self, 0x18C) = 0;
    CSF_AT(uint32_t, self, 0x190) = 0;
    CSF_AT(uint8_t, self, 0x10) = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;
    CSF_AT(uint16_t, self, 0x18) = 0;
    CSF_AT(uint32_t, self, 0x144) = 0;
    CSF_AT(uint8_t, self, 0x158) = 0;
    CSF_AT(uint32_t, self, 0x160) = 0;
    CSF_AT(uint32_t, self, 0x164) = 0;
    CSF_AT(uint32_t, self, 0x168) = 0;
    CSF_AT(uint32_t, self, 0x174) = 0;
    CSF_AT(uint32_t, self, 0x184) = 0;
    CSF_AT(uint32_t, self, 0x150) = 0;

    alignas(8) uint8_t commandBytes[0x138];
    CSF_AT(int, commandBytes, 0) = 0;
    CSF_AT(int, self, 0x15C) = 10;
    CSF_AT(int, self, 0x1A4) = -1;
    CSF_AT(int, self, 0x1A8) = -1;
    CSF_AT(float, self, 0x1C8) = 1.0f;

    // The packet does not provide the text stored at this address.
    CopyTrucoStringToWide(
        reinterpret_cast<const char*>(CSF_ADDR_DATA(0x0083F2EC)),
        reinterpret_cast<uint16_t*>(commandBytes + 4), 150);
    CSF_AT(int, commandBytes, 0x130) =
        MenuExtras_ParseDescriptionId(CSF_STR(0x0083F2E4, "CO08"));
    CSF_AT(uint32_t, commandBytes, 0x134) =
        reinterpret_cast<uint32_t>(CSF_ADDR_FN(0x004FB040));
    DevConsole_AppendCommand(self, commandBytes);

    CSF_AT(int, commandBytes, 0) = 1;
    CopyTrucoStringToWide(
        CSF_STR(0x0083F2E0, "GOD"),
        reinterpret_cast<uint16_t*>(commandBytes + 4), 150);
    CSF_AT(int, commandBytes, 0x130) =
        MenuExtras_ParseDescriptionId(CSF_STR(0x0083F2D8, "CO09"));
    CSF_AT(uint32_t, commandBytes, 0x134) =
        reinterpret_cast<uint32_t>(CSF_ADDR_FN(0x004FB110));
    DevConsole_AppendCommand(self, commandBytes);

    CSF_AT(int, commandBytes, 0) = 2;
    CopyTrucoStringToWide(
        CSF_STR(0x0083F2D0, "HELP"),
        reinterpret_cast<uint16_t*>(commandBytes + 4), 150);
    CSF_AT(int, commandBytes, 0x130) =
        MenuExtras_ParseDescriptionId(CSF_STR(0x0083F2C8, "CO11"));
    CSF_AT(uint32_t, commandBytes, 0x134) =
        reinterpret_cast<uint32_t>(CSF_ADDR_FN(0x004FB1A0));
    DevConsole_AppendCommand(self, commandBytes);

    return reinterpret_cast<int>(self);
}

HOOK(0x004FB240, C_00830438_ConstructDevConsole, "auto");
