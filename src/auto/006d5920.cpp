#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Resolves a resource name to its slot in the variant table. Returns the slot index
// (or 0xFFFFFFFF when not found) and writes the current variant index to *outVariant,
// advancing the round-robin variant counter for the slot.
CSF_FUNC(unsigned int, __fastcall, sub_6D46A0, 0x006D46A0, (void* self, void* edx, char* String2));
CSF_FUNC(int, __fastcall, sub_6D5560, 0x006D5560, (void* self, void* edx, char* String2, int a3));

static unsigned int __fastcall GfxScene_ResolveResourceWithVariant(S_006d4450* self, void* /*edx*/, char* resourceName, uint32_t* outVariant)
{
    unsigned int slot = sub_6D46A0(self, nullptr, resourceName);

    if (slot == 0xFFFFFFFFu)
    {
        int newSlot = sub_6D5560(self, nullptr, resourceName, -1);
        slot = static_cast<unsigned int>(newSlot);

        if (newSlot != -1)
        {
            // Notify the owner's vtable slot 4 with the newly created entry address.
            uint32_t entryAddress = self->field_0008 + static_cast<uint32_t>(newSlot) * 60u;
            ((void(__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, self, 0)[4])(self, nullptr, entryAddress);
        }

        if (slot == 0xFFFFFFFFu)
        {
            *outVariant = 0xFFFFFFFFu;
            return slot;
        }
    }

    uint8_t* entry = reinterpret_cast<uint8_t*>(self->field_0008) + static_cast<size_t>(slot) * 60u;
    uint32_t variantCount = CSF_AT(uint32_t, entry, 0x34);

    if (variantCount == 0)
    {
        *outVariant = 0xFFFFFFFFu;
        return slot;
    }

    *outVariant = CSF_AT(uint32_t, entry, 0x38);
    uint32_t nextVariant = (CSF_AT(uint32_t, entry, 0x38) + 1u) % (variantCount + 1u);
    CSF_AT(uint32_t, entry, 0x38) = nextVariant;
    return slot;
}

HOOK(0x006D5920, GfxScene_ResolveResourceWithVariant, "auto");
