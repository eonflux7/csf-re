#include <cstddef>
#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, C_00863044_EraseEntryCompacting, 0x006FE8F0, (C_00863044* self, void* , int a2));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

// Removes entry `item` from the pointer list held at +0x54 (count at +0x5C), then
// releases the item through its vtable slot 1. Returns immediately if the item
// is not found in the list.
static void __fastcall C_00861120_RemoveItem(C_00861120* self, void* /*edx*/, int a2)
{
    if (a2 == 0)
        return;

    // Sub-object at +0x74 is the item's owner/registry; erase from it first.
    C_00863044_EraseEntryCompacting(
        reinterpret_cast<C_00863044*>(reinterpret_cast<uint8_t*>(self) + 0x74), nullptr, a2);

    uint32_t count = self->field_005c;
    uint32_t index = 0;
    if (count == 0)
        return;

    uint8_t* listBytes = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_0054));
    uint32_t* list = reinterpret_cast<uint32_t*>(listBytes);

    while (list[index] != static_cast<uint32_t>(a2))
    {
        if (++index >= count)
            return;
    }

    if (index == 0xFFFFFFFFu)
        return;

    uint32_t newCount = self->field_005c - 1;
    self->field_005c = newCount;
    if (index != newCount)
    {
        uint8_t* slot = listBytes + 4 * index;
        _memcpy(slot, slot + 4, 4 * (newCount - index));
    }

    void* item = reinterpret_cast<void*>(static_cast<uintptr_t>(a2));
    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(CSF_AT(void**, item, 0)[1])(item, nullptr, 1);
}

HOOK(0x006D6610, C_00861120_RemoveItem, "auto");
