#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int*, __fastcall, WaterEntry_Construct, 0x0070B140, (void* self, void* edx, int a2, int a3, int a4));
CSF_FUNC(char, __fastcall, WaterEntry_Update, 0x0070B7F0, (void* self, void* edx, int a2));

// Finds (or creates and registers) the entry keyed by `key` and updates it with `arg`.
// Stores "update failed" in the byte at +0x20 and returns the update result.
static char __fastcall Water_UpdateEntry(void* self, void* /*edx*/, int arg, int key)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(self);
    int* entry = nullptr;

    unsigned int count = CSF_AT(unsigned int, base, 0x0C);
    unsigned int i = 0;
    if (count != 0) {
        int** data = CSF_AT(int**, base, 0x04);
        while (data[i][3] != key) {
            ++i;
            if (i >= count)
                goto create;
        }
        entry = reinterpret_cast<int*>(CSF_AT(int**, base, 0x04)[i]);
        if (entry)
            goto update;
    }
create:
    {
        void* mem = Mem_Alloc(28);
        if (mem) {
            int owner = CSF_AT(int, base, 0x14);
            entry = WaterEntry_Construct(mem, nullptr, owner, key, CSF_AT(int, owner, 0x08));
            if (entry) {
                if (CSF_AT(unsigned int, base, 0x0C) >= CSF_AT(unsigned int, base, 0x08))
                    S_0041e5f0_GrowArrayCapacity(reinterpret_cast<S_0041e5f0*>(base + 4), nullptr, CSF_AT(int, base, 0x10));
                unsigned int n = CSF_AT(unsigned int, base, 0x0C);
                int** slot = CSF_AT(int**, base, 0x04) + n;
                CSF_AT(unsigned int, base, 0x0C) = n + 1;
                *slot = entry;
                goto update;
            }
        }
        CSF_AT(uint8_t, base, 0x20) = 1;
        return 0;
    }
update:
    {
        char result = WaterEntry_Update(entry, nullptr, arg);
        CSF_AT(uint8_t, base, 0x20) = (result == 0);
        return result;
    }
}

HOOK(0x0070B960, Water_UpdateEntry, "auto");
