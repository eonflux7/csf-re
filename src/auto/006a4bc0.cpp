#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __attribute__((regparm(1))) __cdecl,
         CoreZlibLog_SendTreeDescriptions, 0x006A3CD0,
         (int state, int literalCodes, int distanceCodes, int bitLengthCodes));
CSF_FUNC(int, __attribute__((regparm(1))) __cdecl,
         CoreZlibLog_CompressBlock, 0x006A3F30,
         (int state, int literalTree, int distanceTree));
CSF_FUNC(int, __attribute__((regparm(1))) __cdecl,
         CoreZlibLog_WindUpBits, 0x006A44A0, (int state));
CSF_FUNC(int, __attribute__((regparm(1))) __cdecl,
         CoreZlibLog_BuildBitLengthTree, 0x006A48A0, (int state));
CSF_FUNC(int, __cdecl, CoreZlibLog_BuildTree, 0x006A46A0,
         (uint32_t* state, int* treeDescription));
CSF_FUNC(uint16_t*, __fastcall, CoreZlibLog_ResetBlock, 0x006A3330,
         (int a1, int a2));
CSF_FUNC(bool, __fastcall, CoreZlibLog_DetectDataType, 0x006A4390,
         (void* self, void* edx));
CSF_FUNC(uint32_t*, __cdecl, CoreZlibLog_WriteStoredBlock, 0x006A4910,
         (int a1, uint8_t* a2, int a3, int a4));

static uint16_t* __cdecl CoreZlibLog_FlushBlock(
    int stateAddress, uint8_t* storedData, int storedLength, int finalBlock)
{
    void* state = reinterpret_cast<void*>(stateAddress);
    int lastBitLengthCode = 0;
    uint32_t dynamicBytes;
    uint32_t selectedBytes;

    if (CSF_AT(int, state, 0x7C) <= 0)
    {
        dynamicBytes = static_cast<uint32_t>(storedLength) + 5;
        selectedBytes = dynamicBytes;
    }
    else
    {
        if (CSF_AT(uint8_t, state, 0x1C) == 2)
            CoreZlibLog_DetectDataType(state, nullptr);

        CoreZlibLog_BuildTree(
            reinterpret_cast<uint32_t*>(state),
            reinterpret_cast<int*>(static_cast<uint8_t*>(state) + 0xB10));
        CoreZlibLog_BuildTree(
            reinterpret_cast<uint32_t*>(state),
            reinterpret_cast<int*>(static_cast<uint8_t*>(state) + 0xB1C));
        lastBitLengthCode = CoreZlibLog_BuildBitLengthTree(stateAddress);

        selectedBytes = (CSF_AT(uint32_t, state, 0x16A0) + 10) >> 3;
        dynamicBytes = (CSF_AT(uint32_t, state, 0x16A4) + 10) >> 3;
        if (dynamicBytes <= selectedBytes)
            selectedBytes = dynamicBytes;
    }

    if (static_cast<uint32_t>(storedLength) + 4 <= selectedBytes && storedData)
    {
        CoreZlibLog_WriteStoredBlock(
            stateAddress, storedData, storedLength, finalBlock);
    }
    else if (dynamicBytes == selectedBytes)
    {
        const uint32_t header = static_cast<uint32_t>(finalBlock) + 2;
        const int bitCount = CSF_AT(int, state, 0x16B4);

        if (bitCount <= 13)
        {
            CSF_AT(uint16_t, state, 0x16B0) |=
                static_cast<uint16_t>(header << (static_cast<uint32_t>(bitCount) & 31));
            CSF_AT(uint32_t, state, 0x16B4) =
                static_cast<uint32_t>(bitCount) + 3;
        }
        else
        {
            uint8_t* output = CSF_AT(uint8_t*, state, 0x08);
            CSF_AT(uint16_t, state, 0x16B0) |=
                static_cast<uint16_t>(header << (static_cast<uint32_t>(bitCount) & 31));
            output[CSF_AT(uint32_t, state, 0x14)] =
                CSF_AT(uint8_t, state, 0x16B0);

            uint32_t pending = CSF_AT(uint32_t, state, 0x14);
            const uint8_t highByte = CSF_AT(uint8_t, state, 0x16B1);
            ++pending;
            CSF_AT(uint32_t, state, 0x14) = pending;
            CSF_AT(uint8_t*, state, 0x08)[pending] = highByte;

            pending = CSF_AT(uint32_t, state, 0x14);
            const uint32_t remainingBits = CSF_AT(uint32_t, state, 0x16B4);
            CSF_AT(uint32_t, state, 0x14) = pending + 1;
            CSF_AT(uint32_t, state, 0x16B4) = remainingBits - 13;
            CSF_AT(uint16_t, state, 0x16B0) = static_cast<uint16_t>(
                static_cast<uint32_t>(static_cast<uint16_t>(header)) >>
                ((16 - remainingBits) & 31));
        }

        CoreZlibLog_CompressBlock(
            stateAddress, (int)CSF_ADDR_DATA(0x0085EFA8),
            (int)CSF_ADDR_DATA(0x0085F428));
    }
    else
    {
        const uint32_t header = static_cast<uint32_t>(finalBlock) + 4;
        const int bitCount = CSF_AT(int, state, 0x16B4);

        if (bitCount <= 13)
        {
            CSF_AT(uint16_t, state, 0x16B0) |=
                static_cast<uint16_t>(header << (static_cast<uint32_t>(bitCount) & 31));
            CSF_AT(uint32_t, state, 0x16B4) =
                static_cast<uint32_t>(bitCount) + 3;
        }
        else
        {
            uint8_t* output = CSF_AT(uint8_t*, state, 0x08);
            CSF_AT(uint16_t, state, 0x16B0) |=
                static_cast<uint16_t>(header << (static_cast<uint32_t>(bitCount) & 31));
            output[CSF_AT(uint32_t, state, 0x14)] =
                CSF_AT(uint8_t, state, 0x16B0);

            uint32_t pending = CSF_AT(uint32_t, state, 0x14);
            const uint8_t highByte = CSF_AT(uint8_t, state, 0x16B1);
            ++pending;
            CSF_AT(uint32_t, state, 0x14) = pending;
            CSF_AT(uint8_t*, state, 0x08)[pending] = highByte;

            ++CSF_AT(uint32_t, state, 0x14);
            const uint32_t remainingBits = CSF_AT(uint32_t, state, 0x16B4);
            CSF_AT(uint32_t, state, 0x16B4) = remainingBits - 13;
            CSF_AT(uint16_t, state, 0x16B0) = static_cast<uint16_t>(
                static_cast<uint32_t>(static_cast<uint16_t>(header)) >>
                ((16 - remainingBits) & 31));
        }

        const int distanceCodes =
            static_cast<int>(CSF_AT(uint32_t, state, 0xB20) + 1);
        const int literalCodes =
            static_cast<int>(CSF_AT(uint32_t, state, 0xB14) + 1);
        CoreZlibLog_SendTreeDescriptions(
            stateAddress, literalCodes, distanceCodes, lastBitLengthCode + 1);
        CoreZlibLog_CompressBlock(
            stateAddress,
            reinterpret_cast<int>(static_cast<uint8_t*>(state) + 0x8C),
            reinterpret_cast<int>(static_cast<uint8_t*>(state) + 0x980));
    }

    uint16_t* result = CoreZlibLog_ResetBlock(0, stateAddress);
    if (finalBlock)
        return reinterpret_cast<uint16_t*>(CoreZlibLog_WindUpBits(stateAddress));
    return result;
}

HOOK(0x006A4BC0, CoreZlibLog_FlushBlock, "auto");
