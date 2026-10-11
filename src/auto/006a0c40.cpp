#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Uses the existing FillWindow adapter, which marshals state into EBX.
CSF_FUNC(unsigned int, __cdecl, CoreZlibLog_FillWindow, 0x006A0AA0,
         (void* state));

CSF_FUNC(uint16_t*, __cdecl, CoreZlibLog_EmitStoredBlock, 0x006A4BC0,
         (int a1, uint8_t* a2, int a3, int a4));

// This outlined routine receives the stream in EAX, with no stack arguments.
CSF_FUNC(void, __attribute__((regparm(1))), CoreZlibLog_FlushPending,
         0x007D4D80, (void* stream));

static void CoreZlibLog_CopyPendingOutput(void* stream)
{
    void* pendingState = CSF_AT(void*, stream, 0x1C);
    uint32_t count = CSF_AT(uint32_t, pendingState, 0x14);
    const uint32_t available = CSF_AT(uint32_t, stream, 0x10);
    if (count > available)
        count = available;

    if (count != 0)
    {
        std::memcpy(CSF_AT(void*, stream, 0x0C),
                    CSF_AT(void*, pendingState, 0x10), count);

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
        {
            CSF_AT(uint32_t, pendingState, 0x10) =
                CSF_AT(uint32_t, pendingState, 0x08);
        }
    }
}

static int __cdecl CoreZlibLog_DeflateStored(void* state, int flush)
{
    uint32_t maximumBlockSize = 0xFFFF;
    const uint32_t bufferLimit = CSF_AT(uint32_t, state, 0x0C) - 5;
    if (bufferLimit < 0xFFFF)
        maximumBlockSize = bufferLimit;

    for (;;)
    {
        uint32_t lookahead = CSF_AT(uint32_t, state, 0x6C);
        if (lookahead <= 1)
        {
            CoreZlibLog_FillWindow(state);
            lookahead = CSF_AT(uint32_t, state, 0x6C);
            if (lookahead == 0)
                break;
        }

        const uint32_t advancedPosition =
            CSF_AT(uint32_t, state, 0x64) + lookahead;
        CSF_AT(uint32_t, state, 0x64) = advancedPosition;

        uint32_t blockStart = CSF_AT(uint32_t, state, 0x54);
        const uint32_t position = CSF_AT(uint32_t, state, 0x64);
        CSF_AT(uint32_t, state, 0x6C) = 0;
        const uint32_t blockEnd = blockStart + maximumBlockSize;

        if (advancedPosition == 0 || position >= blockEnd)
        {
            CSF_AT(uint32_t, state, 0x6C) = position - blockEnd;
            CSF_AT(uint32_t, state, 0x64) = blockEnd;

            uint8_t* block = nullptr;
            if (static_cast<int32_t>(blockStart) >= 0)
            {
                block = reinterpret_cast<uint8_t*>(
                    CSF_AT(uint32_t, state, 0x30) + blockStart);
            }
            CoreZlibLog_EmitStoredBlock(
                reinterpret_cast<int>(state), block,
                static_cast<int>(maximumBlockSize), 0);

            const uint32_t nextBlockStart = CSF_AT(uint32_t, state, 0x64);
            void* stream = CSF_AT(void*, state, 0x00);
            CSF_AT(uint32_t, state, 0x54) = nextBlockStart;
            CoreZlibLog_CopyPendingOutput(stream);

            if (CSF_AT(uint32_t, CSF_AT(void*, state, 0x00), 0x10) == 0)
                return 0;
        }

        blockStart = CSF_AT(uint32_t, state, 0x54);
        const uint32_t blockSize =
            CSF_AT(uint32_t, state, 0x64) - blockStart;
        if (blockSize < CSF_AT(uint32_t, state, 0x24) - 262)
            continue;

        uint8_t* block = nullptr;
        if (static_cast<int32_t>(blockStart) >= 0)
        {
            block = reinterpret_cast<uint8_t*>(
                CSF_AT(uint32_t, state, 0x30) + blockStart);
        }
        CoreZlibLog_EmitStoredBlock(
            reinterpret_cast<int>(state), block,
            static_cast<int>(blockSize), 0);

        const uint32_t nextBlockStart = CSF_AT(uint32_t, state, 0x64);
        void* stream = CSF_AT(void*, state, 0x00);
        CSF_AT(uint32_t, state, 0x54) = nextBlockStart;
        CoreZlibLog_CopyPendingOutput(stream);

        if (CSF_AT(uint32_t, CSF_AT(void*, state, 0x00), 0x10) == 0)
            return 0;
    }

    if (flush == 0)
        return 0;

    const uint32_t blockStart = CSF_AT(uint32_t, state, 0x54);
    uint8_t* block = nullptr;
    if (static_cast<int32_t>(blockStart) >= 0)
    {
        block = reinterpret_cast<uint8_t*>(
            CSF_AT(uint32_t, state, 0x30) + blockStart);
    }
    const uint32_t blockSize =
        CSF_AT(uint32_t, state, 0x64) - blockStart;
    CoreZlibLog_EmitStoredBlock(
        reinterpret_cast<int>(state), block,
        static_cast<int>(blockSize), flush == 4);

    CSF_AT(uint32_t, state, 0x54) = CSF_AT(uint32_t, state, 0x64);
    CoreZlibLog_FlushPending(CSF_AT(void*, state, 0x00));

    if (CSF_AT(uint32_t, CSF_AT(void*, state, 0x00), 0x10) != 0)
        return 2 * (flush == 4) + 1;

    return flush != 4 ? 0 : 2;
}

HOOK(0x006A0C40, CoreZlibLog_DeflateStored, "auto");
