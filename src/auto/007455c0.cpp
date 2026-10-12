#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_FUNC(void*, __fastcall, NetPlayerDdl_ReadByte, 0x00655E20,
                (void* self, void* edx, char* a2));
static CSF_FUNC(int, __fastcall, NetPlayerDdl_ReadShort, 0x00655E40,
                (void* self, void* edx, char* a2));
static CSF_FUNC(uint32_t*, __fastcall, NetCogibleDdl_ReadIntoSlot, 0x00655F00,
                (void* self, void* edx, int a2));

static int __fastcall C_008622e8_ReadPlayerData(
    C_008622e8* self, void* /*edx*/, C_008622e8* reader)
{
    char* data = reinterpret_cast<char*>(self);
    NetPlayerDdl_ReadByte(reader, nullptr, data);
    for (int i = 0; i < 256; ++i)
        NetPlayerDdl_ReadByte(reader, nullptr, data + i + 1);

    NetCogibleDdl_ReadIntoSlot(
        reader, nullptr, reinterpret_cast<int>(data + 260));
    NetCogibleDdl_ReadIntoSlot(
        reader, nullptr, reinterpret_cast<int>(data + 264));

    char* slot = data + 268;
    int remaining = 128;
    int result;
    do
    {
        result = NetPlayerDdl_ReadShort(reader, nullptr, slot);
        slot += 2;
        --remaining;
    } while (remaining);
    return result;
}

HOOK(0x007455C0, C_008622e8_ReadPlayerData, "auto");
