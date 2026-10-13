#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, CoreZlibLog_DeflateStored, 0x006A0C40, (uint32_t* a1, int a2));
CSF_FUNC(std::uint32_t*, __cdecl, CoreZlibLog_EmitStoredBlock, 0x006A4910, (void* state, std::uint8_t* source, int length, int blockFlags));
CSF_FUNC(int, __cdecl, CoreZlibLog_AlignDeflateBlock, 0x006A49C0, (void* state));

using DeflateFunction = int(__cdecl*)(uint32_t*, int);

CSF_GLOBAL(const char*, g_DeflateStreamError, 0x008A6C08);
CSF_GLOBAL(const char*, g_DeflateBufferError, 0x008A6C14);

static uint32_t CoreZlibLog_AppendHeaderWord(uint32_t writer, uint32_t value)
{
    hook::Regs regs{};
    regs.eax = writer;
    regs.ecx = value;
    regs.esi = writer;
    return hook::call_regs(CSF_ADDR_FN(0x006A0540), &regs, nullptr, 0);
}

static void CoreZlibLog_FlushPending(uint32_t* stream)
{
    hook::Regs regs{};
    regs.eax = reinterpret_cast<uint32_t>(stream);
    hook::call_regs(CSF_ADDR_FN(0x007D4D80), &regs, nullptr, 0);
}

static int __cdecl CoreZlibLog_Deflate(uint32_t* stream, int flush)
{
    if (!stream)
        return -2;

    uint32_t* state = CSF_AT(uint32_t*, stream, 0x1C);
    if (!state || flush > 4 || flush < 0)
        return -2;

    if (!CSF_AT(void*, stream, 0x0C) ||
        (!CSF_AT(void*, stream, 0x00) && CSF_AT(uint32_t, stream, 0x04)) ||
        (CSF_AT(int, state, 0x04) == 666 && flush != 4))
    {
        CSF_AT(const char*, stream, 0x18) = g_DeflateStreamError;
        return -2;
    }

    if (!CSF_AT(uint32_t, stream, 0x10))
    {
        CSF_AT(const char*, stream, 0x18) = g_DeflateBufferError;
        return -5;
    }

    const int previousFlush = CSF_AT(int, state, 0x20);
    CSF_AT(uint32_t*, state, 0x00) = stream;
    CSF_AT(int, state, 0x20) = flush;

    if (CSF_AT(int, state, 0x04) == 42)
    {
        uint32_t levelFlags = static_cast<uint32_t>(
            static_cast<int32_t>(CSF_AT(uint32_t, state, 0x7C) - 1) >> 1);
        if (levelFlags > 3)
            levelFlags = 3;

        uint32_t header = (levelFlags << 6) |
            (((CSF_AT(uint32_t, state, 0x28) - 8) << 12) + 2048);
        if (CSF_AT(uint32_t, state, 0x64))
            header |= 0x20u;

        CSF_AT(int, state, 0x04) = 113;
        CoreZlibLog_AppendHeaderWord(
            reinterpret_cast<uint32_t>(state), header - header % 0x1F + 31);

        if (CSF_AT(uint32_t, state, 0x64))
        {
            uint32_t writer = CoreZlibLog_AppendHeaderWord(
                reinterpret_cast<uint32_t>(state),
                CSF_AT(uint16_t, stream, 0x32));
            CoreZlibLog_AppendHeaderWord(
                writer, CSF_AT(uint32_t, stream, 0x30) & 0xFFFF);
        }
        CSF_AT(uint32_t, stream, 0x30) = 1;
    }

    if (CSF_AT(uint32_t, state, 0x14))
    {
        CoreZlibLog_FlushPending(stream);
        if (!CSF_AT(uint32_t, stream, 0x10))
        {
            CSF_AT(int, state, 0x20) = -1;
            return 0;
        }
    }
    else if (!CSF_AT(uint32_t, stream, 0x04) &&
             flush <= previousFlush && flush != 4)
    {
        CSF_AT(const char*, stream, 0x18) = g_DeflateBufferError;
        return -5;
    }

    const int status = CSF_AT(int, state, 0x04);
    const uint32_t availableInput = CSF_AT(uint32_t, stream, 0x04);
    if (status == 666 && availableInput)
    {
        CSF_AT(const char*, stream, 0x18) = g_DeflateBufferError;
        return -5;
    }

    if ((status != 666 && availableInput) ||
        CSF_AT(uint32_t, state, 0x6C) ||
        (flush != 0 && status != 666))
    {
        const int result =
            reinterpret_cast<DeflateFunction*>(CSF_VTBL(0x0085ECE0))
                [3 * CSF_AT(uint32_t, state, 0x7C)](state, flush);

        if (result == 2 || result == 3)
            CSF_AT(int, state, 0x04) = 666;

        if (result == 0 || result == 2)
        {
            if (!CSF_AT(uint32_t, stream, 0x10))
                CSF_AT(int, state, 0x20) = -1;
            return 0;
        }

        if (result == 1)
        {
            if (flush == 1)
            {
                CoreZlibLog_AlignDeflateBlock(state);
            }
            else
            {
                CoreZlibLog_EmitStoredBlock(state, nullptr, 0, 0);
                if (flush == 3)
                {
                    CSF_AT(uint16_t*, state, 0x3C)
                        [CSF_AT(uint32_t, state, 0x44) - 1] = 0;
                    const uint32_t clearBytes =
                        2 * CSF_AT(uint32_t, state, 0x44) - 2;
                    std::memset(CSF_AT(void*, state, 0x3C), 0, clearBytes);
                }
            }

            CoreZlibLog_FlushPending(stream);
            if (!CSF_AT(uint32_t, stream, 0x10))
            {
                CSF_AT(int, state, 0x20) = -1;
                return 0;
            }
        }
    }

    if (flush != 4)
        return 0;
    if (CSF_AT(int, state, 0x18))
        return 1;

    uint32_t writer = CoreZlibLog_AppendHeaderWord(
        reinterpret_cast<uint32_t>(state), CSF_AT(uint16_t, stream, 0x32));
    CoreZlibLog_AppendHeaderWord(
        writer, CSF_AT(uint32_t, stream, 0x30) & 0xFFFF);
    CoreZlibLog_FlushPending(stream);

    const uint32_t pending = CSF_AT(uint32_t, state, 0x14);
    CSF_AT(int, state, 0x18) = -1;
    return pending == 0;
}

HOOK(0x006A0570, CoreZlibLog_Deflate, "auto");
