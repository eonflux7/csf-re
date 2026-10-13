#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));

// Resolves the linked object (offset -4 when non-null, else null base), then forwards
// a2 to the virtual slot at byte offset 0x90 (slot 36) of the object stored at +0x718.
static int __stdcall Order_ForwardToLinkedMemberSlot36(int a1, int a2) {
    int linkedObjectValue = GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(static_cast<uintptr_t>(a1)), nullptr);

    int baseValue;
    if (linkedObjectValue != 0) {
        baseValue = linkedObjectValue - 4;
    } else {
        baseValue = 0;
    }

    void* base = reinterpret_cast<void*>(static_cast<uintptr_t>(baseValue));
    void* memberObject = CSF_AT(void*, base, 0x718);

    return ((int(__fastcall*)(void*, void*, int))(CSF_AT(void**, memberObject, 0)[0x90 / 4]))(
        memberObject, nullptr, a2);
}

HOOK(0x004ACD20, Order_ForwardToLinkedMemberSlot36, "auto");
