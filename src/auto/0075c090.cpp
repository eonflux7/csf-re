#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Copies the graph-table state of `source` into `self`: reallocates the (index, index) pair
// table when the source holds more entries than `self` can store, then copies the entries
// and the count/extra fields. Returns `self`.
static void* __fastcall WaypointGraph_CopyAssignFrom(void* self, void* /*edx*/, void* source)
{
    if (CSF_AT(int, source, 0x8) > CSF_AT(int, self, 0x4))
    {
        int newCapacity = CSF_AT(int, source, 0x4);
        int* newData = nullptr;

        int* block = static_cast<int*>(Mem_Alloc(8 * static_cast<std::size_t>(newCapacity)));
        if (block)
        {
            // Every pair is initialised to (-1, -1).
            for (int i = 0; i < newCapacity; ++i)
            {
                block[i * 2] = -1;
                block[i * 2 + 1] = -1;
            }
            newData = block;
        }

        void* oldData = CSF_AT(void*, self, 0);
        CSF_AT(int, self, 0x4) = CSF_AT(int, source, 0x4);
        if (oldData != nullptr)
        {
            Mem_Free(oldData);
            CSF_AT(void*, self, 0) = nullptr;
        }
        CSF_AT(void*, self, 0) = newData;
    }

    int count = CSF_AT(int, source, 0x8);
    if (count != 0)
    {
        std::memcpy(CSF_AT(void*, self, 0),
                    CSF_AT(const void*, source, 0),
                    8 * static_cast<std::size_t>(count));
    }

    CSF_AT(int, self, 0x8) = CSF_AT(int, source, 0x8);
    CSF_AT(int, self, 0xC) = CSF_AT(int, source, 0xC);
    return self;
}

HOOK(0x0075C090, WaypointGraph_CopyAssignFrom, "auto");
