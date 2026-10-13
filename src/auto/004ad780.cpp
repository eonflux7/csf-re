#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// GetLinkedObjectValue: __thiscall-style helper, self passed in ecx.
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));

// Calls vtable slot 0xA0 (index 40) on an object with `this` in ecx.
static int InvokeVirtualSlot40(void* object)
{
    return ((int(__fastcall*)(void*, void*))CSF_AT(void**, object, 0)[40])(object, nullptr);
}

// Dispatches vtable slot 0xA0 on the object that owns the linked value.
// With a linked value: the object is reached via (value - 4)+0x2C -> +0x30.
// Without one: the object is reached via the dword at absolute address 0x2C -> +0x30.
static int __stdcall DispatchLinkedObjectSlot40(int linkedValueArg)
{
    int linkedValue = GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(static_cast<uintptr_t>(linkedValueArg)), nullptr);

    if (linkedValue == 0) {
        void* nullBase = nullptr;
        uint32_t root = CSF_AT(uint32_t, nullBase, 0x2C);
        uint32_t object = CSF_AT(uint32_t, reinterpret_cast<void*>(static_cast<uintptr_t>(root)), 0x30);
        return InvokeVirtualSlot40(reinterpret_cast<void*>(static_cast<uintptr_t>(object)));
    }

    uint8_t* entry = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(linkedValue)) - 4;
    uint32_t owner = CSF_AT(uint32_t, entry, 0x2C);
    uint32_t object = CSF_AT(uint32_t, reinterpret_cast<void*>(static_cast<uintptr_t>(owner)), 0x30);
    return InvokeVirtualSlot40(reinterpret_cast<void*>(static_cast<uintptr_t>(object)));
}

HOOK(0x004AD780, DispatchLinkedObjectSlot40, "auto");
