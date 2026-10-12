#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, NetPlayerDdl_ReadEventByte, 0x00655E20, (void* self, void* edx, char* a2));
CSF_FUNC(uint32_t*, __fastcall, NetCogibleDdl_ReadIntoSlot, 0x00655F00, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_00858180_EnqueueDdlEventRecord, 0x00741F40, (C_00858180* self, void* , int arg0, int a3, char a2, int a5));

static int __fastcall C_00858180_ReadAndEnqueueDdlEvent(
    C_00858180* self, void* /*edx*/, C_008622e8* arg0)
{
    C_008622e8* source = arg0;
    int firstValue = 0;
    int secondValue = 0;
    alignas(8) uint8_t eventBytes[4] = {};

    NetCogibleDdl_ReadIntoSlot(source, nullptr, reinterpret_cast<int>(&firstValue));
    NetCogibleDdl_ReadIntoSlot(source, nullptr, reinterpret_cast<int>(&secondValue));
    NetPlayerDdl_ReadEventByte(source, nullptr, reinterpret_cast<char*>(eventBytes));
    NetCogibleDdl_ReadIntoSlot(source, nullptr, reinterpret_cast<int>(&arg0));

    return C_00858180_EnqueueDdlEventRecord(
        self, nullptr, firstValue, secondValue,
        static_cast<char>(eventBytes[0]), reinterpret_cast<int>(arg0));
}

HOOK(0x00745230, C_00858180_ReadAndEnqueueDdlEvent, "auto");
