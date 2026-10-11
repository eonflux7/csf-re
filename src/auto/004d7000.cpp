#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CharTask_LookupByOffset, 0x004DB440, (void* self, void* edx, int a2));

// Resolves the object tied to this task's owner. If the owner's ai object reports
// the task as active, returns the lookup by offset directly. Otherwise it queries
// the object for a flags word and returns that pointer when bit 1 is set; if not,
// it falls back to the lookup.
static uint32_t* __fastcall C_00834fa8_GetFlaggedResultOrLookup(C_00834fa8* self, void* /*edx*/, int /*a2*/)
{
    uint8_t* owner = reinterpret_cast<uint8_t*>(self->field_0008);
    void* parent = CSF_AT(void*, owner, 0x14);
    void* obj = CSF_AT(void*, parent, 0x30);

    // Virtual slot 0xB8 / 4 = 46: bool query on obj.
    if (((bool(__fastcall*)(void*, void*))CSF_AT(void**, obj, 0)[46])(obj, nullptr))
    {
        int lookupOffset = static_cast<int>(self->field_000c + 84);
        return reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(
            CharTask_LookupByOffset(self, nullptr, lookupOffset)));
    }

    // Virtual slot 0xBC / 4 = 47: returns a pointer, writes an out value into the stack buffer.
    alignas(4) uint8_t outBuffer[4];
    uint32_t* result = reinterpret_cast<uint32_t*>(
        ((void* (__fastcall*)(void*, void*, uint8_t*))CSF_AT(void**, obj, 0)[47])(obj, nullptr, outBuffer));

    if ((*result & 2) == 0)
    {
        int lookupOffset = static_cast<int>(self->field_000c + 84);
        return reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(
            CharTask_LookupByOffset(self, nullptr, lookupOffset)));
    }
    return result;
}

HOOK(0x004D7000, C_00834fa8_GetFlaggedResultOrLookup, "auto");
