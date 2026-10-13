#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static uint32_t __cdecl CoreZlibLog_GenerateBitLengths(
    hook::Regs* regs, const uint32_t* /*args*/)
{
    const uint32_t stateAddress = regs->eax;
    auto* state = reinterpret_cast<uint8_t*>(stateAddress);
    auto* descriptor = reinterpret_cast<uint8_t*>(regs->ecx);

    const int32_t maxCode = CSF_AT(int32_t, descriptor, 0x04);
    auto* tree = reinterpret_cast<uint8_t*>(
        CSF_AT(uint32_t, descriptor, 0x00));
    auto* staticDescriptor = reinterpret_cast<uint8_t*>(
        CSF_AT(uint32_t, descriptor, 0x08));

    auto* staticTree = reinterpret_cast<uint8_t*>(
        CSF_AT(uint32_t, staticDescriptor, 0x00));
    auto* extraBits = reinterpret_cast<uint8_t*>(
        CSF_AT(uint32_t, staticDescriptor, 0x04));
    const int32_t extraBase = CSF_AT(int32_t, staticDescriptor, 0x08);
    const int32_t maxLength = CSF_AT(int32_t, staticDescriptor, 0x10);

    CSF_AT(uint32_t, state, 0xB34) = 0;
    CSF_AT(uint32_t, state, 0xB38) = 0;
    CSF_AT(uint32_t, state, 0xB3C) = 0;
    CSF_AT(uint32_t, state, 0xB40) = 0;
    CSF_AT(uint32_t, state, 0xB44) = 0;
    CSF_AT(uint32_t, state, 0xB48) = 0;
    CSF_AT(uint32_t, state, 0xB4C) = 0;
    CSF_AT(uint32_t, state, 0xB50) = 0;

    const int32_t root = CSF_AT(
        int32_t, state,
        0xB54 + 4 * CSF_AT(int32_t, state, 0x144C));
    CSF_AT(uint16_t, tree, 4 * root + 2) = 0;

    int32_t heapIndex = CSF_AT(int32_t, state, 0x144C) + 1;
    int32_t overflow = 0;
    if (heapIndex >= 573)
        return stateAddress;

    for (; heapIndex < 573; ++heapIndex)
    {
        const int32_t node = CSF_AT(int32_t, state, 0xB54 + 4 * heapIndex);
        const uint16_t parent = CSF_AT(uint16_t, tree, 4 * node + 2);
        int32_t length = CSF_AT(uint16_t, tree, 4 * parent + 2) + 1;
        if (length > maxLength)
        {
            ++overflow;
            length = maxLength;
        }

        CSF_AT(uint16_t, tree, 4 * node + 2) =
            static_cast<uint16_t>(length);

        if (node <= maxCode)
        {
            ++CSF_AT(uint16_t, state, 0xB34 + 2 * length);

            int32_t extra = 0;
            if (node >= extraBase)
                extra = CSF_AT(int32_t, extraBits, 4 * (node - extraBase));

            const uint32_t frequency = CSF_AT(uint16_t, tree, 4 * node);
            CSF_AT(uint32_t, state, 0x16A0) +=
                frequency * (static_cast<uint32_t>(extra) +
                             static_cast<uint32_t>(length));

            if (staticTree)
            {
                CSF_AT(uint32_t, state, 0x16A4) +=
                    frequency *
                    (static_cast<uint32_t>(extra) +
                     CSF_AT(uint16_t, staticTree, 4 * node + 2));
            }
        }
    }

    if (!overflow)
        return stateAddress;

    do
    {
        int32_t length = maxLength - 1;
        while (!CSF_AT(uint16_t, state, 0xB34 + 2 * length))
            --length;

        --CSF_AT(uint16_t, state, 0xB34 + 2 * length);
        CSF_AT(uint16_t, state, 0xB34 + 2 * (length + 1)) += 2;
        --CSF_AT(uint16_t, state, 0xB34 + 2 * maxLength);
        overflow -= 2;
    }
    while (overflow > 0);

    heapIndex = 573;
    for (int32_t length = maxLength; length != 0; --length)
    {
        int32_t remaining = CSF_AT(uint16_t, state, 0xB34 + 2 * length);
        while (remaining)
        {
            --heapIndex;
            const int32_t node =
                CSF_AT(int32_t, state, 0xB54 + 4 * heapIndex);
            if (node <= maxCode)
            {
                const int32_t oldLength =
                    CSF_AT(uint16_t, tree, 4 * node + 2);
                if (oldLength != length)
                {
                    CSF_AT(uint32_t, state, 0x16A0) +=
                        CSF_AT(uint16_t, tree, 4 * node) *
                        (static_cast<uint32_t>(length) -
                         static_cast<uint32_t>(oldLength));
                    CSF_AT(uint16_t, tree, 4 * node + 2) =
                        static_cast<uint16_t>(length);
                }
                --remaining;
            }
        }
    }

    return stateAddress;
}

HOOK_REGS(0x006A3480, CoreZlibLog_GenerateBitLengths, 0, "core_zlib_log");
