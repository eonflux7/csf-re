#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, LinkedObject_ProcessEntry, 0x004CAAD0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Dispatch_NoLinkedTarget, 0x004DBCB0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Dispatch_LinkedTarget, 0x004DC380, (void* self, void* edx, int a2));

// Returns the object linked at +0 of the task (GetLinkedObjectField10 result) as a byte pointer.
static uint8_t* GetLinkedObject(C_0085e760* self) {
    return reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetLinkedObjectField10(self, nullptr)));
}

static int __stdcall C_0085e760_ResetAndDispatch(C_0085e760* self, int /*a2*/) {
    // Clear the three counters inside the linked object's sub-block at +0x6C.
    uint8_t* linked = GetLinkedObject(self);
    uint8_t* counters = linked + 0x6C;
    CSF_AT(int, counters, 0x8) = 0;
    CSF_AT(int, counters, 0x18) = 0;
    CSF_AT(int, counters, 0x28) = 0;

    // Process entries while the linked object's flag at +0xD4 is set and processing continues.
    for (;;) {
        uint8_t* current = GetLinkedObject(self);
        if (CSF_AT(int, current, 0xD4) == 0)
            break;
        int entryValue = ResolveEntryLookupValue(self, nullptr);
        uint8_t* refreshed = GetLinkedObject(self);
        if (!LinkedObject_ProcessEntry(refreshed, nullptr, entryValue))
            break;
    }

    // Dispatch based on the target field at +0x74.
    uint8_t* afterLoop = GetLinkedObject(self);
    if (CSF_AT(int, afterLoop, 0x74) == 0)
        return Dispatch_NoLinkedTarget(self, nullptr, 0);

    uint8_t* target = GetLinkedObject(self);
    return Dispatch_LinkedTarget(self, nullptr,
                                 static_cast<int>(reinterpret_cast<uintptr_t>(target + 8)));
}

HOOK(0x4CAF60, C_0085e760_ResetAndDispatch, "auto");
