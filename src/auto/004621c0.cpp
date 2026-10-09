#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Local message record built on the stack: vtable at +0, then three payload dwords.
struct GlobalValueMessage {
    void* vtbl;
    uint32_t payloadA;
    uint32_t payloadB;
    uint32_t payloadC;
};

CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(char, __fastcall, IsGlobalValueReady, 0x00521C00, (void* self, void* edx));
CSF_FUNC(int, __fastcall, DispatchGlobalValueMessage, 0x00522330, (void* self, void* edx, int a2));

static char __stdcall NotifyGlobalValueUpdate(int a1, int a2, uint32_t* a3)
{
    (void)a1;
    (void)a2;

    // Read the source chain (no calls made yet, matching the original load order).
    uint32_t* first = reinterpret_cast<uint32_t*>(a3[0]);
    uintptr_t recordBase = static_cast<uintptr_t>(first[1]) + 4;
    uint8_t* record = reinterpret_cast<uint8_t*>(recordBase);

    uint32_t* second = reinterpret_cast<uint32_t*>(a3[1]);
    uint32_t* secondInner = reinterpret_cast<uint32_t*>(second[1]);
    uint32_t sourceValue = secondInner[1];

    C_00863678* globalValue = reinterpret_cast<C_00863678*>(GetGlobalValue_521BB0());
    char isReady = IsGlobalValueReady(globalValue, nullptr);
    if (!isReady)
    {
        return isReady;
    }

    GlobalValueMessage message;
    message.vtbl = CSF_VTBL(0x0082D448);
    message.payloadA = CSF_AT(uint32_t, record, 4);
    message.payloadB = CSF_AT(uint32_t, record, 8);
    message.payloadC = sourceValue;

    void* dispatchTarget = reinterpret_cast<void*>(GetGlobalValue_521BB0());
    int dispatchResult = DispatchGlobalValueMessage(
        dispatchTarget, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(&message)));
    return static_cast<char>(dispatchResult);
}

HOOK(0x4621C0, NotifyGlobalValueUpdate, "auto");
