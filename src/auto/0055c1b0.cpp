#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Resolves the object held behind an embedded member slot (lib helper, __thiscall, no args).
CSF_FUNC(void*, __fastcall, AccountEdit_ResolveHeldObject_6683F0, 0x006683F0, (void* self, void* edx));

// Calls the virtual at byte offset Off of the object's vtable.
template <typename R, int Off, typename... A>
static inline R AccountEdit_VCall(void* obj, A... args)
{
    auto fn = reinterpret_cast<R(__fastcall*)(void*, void*, A...)>((*reinterpret_cast<void***>(obj))[Off / 4]);
    return fn(obj, nullptr, args...);
}

// Reads the positions of two held objects, computes the delta between the target
// position and the origin reported by the scroll/viewport object (slot 0x160),
// and forwards that delta to the viewport (slot 0x14C) with a flag of 1.
// Returns 0 without side effects when the first status check (slot 0x30) fails.
static int __fastcall CMenuAccountEdit_ApplyHeldPositionDelta(CMenuAccountEdit* self, void* /*edx*/)
{
    void* holderStatus = AccountEdit_ResolveHeldObject_6683F0(reinterpret_cast<uint8_t*>(self) + 4, nullptr);
    int statusOk = AccountEdit_VCall<int, 0x30>(holderStatus);
    if (statusOk == 0)
        return statusOk;

    void* holderA = AccountEdit_ResolveHeldObject_6683F0(reinterpret_cast<uint8_t*>(self) + 4, nullptr);
    void* holderB = AccountEdit_ResolveHeldObject_6683F0(reinterpret_cast<uint8_t*>(self) + 4, nullptr);

    void* posA = AccountEdit_VCall<void*, 0x30>(holderA);
    int32_t targetY = *reinterpret_cast<int32_t*>(static_cast<uint8_t*>(posA) + 0x18);

    void* posB = AccountEdit_VCall<void*, 0x30>(holderB);
    int32_t targetX = *reinterpret_cast<int32_t*>(static_cast<uint8_t*>(posB) + 0x14);

    void* viewport = reinterpret_cast<void*>(self->field_0284);
    int32_t origin[2] = {0, 0};
    int32_t* originPtr = AccountEdit_VCall<int32_t*, 0x160>(viewport, origin);

    int32_t deltaY = targetY - originPtr[1];
    int32_t deltaX = targetX - originPtr[0];

    int32_t delta[2];
    delta[0] = deltaX;
    delta[1] = deltaY;

    // The original re-reads [this+0x284] for the final call.
    void* viewportAgain = reinterpret_cast<void*>(self->field_0284);
    return AccountEdit_VCall<int, 0x14C>(viewportAgain, delta, 1);
}

HOOK(0x0055C1B0, CMenuAccountEdit_ApplyHeldPositionDelta, "auto");

