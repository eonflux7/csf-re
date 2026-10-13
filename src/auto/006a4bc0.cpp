#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(uint16_t*, __fastcall, Inflate_ClearLengthTables, 0x006A3330, (void* , void* self));
CSF_FUNC(bool, __fastcall, UpdateBufferPressureFlag, 0x006A4390, (void* self, void*));
CSF_FUNC(std::uint32_t*, __cdecl, CoreZlibLog_EmitStoredBlock, 0x006A4910, (void* state, std::uint8_t* source, int length, int blockFlags));

static int __cdecl CoreZlibLog_FlushBlock(
    void* state, std::uint8_t* source, int length, int blockFlags)
{
    const auto stateAddress =
        static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(state));
    auto* stateBytes = static_cast<std::uint8_t*>(state);
    std::uint32_t lastBitLengthCode = 0;
    std::uint32_t fixedBytes;
    std::uint32_t bestBytes;

    if (CSF_AT(int, state, 0x7C) <= 0)
    {
        fixedBytes = static_cast<std::uint32_t>(length) + 5;
        bestBytes = fixedBytes;
    }
    else
    {
        if (CSF_AT(std::uint8_t, state, 0x1C) == 2)
            UpdateBufferPressureFlag(state, nullptr);

        {
            hook::Regs regs{};
            regs.esi = stateAddress;
            regs.eax = static_cast<std::uint32_t>(
                reinterpret_cast<std::uintptr_t>(stateBytes + 0xB10));
            std::uint32_t stack[] = {regs.eax};
            hook::call_regs(CSF_ADDR_FN(0x006A46A0), &regs, stack, 1);
        }
        {
            hook::Regs regs{};
            regs.esi = stateAddress;
            regs.ecx = static_cast<std::uint32_t>(
                reinterpret_cast<std::uintptr_t>(stateBytes + 0xB1C));
            std::uint32_t stack[] = {regs.ecx};
            hook::call_regs(CSF_ADDR_FN(0x006A46A0), &regs, stack, 1);
        }
        {
            hook::Regs regs{};
            regs.eax = stateAddress;
            regs.esi = stateAddress;
            lastBitLengthCode =
                hook::call_regs(CSF_ADDR_FN(0x006A48A0), &regs, nullptr, 0);
        }

        bestBytes = (CSF_AT(std::uint32_t, state, 0x16A0) + 10) >> 3;
        fixedBytes = (CSF_AT(std::uint32_t, state, 0x16A4) + 10) >> 3;
        if (fixedBytes <= bestBytes)
            bestBytes = fixedBytes;
    }

    if (static_cast<std::uint32_t>(length) + 4 <= bestBytes && source)
    {
        CoreZlibLog_EmitStoredBlock(state, source, length, blockFlags);
    }
    else
    {
        const bool useFixedTree = fixedBytes == bestBytes;
        const int validBits = CSF_AT(int, state, 0x16B4);
        const std::uint32_t header =
            static_cast<std::uint32_t>(blockFlags) + (useFixedTree ? 2 : 4);

        if (validBits <= 13)
        {
            CSF_AT(std::uint16_t, state, 0x16B0) |=
                static_cast<std::uint16_t>(
                    header << (static_cast<unsigned>(validBits) & 31));
            CSF_AT(int, state, 0x16B4) = validBits + 3;
        }
        else
        {
            auto* pendingBuffer = CSF_AT(std::uint8_t*, state, 0x08);
            CSF_AT(std::uint16_t, state, 0x16B0) |=
                static_cast<std::uint16_t>(
                    header << (static_cast<unsigned>(validBits) & 31));

            pendingBuffer[CSF_AT(std::uint32_t, state, 0x14)] =
                CSF_AT(std::uint8_t, state, 0x16B0);

            std::uint32_t pending = CSF_AT(std::uint32_t, state, 0x14);
            const std::uint8_t highByte =
                CSF_AT(std::uint8_t, state, 0x16B1);
            ++pending;
            CSF_AT(std::uint32_t, state, 0x14) = pending;
            CSF_AT(std::uint8_t*, state, 0x08)[pending] = highByte;

            int remainingBits;
            if (useFixedTree)
            {
                pending = CSF_AT(std::uint32_t, state, 0x14);
                remainingBits = CSF_AT(int, state, 0x16B4);
                CSF_AT(std::uint32_t, state, 0x14) = pending + 1;
            }
            else
            {
                ++CSF_AT(std::uint32_t, state, 0x14);
                remainingBits = CSF_AT(int, state, 0x16B4);
            }

            CSF_AT(int, state, 0x16B4) = remainingBits - 13;
            CSF_AT(std::uint16_t, state, 0x16B0) =
                static_cast<std::uint16_t>(
                    static_cast<std::uint32_t>(
                        static_cast<std::uint16_t>(header)) >>
                    ((16u - static_cast<unsigned>(remainingBits)) & 31));
        }

        if (useFixedTree)
        {
            hook::Regs regs{};
            regs.eax = stateAddress;
            regs.esi = stateAddress;
            std::uint32_t stack[] = {
                static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(
                    CSF_ADDR_DATA(0x0085EFA8))),
                static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(
                    CSF_ADDR_DATA(0x0085F428)))
            };
            hook::call_regs(CSF_ADDR_FN(0x006A3F30), &regs, stack, 2);
        }
        else
        {
            const std::uint32_t distanceCodes =
                CSF_AT(std::uint32_t, state, 0xB20) + 1;
            const std::uint32_t literalCodes =
                CSF_AT(std::uint32_t, state, 0xB14) + 1;
            {
                hook::Regs regs{};
                regs.eax = stateAddress;
                regs.esi = stateAddress;
                std::uint32_t stack[] = {
                    literalCodes, distanceCodes, lastBitLengthCode + 1
                };
                hook::call_regs(CSF_ADDR_FN(0x006A3CD0), &regs, stack, 3);
            }
            {
                hook::Regs regs{};
                regs.eax = stateAddress;
                regs.esi = stateAddress;
                std::uint32_t stack[] = {
                    static_cast<std::uint32_t>(
                        reinterpret_cast<std::uintptr_t>(stateBytes + 0x8C)),
                    static_cast<std::uint32_t>(
                        reinterpret_cast<std::uintptr_t>(stateBytes + 0x980))
                };
                hook::call_regs(CSF_ADDR_FN(0x006A3F30), &regs, stack, 2);
            }
        }
    }

    const int result = static_cast<int>(reinterpret_cast<std::uintptr_t>(
        Inflate_ClearLengthTables(nullptr, state)));

    if (blockFlags)
    {
        hook::Regs regs{};
        regs.eax = stateAddress;
        regs.esi = stateAddress;
        return static_cast<int>(
            hook::call_regs(CSF_ADDR_FN(0x006A44A0), &regs, nullptr, 0));
    }
    return result;
}

HOOK(0x006A4BC0, CoreZlibLog_FlushBlock, "auto");
