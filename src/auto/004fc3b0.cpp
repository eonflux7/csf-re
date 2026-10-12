#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));

// Resolves the lookup object used for selection notifications: the owner is read
// from the pointer stored at +0x14 when the flag at +0x1C is set, otherwise null.
static void* ResolveSelectionEntryLookup(C_0083f448* self) {
    C_0085e760* owner = nullptr;
    if (self->field_001c) {
        owner = *reinterpret_cast<C_0085e760**>(self->field_0014);
    }
    int lookup = ResolveEntryLookupValue(owner, nullptr);
    return reinterpret_cast<void*>(static_cast<uintptr_t>(lookup));
}

// Sets the current selection index. Notifies the previously selected entry (vtbl +0x3C)
// if it is valid, stores the new index, then notifies the newly selected entry (vtbl +0x30).
static int __fastcall C_0083f448_SetSelection(C_0083f448* self, void* /*edx*/, int a2) {
    int result = static_cast<int>(self->field_008c);
    if (a2 != result) {
        if (result != -1 && CSF_AT(uint32_t, self, 0x60 + result * 4) != 0) {
            void* lookup = ResolveSelectionEntryLookup(self);
            int previousSlotValue = static_cast<int>(CSF_AT(uint32_t, self, 0x60 + static_cast<int>(self->field_008c) * 4));
            result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, lookup, 0)[15])(lookup, nullptr, previousSlotValue);
        }

        self->field_008c = static_cast<uint32_t>(a2);

        if (a2 != -1) {
            result = static_cast<int>(CSF_AT(uint32_t, self, 0x60 + a2 * 4));
            if (result != 0) {
                void* lookup = ResolveSelectionEntryLookup(self);
                int currentSlotValue = static_cast<int>(CSF_AT(uint32_t, self, 0x60 + static_cast<int>(self->field_008c) * 4));
                return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, lookup, 0)[12])(lookup, nullptr, currentSlotValue);
            }
        }
    }
    return result;
}

HOOK(0x4FC3B0, C_0083f448_SetSelection, "auto");
