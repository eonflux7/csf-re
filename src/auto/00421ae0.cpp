#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, sub_4DB110, 0x004DB110, (void* self, void* edx));

// Returns the owner's sub-object at +0x14 (the character state holder).
static void* TCharTask_GetOwnerSubObject(uint32_t owner)
{
    return reinterpret_cast<void*>(CSF_AT(uint32_t, reinterpret_cast<void*>(owner), 0x14));
}

// Clears bit 0x10 of the owner sub-object's flags at +0x1F0 and returns a pointer to that flags dword.
// If the sub-object's current action reports the owner as active, it is notified via slot 15.
static uint32_t* __fastcall TCharTask_ClearFlag10(TCharTask* self, void* /*edx*/)
{
    sub_4DB110(self, nullptr);

    uint32_t owner = CSF_AT(uint32_t, self, 0x08);

    void* stateHolder = TCharTask_GetOwnerSubObject(owner);
    void* stateObj = reinterpret_cast<void*>(CSF_AT(uint32_t, stateHolder, 0x48));
    uint32_t currentOwner = ((uint32_t(__fastcall*)(void*, void*))CSF_AT(void**, stateObj, 0)[16])(stateObj, nullptr);

    if (currentOwner == owner)
    {
        void* holder2 = TCharTask_GetOwnerSubObject(CSF_AT(uint32_t, self, 0x08));
        void* stateObj2 = reinterpret_cast<void*>(CSF_AT(uint32_t, holder2, 0x48));
        ((void(__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, stateObj2, 0)[15])(stateObj2, nullptr, owner);
    }

    void* ownerHolder = TCharTask_GetOwnerSubObject(CSF_AT(uint32_t, self, 0x08));
    uint32_t* flags = &CSF_AT(uint32_t, ownerHolder, 0x1F0);
    *flags = *flags & 0xFFFFFFEFu;
    return flags;
}

HOOK(0x00421AE0, TCharTask_ClearFlag10, "auto");
