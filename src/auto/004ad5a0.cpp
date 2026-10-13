#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, Dispatch_NoLinkedTarget, 0x004DBCB0, (void* self, void* edx, int a2));

static uint32_t* __stdcall QueryLinkedObjectFlagOrDispatch(int a1, int /*a2*/)
{
    // The original loads its object handle once into edi and reuses it for the later dispatch call,
    // even though the flag query may write back through &a1.
    const int objectHandle = a1;

    const int linkedValue = GetLinkedObjectValue_4DB5C0(
        reinterpret_cast<void*>(static_cast<intptr_t>(objectHandle)), nullptr);
    const int v3 = linkedValue ? linkedValue - 4 : 0;

    const uint32_t inner = CSF_AT(uint32_t, reinterpret_cast<void*>(static_cast<intptr_t>(v3)), 0x2C);
    void* v4 = reinterpret_cast<void*>(
        static_cast<intptr_t>(CSF_AT(uint32_t, reinterpret_cast<void*>(static_cast<intptr_t>(inner)), 0x30)));

    void** vtbl = CSF_AT(void**, v4, 0);
    const bool isBlocked = ((bool(__fastcall*)(void*, void*))vtbl[46])(v4, nullptr);
    if (isBlocked)
        return reinterpret_cast<uint32_t*>(static_cast<intptr_t>(
            Dispatch_NoLinkedTarget(reinterpret_cast<void*>(static_cast<intptr_t>(objectHandle)), nullptr, 1)));

    uint32_t* result = ((uint32_t*(__fastcall*)(void*, void*, int*))vtbl[47])(v4, nullptr, &a1);
    if ((*result & 2) == 0)
        return reinterpret_cast<uint32_t*>(static_cast<intptr_t>(
            Dispatch_NoLinkedTarget(reinterpret_cast<void*>(static_cast<intptr_t>(objectHandle)), nullptr, 1)));

    return result;
}

HOOK(0x004AD5A0, QueryLinkedObjectFlagOrDispatch, "auto");
