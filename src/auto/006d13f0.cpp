#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Virtual slot +0x14 (index 5) of the list element's vtable, returning an int key.
using InnerKeyGetterFn = int(__fastcall*)(void* self, void* edx);

static int __fastcall C_00860e68_FindEntryByKey(C_00860e68* self, void* /*edx*/, int key)
{
    uint32_t count = self->field_003c;
    int result = 0;
    uint32_t index = 0;

    if (count == 0)
        return result;

    while (true)
    {
        // The element array is re-read from the object on every iteration.
        uint32_t* entries = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(self->field_0034));
        void* item = reinterpret_cast<void*>(static_cast<uintptr_t>(entries[index]));

        InnerKeyGetterFn getInnerKey = reinterpret_cast<InnerKeyGetterFn>(CSF_AT(void**, item, 0)[5]);
        if (getInnerKey(item, nullptr) == key)
        {
            entries = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(self->field_0034));
            return static_cast<int>(entries[index]);
        }

        ++index;
        if (index >= count)
            return 0;
    }
}

HOOK(0x6D13F0, C_00860e68_FindEntryByKey, "auto");
