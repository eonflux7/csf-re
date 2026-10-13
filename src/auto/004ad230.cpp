#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));

// Resolves the linked object for a1 (base = value - 4, or null when none), then
// calls vtable slot 40 on the object at base+0x2C -> +0x30, and finally returns
// the result of vtable slot 35 on the object at base+0x2C.
static int __stdcall TOrden_UpdateLinkedObjectAndDispatch(int a1)
{
    void* ownerSelf = reinterpret_cast<void*>(static_cast<uintptr_t>(a1));

    int linkedValue = GetLinkedObjectValue_4DB5C0(ownerSelf, nullptr);
    int linkedBase = linkedValue ? linkedValue - 4 : 0;
    void* linkedMember = CSF_AT(void*, reinterpret_cast<void*>(static_cast<uintptr_t>(linkedBase)), 0x2C);
    void* memberChild = CSF_AT(void*, linkedMember, 0x30);
    ((void(__fastcall*)(void*, void*))(CSF_AT(void**, memberChild, 0)[40]))(memberChild, nullptr);

    int linkedValue2 = GetLinkedObjectValue_4DB5C0(ownerSelf, nullptr);
    int linkedBase2 = linkedValue2 ? linkedValue2 - 4 : 0;
    void* dispatchObj = CSF_AT(void*, reinterpret_cast<void*>(static_cast<uintptr_t>(linkedBase2)), 0x2C);

    return ((int(__fastcall*)(void*, void*, int, int, int, int))(CSF_AT(void**, dispatchObj, 0)[35]))(
        dispatchObj, nullptr, 0x12, 0, -1, 0);
}

HOOK(0x4AD230, TOrden_UpdateLinkedObjectAndDispatch, "auto");
