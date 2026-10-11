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

// Callee at 0x5173B0: tests the entry pointed to by a2 and reports whether it was consumed.
CSF_FUNC(char, __fastcall, CProfileManager_TryConsumeEntry, 0x005173B0,
         (void* self, void* edx, unsigned int* a2));

// Returns the result of the vtable slot 0x38 handler when the entry check succeeds,
// otherwise returns 0.
static int __fastcall CProfileManager_ConsumeEntryThenPostHook(CProfileManager* self,
                                                               void* /*edx*/,
                                                               int entryPointer)
{
    const char entryConsumed = CProfileManager_TryConsumeEntry(
        self, nullptr, reinterpret_cast<unsigned int*>(entryPointer));
    if (entryConsumed) {
        // Virtual slot 0x38 -> index 14 of the vtable.
        return ((int(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x38 / 4])(self, nullptr);
    }
    return 0;
}

HOOK(0x005147E0, CProfileManager_ConsumeEntryThenPostHook, "auto");
