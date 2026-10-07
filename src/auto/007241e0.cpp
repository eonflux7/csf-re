#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, ResetWideStringsInRecordRange, 0x007233D0,
         (int begin, int end));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int* __cdecl ResetWideStringRecordRange(int* first, int* last)
{
    auto* record = reinterpret_cast<std::uint8_t*>(first);
    auto* end = reinterpret_cast<std::uint8_t*>(last);

    while (record != end)
    {
        int storage = CSF_AT(int, record, 0x04);
        if (storage != 0)
        {
            ResetWideStringsInRecordRange(
                storage, CSF_AT(int, record, 0x08));

            storage = CSF_AT(int, record, 0x04);
            if (storage != 0)
            {
                Mem_FreeDebug(
                    reinterpret_cast<void*>(
                        static_cast<std::uintptr_t>(storage)));
            }
        }

        CSF_AT(int, record, 0x04) = 0;
        CSF_AT(int, record, 0x08) = 0;
        CSF_AT(int, record, 0x0C) = 0;
        record += 0x10;
    }

    return last;
}

HOOK(0x007241E0, ResetWideStringRecordRange, "auto");
