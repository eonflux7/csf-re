#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// regparm(3) assigns EAX, EDX, ECX; the unused middle argument
// preserves the original EAX/ECX inputs and caller-popped stack argument.
static int __cdecl __attribute__((regparm(3)))
CoreZlibLog_ScanTreeCodeLengths(
    void* tree,
    void* /*edx*/,
    int maxCode,
    std::uint16_t* frequencies)
{
    int nextLength = CSF_AT(std::uint16_t, tree, 2);
    int runLength = 0;
    int previousLength = -1;
    int maxRun = 7;
    int minRun = 4;

    if (nextLength == 0)
    {
        maxRun = 138;
        minRun = 3;
    }

    CSF_AT(std::uint16_t, tree,
           static_cast<std::uint32_t>(maxCode) * 4 + 6) = 0xFFFF;

    if (maxCode < 0)
        return static_cast<int>(reinterpret_cast<std::intptr_t>(tree));

    auto* nextNode = static_cast<std::uint8_t*>(tree) + 6;
    std::uint32_t remaining = static_cast<std::uint32_t>(maxCode) + 1;
    int length = 0;

    do
    {
        length = nextLength;
        nextLength = CSF_AT(std::uint16_t, nextNode, 0);
        ++runLength;

        if (runLength >= maxRun || length != nextLength)
        {
            if (runLength < minRun)
            {
                CSF_AT(std::uint16_t, frequencies, 0xA74 + 4 * length) +=
                    runLength;
            }
            else if (length != 0)
            {
                if (length != previousLength)
                    ++CSF_AT(std::uint16_t, frequencies, 0xA74 + 4 * length);
                ++CSF_AT(std::uint16_t, frequencies, 0xAB4);
            }
            else if (runLength > 10)
            {
                ++CSF_AT(std::uint16_t, frequencies, 0xABC);
            }
            else
            {
                ++CSF_AT(std::uint16_t, frequencies, 0xAB8);
            }

            runLength = 0;
            previousLength = length;

            if (nextLength == 0)
            {
                maxRun = 138;
                minRun = 3;
            }
            else if (length == nextLength)
            {
                maxRun = 6;
                minRun = 3;
            }
            else
            {
                maxRun = 7;
                minRun = 4;
            }
        }

        nextNode += 4;
        --remaining;
    } while (remaining != 0);

    return length;
}

HOOK(0x006A3680, CoreZlibLog_ScanTreeCodeLengths, "auto");
