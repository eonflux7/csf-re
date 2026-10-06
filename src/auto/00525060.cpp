#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall ResetAndReleaseObjectSlots(void* self, void* /*edx*/)
{
    // The first slot is read before the sentinel writes.
    void* object = reinterpret_cast<void*>(
        CSF_AT(std::uint32_t, self, 0x08));

    CSF_AT(std::uint32_t, self, 0x00) = 0xFFFFFFFFu;
    CSF_AT(std::uint32_t, self, 0x04) = 0xFFFFFFFFu;

    // Incoming EAX is unavailable in ordinary C++; use zero if all slots are null.
    int result = 0;

    for (std::uint32_t offset = 0x08; offset <= 0x34; offset += 4)
    {
        if (offset != 0x08)
        {
            object = reinterpret_cast<void*>(
                CSF_AT(std::uint32_t, self, offset));
        }

        if (object != nullptr)
        {
            result =
                reinterpret_cast<int (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, object, 0)[19])(object, nullptr);

            CSF_AT(std::uint32_t, self, offset) = 0;
        }
    }

    return result;
}

HOOK(0x00525060, ResetAndReleaseObjectSlots, "auto");
