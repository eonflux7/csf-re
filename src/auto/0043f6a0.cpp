#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"

// Campaign subobject lookup: returns the address of a slot holding an entry pointer.
CSF_FUNC(void**, __fastcall, Campaign_LookupSubobjectEntry, 0x0043F2A0,
         (void* self, void* edx, void** a2, unsigned int* a3));

// Returns the float stored at offset 0x28 of the entry found for the key in a2,
// or -1.0f when the entry is the sentinel stored at offset 8 of this object.
static float __fastcall Campaign_GetSubobjectFloatOrDefault(void* self, void* /*edx*/, int* a2)
{
    float defaultValue = -1.0f;
    int* keyPtr = a2;

    // The lookup takes the address of the key slot and the key value itself.
    void** lookupSlot = Campaign_LookupSubobjectEntry(
        reinterpret_cast<uint8_t*>(self) + 4, nullptr,
        reinterpret_cast<void**>(&keyPtr),
        reinterpret_cast<unsigned int*>(a2));

    int* entry = reinterpret_cast<int*>(*lookupSlot);
    if (entry == CSF_AT(int*, self, 0x8))
        return defaultValue;

    return CSF_AT(float, entry, 0x28);
}

HOOK(0x43F6A0, Campaign_GetSubobjectFloatOrDefault, "auto");
