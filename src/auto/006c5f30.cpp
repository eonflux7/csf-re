#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Advances every non-null entry in the pointer array at field_0060 by calling
// its virtual slot 5 (vtable offset 0x14). The count lives in field_0068 and is
// re-read on every iteration, as the original does. field_011c accumulates the
// count each call. Returns the last value read from field_0068.
static unsigned int __fastcall S_006c5cd0_AdvanceAndUpdateEntries(S_006c5cd0* self, void* /*edx*/)
{
    uint32_t result = self->field_0068;
    self->field_011c += result;

    if (result != 0) {
        for (uint32_t i = 0; i < result; ++i) {
            uint32_t entryAddr = reinterpret_cast<uint32_t*>(self->field_0060)[i];
            void* entry = reinterpret_cast<void*>(entryAddr);
            if (entry != nullptr) {
                // Virtual slot 5 (byte offset 0x14), __thiscall with no stack args.
                auto updateFn = reinterpret_cast<void (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, entry, 0)[5]);
                updateFn(entry, nullptr);
            }
            result = self->field_0068;
        }
    }
    return result;
}

HOOK(0x6C5F30, S_006c5cd0_AdvanceAndUpdateEntries, "auto");
