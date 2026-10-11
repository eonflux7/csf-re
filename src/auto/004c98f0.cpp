#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __stdcall, sub_4C5B80, 0x004C5B80, (int a1));

// Resolves the linked object: the linked value minus 4, or null when there is no link.
static uint8_t* ResolveLinkedObjectBase(int linkedHandle) {
    void* handle = reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(linkedHandle)));
    int linkedValue = GetLinkedObjectValue_4DB5C0(handle, nullptr);
    if (linkedValue) {
        return reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(linkedValue - 4)));
    }
    return nullptr;
}

static int __stdcall CoverAttackTasks_ResetLinkedObjectAndRun(int a1) {
    // First linked object: the sub-object at +0x2C gets a virtual call (slot 35) with (0, 0, -1, 0).
    uint8_t* firstBase = ResolveLinkedObjectBase(a1);
    void* firstSubObject = CSF_AT(void*, firstBase, 0x2C);
    using FirstSlotFn = void(__fastcall*)(void*, void*, int, int, int, int);
    FirstSlotFn firstSlot = reinterpret_cast<FirstSlotFn>(CSF_AT(void**, firstSubObject, 0)[35]);
    firstSlot(firstSubObject, nullptr, 0, 0, -1, 0);

    // Second linked object: embedded object at +0x2D4 gets a virtual call (slot 10) with a1.
    uint8_t* secondBase = ResolveLinkedObjectBase(a1);
    uint8_t* embeddedObject = secondBase + 0x2D4;
    using SecondSlotFn = void(__fastcall*)(void*, void*, int);
    SecondSlotFn secondSlot = reinterpret_cast<SecondSlotFn>(CSF_AT(void**, embeddedObject, 0)[10]);
    secondSlot(embeddedObject, nullptr, a1);

    // Third linked object: scale fields at +0x2E0 and +0x2E4 are set to 1.0f.
    uint8_t* thirdBase = ResolveLinkedObjectBase(a1);
    CSF_AT(float, thirdBase, 0x2E0) = 1.0f;
    CSF_AT(float, thirdBase, 0x2E4) = 1.0f;

    return sub_4C5B80(a1);
}

HOOK(0x004C98F0, CoverAttackTasks_ResetLinkedObjectAndRun, "auto");
