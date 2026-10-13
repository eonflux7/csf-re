#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));

// Looks up the linked object, rebases it by -4, takes the sub-object at +0x2C,
// and invokes its virtual slot 35 (byte offset 0x8C) with (17, 0, -1, 0).
static int __stdcall AiOrder_InvokeLinkedObjectVirtualSlot35(int a1)
{
    int linkedValue = GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(static_cast<uintptr_t>(a1)), nullptr);

    uint8_t* base = nullptr;
    if (linkedValue != 0)
        base = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(linkedValue - 4));

    void* subObject = CSF_AT(void*, base, 0x2C);

    using SlotFn = int(__fastcall*)(void*, void*, int, int, int, int);
    SlotFn slot = reinterpret_cast<SlotFn>(CSF_AT(void**, subObject, 0)[0x8C / 4]);
    return slot(subObject, nullptr, 17, 0, -1, 0);
}

HOOK(0x004AD300, AiOrder_InvokeLinkedObjectVirtualSlot35, "auto");
