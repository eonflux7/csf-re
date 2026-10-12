#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

// Removes every registered item whose field at +4 equals a2 from the list at +0x7C
// (count at +0x84), calling the item's slot-1 virtual (destructor-like, arg 1) first.
// Stops after the first removal. Returns 1 if something was removed, else 0.
static char __fastcall C_0083e124_UnregisterFromGlobalList(C_0083e124* self, void* /*edx*/, int a2)
{
    char result = 0;
    for (uint32_t i = 0; i < CSF_AT(uint32_t, self, 0x84); ++i)
    {
        if (result)
            break;

        void** items = CSF_AT(void**, self, 0x7C);
        void** item = reinterpret_cast<void**>(items[i]);
        if (CSF_AT(int, item, 4) == a2)
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(CSF_AT(void**, item, 0)[1])(item, nullptr, 1);

            uint32_t newCount = CSF_AT(uint32_t, self, 0x84) - 1;
            CSF_AT(uint32_t, self, 0x84) = newCount;
            if (i != newCount)
            {
                void** list = CSF_AT(void**, self, 0x7C);
                _memcpy(
                    static_cast<void*>(&list[i]),
                    static_cast<const void*>(&list[i + 1]),
                    static_cast<std::size_t>(4) * (newCount - i));
            }
            result = 1;
        }
    }
    return result;
}

HOOK(0x6B1A30, C_0083e124_UnregisterFromGlobalList, "auto");
