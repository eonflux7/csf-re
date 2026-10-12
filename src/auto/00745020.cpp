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
    int record1 = 0;
    int record2 = 0;
    int record3 = 0;
    int record4 = 0;
    int record5 = 0;
    int record6 = 0;
    int record7 = 0;
    char recordByte[4] = {};
    int record9 = 0;

    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&record1));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&record2));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&record3));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&record4));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&record5));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&record6));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&record7));
    NetCogibleDdl_ReadByte(reader, nullptr, recordByte);
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&record9));

    return C_00858180_EnqueueNetRecord(
        self, nullptr, record1, record2, record3, record4,
        record5, record6, record7, recordByte[0], record9);
}

HOOK(0x00745020, C_00858180_ReadAndEnqueueNetRecord, "auto");
