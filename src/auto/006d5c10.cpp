#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"

// Virtual slot 0 of the list element: returns the element's name as a C string.
using GetEntryNameFn = const char* (__fastcall*)(void* self, void* edx);

static int __fastcall FindFirstEntryByName(int* self, void* /*edx*/, const char* name)
{
    uint32_t result = 0;
    int index = 0;
    uint32_t foundEntry = 0;

    const int count = CSF_AT(int, self, 0xC);
    if (count > 0)
    {
        do
        {
            if (result != 0)
                break;

            // The list data pointer is re-read on every iteration, as the original does.
            auto* items = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x4));
            void* entry = reinterpret_cast<void*>(items[index]);

            // entry->vtbl[0]() gives the entry's name.
            auto getName = reinterpret_cast<GetEntryNameFn>(CSF_AT(void**, entry, 0)[0]);
            const char* entryName = getName(entry, nullptr);

            if (std::strcmp(entryName, name) == 0)
            {
                auto* itemsAgain = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x4));
                foundEntry = itemsAgain[index];
            }

            ++index;
            result = foundEntry;
        } while (index < count);
    }

    return static_cast<int>(result);
}

HOOK(0x006D5C10, FindFirstEntryByName, "auto");
