#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(double, __stdcall, GetSceneObjectFloatAt08, 0x004E9800, (uint32_t* a1));
CSF_FUNC(double, __stdcall, GetSceneObjectFloatAt0C, 0x004E9830, (uint32_t* a1));
CSF_FUNC(int, __stdcall, GetSceneObjectIntAt10, 0x004E9860, (uint32_t* a1));
CSF_FUNC(int, __stdcall, GetSceneObjectIntAt14, 0x004E9890, (uint32_t* a1));
CSF_FUNC(double, __stdcall, GetSceneObjectFloatAt04, 0x004E98C0, (uint32_t* a1));

static int __stdcall SendStateEventToHandleObject(int /*a1*/, int /*a2*/, int a3)
{
    // a3 -> slot holding a pointer to a record; that record's +4 field is a second record.
    const uint32_t slotTarget = *reinterpret_cast<uint32_t*>(a3);
    const uint32_t recordAddress = *reinterpret_cast<uint32_t*>(slotTarget + 4);
    uint8_t* record = reinterpret_cast<uint8_t*>(recordAddress);

    const int32_t recordIndex = CSF_AT(int32_t, record, 0x0C);
    const int handleValue = static_cast<int>(recordAddress + 4);
    if (recordIndex < 0)
        return handleValue;

    const int resolvedValue = CHandleTable_Resolve(g_pCurrentScene, nullptr, handleValue);
    if (resolvedValue == 0)
        return 0;

    uint32_t* resolved = reinterpret_cast<uint32_t*>(resolvedValue);
    if (CSF_AT(uint32_t, resolved, 0x34) == 0)
        return 0;

    // Event block sent to the sub-object's virtual method (24 bytes).
    struct StateEventPayload {
        int32_t kind;        // +0x00: always 14
        float floatAt04;    // +0x04: from GetSceneObjectFloatAt04
        float floatAt08;    // +0x08: from GetSceneObjectFloatAt08
        float floatAt0C;    // +0x0C: from GetSceneObjectFloatAt0C
        int32_t intAt10;    // +0x10: from GetSceneObjectIntAt10
        int32_t intAt14;    // +0x14: from GetSceneObjectIntAt14
    };

    StateEventPayload payload{};
    payload.kind = 14;

    GetValue8B0554();
    payload.floatAt08 = static_cast<float>(GetSceneObjectFloatAt08(resolved));
    GetValue8B0554();
    payload.floatAt0C = static_cast<float>(GetSceneObjectFloatAt0C(resolved));
    GetValue8B0554();
    payload.intAt10 = GetSceneObjectIntAt10(resolved);
    GetValue8B0554();
    payload.intAt14 = GetSceneObjectIntAt14(resolved);
    GetValue8B0554();
    payload.floatAt04 = static_cast<float>(GetSceneObjectFloatAt04(resolved));

    // The original reloads the sub-object pointer after the helper calls.
    void* targetObject = CSF_AT(void*, resolved, 0x34);
    using StateEventFn = int(__fastcall*)(void* self, void* edx, StateEventPayload* eventPayload);
    StateEventFn method = (StateEventFn)(CSF_AT(void**, targetObject, 0)[42]);
    return method(targetObject, nullptr, &payload);
}

HOOK(0x0045EA70, SendStateEventToHandleObject, "auto");
