#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180, (void* self, void* edx, int a2));

// Resolves the linked object's value (or the base value 456 when none is linked)
// and applies it through UnnamedApply_4DC180 on the same object.
static int __stdcall AiMoveTasks_ApplyLinkedShortOffset(int ownerObject) {
    void* self = reinterpret_cast<void*>(static_cast<uintptr_t>(ownerObject));

    int linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    if (linkedValue) {
        return UnnamedApply_4DC180(self, nullptr, linkedValue - 4 + 456);
    }
    return UnnamedApply_4DC180(self, nullptr, 456);
}

HOOK(0x004ABA50, AiMoveTasks_ApplyLinkedShortOffset, "auto");
