#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int16_t, __fastcall, SortedTable_ReadNextByte, 0x00698CB0, (void* self, void* edx, uint8_t* a2));

static char __fastcall S_00698c10_ReadFourBytes(
    S_00698c10* self, void* /*edx*/, uint8_t* output)
{
    alignas(8) uint8_t statusBytes[0x1];

    uint8_t value = static_cast<uint8_t>(
        SortedTable_ReadNextByte(self, nullptr, statusBytes));
    char status = static_cast<char>(statusBytes[0]);
    if (!status)
        return status;
    output[0] = value;

    value = static_cast<uint8_t>(
        SortedTable_ReadNextByte(self, nullptr, statusBytes));
    status = static_cast<char>(statusBytes[0]);
    if (!status)
        return status;
    output[1] = value;

    value = static_cast<uint8_t>(
        SortedTable_ReadNextByte(self, nullptr, statusBytes));
    status = static_cast<char>(statusBytes[0]);
    if (!status)
        return status;
    output[2] = value;

    value = static_cast<uint8_t>(
        SortedTable_ReadNextByte(self, nullptr, statusBytes));
    status = static_cast<char>(statusBytes[0]);
    if (status) {
        output[3] = value;
        output[4] = 0;
    }
    return status;
}

HOOK(0x00698D10, S_00698c10_ReadFourBytes, "auto");
