#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, NetCogibleDdl_ReadIntoSlot, 0x00655F00, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, C_00858180_QueueNetMessage, 0x00741E20, (C_00858180* self, void* , int a2, int a3, int a4, int a5, int a6, int a7, int a8));

static int __fastcall C_00858180_ReadAndQueueNetMessage(
    C_00858180* self, void* /*edx*/, C_008622e8* reader)
{
    int first = 0;
    int second = 0;
    int third = 0;
    int fourth = 0;
    int fifth = 0;
    int sixth = 0;
    int seventh = reinterpret_cast<int>(reader);

    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&first));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&second));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&third));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&fourth));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&fifth));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&sixth));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&seventh));

    return C_00858180_QueueNetMessage(
        self, nullptr, first, second, third, fourth, fifth, sixth, seventh);
}

HOOK(0x00744EE0, C_00858180_ReadAndQueueNetMessage, "auto");
