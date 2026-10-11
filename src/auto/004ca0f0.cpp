#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// Callees (bound by address)
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GetStateValue, 0x0050DD30, (void* self, void* edx, int a2));
CSF_FUNC(int, __stdcall, sub_4C69C0, 0x004C69C0, (int a1));
CSF_FUNC(void, __stdcall, sub_4C6B70, 0x004C6B70, (int a1));

// Dispatches a cover-attack step. If the linked object's state fields are all
// valid, or the global state value is below 50, it tail-calls sub_4C69C0 and
// passes its eax result back to the caller. Otherwise it calls sub_4C6B70.
static int __stdcall CoverAttackTasks_DispatchStep(int a1)
{
    void* linkedSelf = reinterpret_cast<void*>(static_cast<uintptr_t>(a1));
    int linkedValue = GetLinkedObjectValue_4DB5C0(linkedSelf, nullptr);

    uint8_t* obj = nullptr;
    if (linkedValue != 0)
        obj = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(linkedValue - 4));

    if (CSF_AT(int, obj, 0x6C8) != 0
        && CSF_AT(int, obj, 0x6C0) != -1
        && CSF_AT(int, obj, 0x6C4) != -1)
    {
        return sub_4C69C0(a1);
    }

    int globalState = GetGlobalState_50D190();
    void* globalSelf = reinterpret_cast<void*>(static_cast<uintptr_t>(globalState));
    uint32_t stateValue = static_cast<uint32_t>(GetStateValue(globalSelf, nullptr, 100));

    if (stateValue < 0x32u)
        return sub_4C69C0(a1);

    sub_4C6B70(a1);
    return 0;
}

HOOK(0x004CA0F0, CoverAttackTasks_DispatchStep, "auto");
