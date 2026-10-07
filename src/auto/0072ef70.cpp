#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Resets every 0x20-byte string record in [begin, end):
// releases the heap buffer of non-SSO strings and re-initialises the
// record to the empty small-string state (buffer = 0, size = 0, capacity = 7).
static int __cdecl ResetStringRecordRange(int begin, int end)
{
    if (begin != end)
    {
        std::uint8_t* record = reinterpret_cast<std::uint8_t*>(begin);
        std::uint8_t* capacityField = record + 0x1Cu;

        do
        {
            if (CSF_AT(std::uint32_t, capacityField, 0) >= 8u)
            {
                void* buffer = reinterpret_cast<void*>(
                    CSF_AT(std::uint32_t, capacityField, -0x14));

                if (buffer != nullptr)
                    Mem_FreeDebug(buffer);
            }

            CSF_AT(std::uint32_t, capacityField, 0) = 7;
            CSF_AT(std::uint32_t, capacityField, -4) = 0;
            CSF_AT(std::uint16_t, capacityField, -0x14) = 0;

            record += 0x20;
            capacityField += 0x20;
        }
        while (record != reinterpret_cast<std::uint8_t*>(end));
    }

    return end;
}

HOOK(0x0072EF70, ResetStringRecordRange, "auto");
