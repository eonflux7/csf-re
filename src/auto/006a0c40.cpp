#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(uint16_t*, __cdecl, CoreZlibLog_FlushBlock, 0x006A4BC0, (int a1, uint8_t* a2, int a3, int a4));

static void CoreZlibLog_DrainPendingInline(void* stream)
{
    void* pendingState = CSF_AT(void*, stream, 0x1C);
    uint32_t count = CSF_AT(uint32_t, pendingState, 0x14);
    const uint32_t available = CSF_AT(uint32_t, stream, 0x10);
    if (count > available)
        count = available;

    if (count == 0)
        return;

    std::memcpy(CSF_AT(void*, stream, 0x0C),
                CSF_AT(const void*, pendingState, 0x10), count);

    const uint32_t nextOutput = CSF_AT(uint32_t, stream, 0x0C);
    pendingState = CSF_AT(void*, stream, 0x1C);
    CSF_AT(uint32_t, stream, 0x0C) = nextOutput + count;
    CSF_AT(uint32_t, pendingState, 0x10) += count;

    const uint32_t totalOutput = CSF_AT(uint32_t, stream, 0x14);
    const uint32_t remainingOutput = CSF_AT(uint32_t, stream, 0x10);
    CSF_AT(uint32_t, stream, 0x14) = totalOutput + count;
    pendingState = CSF_AT(void*, stream, 0x1C);
    CSF_AT(uint32_t, stream, 0x10) = remainingOutput - count;
    CSF_AT(uint32_t, pendingState, 0x14) -= count;

    pendingState = CSF_AT(void*, stream, 0x1C);
    if (CSF_AT(uint32_t, pendingState, 0x14) == 0)
        CSF_AT(uint32_t, pendingState, 0x10) =
            CSF_AT(uint32_t, pendingState, 0x08);
}

static int __cdecl CoreZlibLog_DeflateStored(uint32_t* state, int flush)
{
    uint32_t maxBlockSize = 0xFFFF;
    if (CSF_AT(uint32_t, state, 0x0C) - 5 < 0xFFFF)
        maxBlockSize = CSF_AT(uint32_t, state, 0x0C) - 5;

    for (;;)
    {
        uint32_t lookahead = CSF_AT(uint32_t, state, 0x6C);
        if (lookahead <= 1)
        {
            hook::Regs registers{};
            registers.ebx = reinterpret_cast<uint32_t>(state);
            registers.eax = lookahead;
            hook::call_regs(CSF_ADDR_FN(0x006A0AA0), &registers, nullptr, 0);

            lookahead = CSF_AT(uint32_t, state, 0x6C);
            if (lookahead == 0)
                break;
        }

        const uint32_t advancedPosition =
            CSF_AT(uint32_t, state, 0x64) + lookahead;
        CSF_AT(uint32_t, state, 0x64) = advancedPosition;
        const int32_t blockStart = CSF_AT(int32_t, state, 0x54);
        const uint32_t position = CSF_AT(uint32_t, state, 0x64);
        CSF_AT(uint32_t, state, 0x6C) = 0;
        const uint32_t blockEnd =
            static_cast<uint32_t>(blockStart) + maxBlockSize;

        if (advancedPosition == 0 || position >= blockEnd)
        {
            CSF_AT(uint32_t, state, 0x6C) = position - blockEnd;
            CSF_AT(uint32_t, state, 0x64) = blockEnd;
            uint8_t* blockData = blockStart < 0 ? nullptr :
                reinterpret_cast<uint8_t*>(
                    CSF_AT(uint32_t, state, 0x30) +
                    static_cast<uint32_t>(blockStart));

            CoreZlibLog_FlushBlock(reinterpret_cast<int>(state), blockData,
                                  static_cast<int>(maxBlockSize), 0);

            const uint32_t newBlockStart = CSF_AT(uint32_t, state, 0x64);
            void* stream = CSF_AT(void*, state, 0x00);
            CSF_AT(uint32_t, state, 0x54) = newBlockStart;
            CoreZlibLog_DrainPendingInline(stream);

            if (CSF_AT(uint32_t, CSF_AT(void*, state, 0x00), 0x10) == 0)
                return 0;
        }

        const int32_t currentBlockStart = CSF_AT(int32_t, state, 0x54);
        const uint32_t blockLength =
            CSF_AT(uint32_t, state, 0x64) -
            static_cast<uint32_t>(currentBlockStart);
        if (blockLength < CSF_AT(uint32_t, state, 0x24) - 262)
            continue;

        uint8_t* blockData = currentBlockStart < 0 ? nullptr :
            reinterpret_cast<uint8_t*>(
                CSF_AT(uint32_t, state, 0x30) +
                static_cast<uint32_t>(currentBlockStart));

        CoreZlibLog_FlushBlock(reinterpret_cast<int>(state), blockData,
                              static_cast<int>(blockLength), 0);

        const uint32_t newBlockStart = CSF_AT(uint32_t, state, 0x64);
        void* stream = CSF_AT(void*, state, 0x00);
        CSF_AT(uint32_t, state, 0x54) = newBlockStart;
        CoreZlibLog_DrainPendingInline(stream);

        if (CSF_AT(uint32_t, CSF_AT(void*, state, 0x00), 0x10) == 0)
            return 0;
    }

    if (flush == 0)
        return 0;

    const int32_t blockStart = CSF_AT(int32_t, state, 0x54);
    uint8_t* blockData = blockStart < 0 ? nullptr :
        reinterpret_cast<uint8_t*>(
            CSF_AT(uint32_t, state, 0x30) +
            static_cast<uint32_t>(blockStart));
    const uint32_t blockLength =
        CSF_AT(uint32_t, state, 0x64) - static_cast<uint32_t>(blockStart);

    CoreZlibLog_FlushBlock(reinterpret_cast<int>(state), blockData,
                          static_cast<int>(blockLength), flush == 4);
    CSF_AT(uint32_t, state, 0x54) = CSF_AT(uint32_t, state, 0x64);

    hook::Regs registers{};
    registers.eax = CSF_AT(uint32_t, state, 0x00);
    hook::call_regs(CSF_ADDR_FN(0x007D4D80), &registers, nullptr, 0);

    if (CSF_AT(uint32_t, CSF_AT(void*, state, 0x00), 0x10) != 0)
        return 2 * (flush == 4) + 1;
    return flush != 4 ? 0 : 2;
}

HOOK(0x006A0C40, CoreZlibLog_DeflateStored, "auto");
