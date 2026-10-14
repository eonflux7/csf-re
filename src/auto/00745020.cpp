#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, NetCogibleDdl_ReadByte, 0x00655E20,
         (void* self, void* edx, char* a2));
CSF_FUNC(uint32_t*, __fastcall, NetCogibleDdl_ReadIntoSlot, 0x00655F00,
         (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, C_00858180_EnqueueNetRecord, 0x00741EB0,
         (C_00858180* self, void* , int a2, int a3, int a4, int a5, int a6, int a7, int a8, char a9, int a10));

static char __fastcall C_00858180_ReadAndEnqueueNetRecord(
    C_00858180* self, void* /*edx*/, C_008622e8* reader)
{
    // No initialisers: the original reserves these slots with plain `sub esp, 0x20` and no stores. The tenth value
    // goes into the reader parameter's own slot, as the original does (`lea edx, [ebp+8]` before the last read).
    int record1;
    int record2;
    int record3;
    int record4;
    int record5;
    int record6;
    int record7;
    char recordByte[4];

    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&record1));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&record2));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&record3));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&record4));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&record5));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&record6));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&record7));
    NetCogibleDdl_ReadByte(reader, nullptr, recordByte);
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&reader));

    return C_00858180_EnqueueNetRecord(
        self, nullptr, record1, record2, record3, record4,
        record5, record6, record7, recordByte[0], reinterpret_cast<int>(reader));
}

HOOK(0x00745020, C_00858180_ReadAndEnqueueNetRecord, "auto");
