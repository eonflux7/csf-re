#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));

// Vtable of the grow-array class (referenced by the original's code path around the grow call).
[[maybe_unused]] static const void* kGrowArrayVtable = CSF_VTBL(0x0041E5F0);

// Resolves the linked object and returns its base (linked - 4), or null when there is no link.
static uint8_t* AiCore_LinkedBaseOrNull(void* self)
{
    int linked = GetLinkedObjectValue_4DB5C0(self, nullptr);
    if (linked != 0)
        return reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(static_cast<intptr_t>(linked) - 4));
    return nullptr;
}

static int __stdcall AiCore_ApplyLinkedStateAndPushFlag(int a1)
{
    void* self = reinterpret_cast<void*>(static_cast<uintptr_t>(a1));

    // First: owner->sub->vtbl[160 / 4](owner)
    uint8_t* base = AiCore_LinkedBaseOrNull(self);
    void* subObject = reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, base, 0x2c)));
    void* ownerObject = reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, subObject, 0x30)));
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, ownerObject, 0)[160 / 4])(ownerObject, nullptr);

    // Second: apply to the linked object's embedded block at +0x440.
    base = AiCore_LinkedBaseOrNull(self);
    UnnamedApply_4DC180(self, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(base) + 0x440));

    // Third: append a flag (value 1) to the growable array at +0x50c.
    base = AiCore_LinkedBaseOrNull(self);
    uint8_t* list = base + 0x50c;
    uint32_t capacity = CSF_AT(uint32_t, base, 0x510);
    uint32_t count = CSF_AT(uint32_t, list, 8);
    if (count >= capacity)
        S_0041e5f0_GrowArrayCapacity(reinterpret_cast<S_0041e5f0*>(list), nullptr, CSF_AT(int, list, 0xc));

    uint32_t index = CSF_AT(uint32_t, list, 8);
    uint32_t* slots = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, list, 0)));
    CSF_AT(uint32_t, list, 8) = index + 1;
    slots[index] = 1;
    return static_cast<int>(index + 1);
}

HOOK(0x004BFAD0, AiCore_ApplyLinkedStateAndPushFlag, "auto");
