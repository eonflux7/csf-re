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

// Resolves the object held behind this+0x04 (lib helper, __thiscall, no args).
CSF_FUNC(void**, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));

// Invokes virtual slot at byte offset `slot` on a polymorphic object.
template <typename R, typename... A>
static inline R MenuNetType_VCall(void* obj, int slot, A... args)
{
    auto fn = reinterpret_cast<R(__fastcall*)(void*, void*, A...)>((*reinterpret_cast<void***>(obj))[slot / 4]);
    return fn(obj, nullptr, args...);
}

// Aligns the network-type screen's tracked target with the viewport at this+0x38:
// computes (targetB.x - origin.x, targetA.y - origin.y) and forwards it (with flag 1)
// to slot 0x14C of the viewport. Returns 0 when the first status check (slot 0x30 on
// the held object) fails; otherwise returns the result of the forwarding call.
static int __fastcall CMenuMpNetworkType_ScrollViewportToTarget(CMenuMpNetworkType* self, void* /*edx*/)
{
    void** holderStatus = GetReferencedObject(reinterpret_cast<uint8_t*>(self) + 4, nullptr);
    int statusOk = MenuNetType_VCall<int>(holderStatus, 0x30);
    if (statusOk == 0)
        return statusOk;

    void** targetA = GetReferencedObject(reinterpret_cast<uint8_t*>(self) + 4, nullptr);
    void** targetB = GetReferencedObject(reinterpret_cast<uint8_t*>(self) + 4, nullptr);

    void* posA = MenuNetType_VCall<void*>(targetA, 0x30);
    int32_t targetAY = CSF_AT(int32_t, posA, 0x18);

    void* posB = MenuNetType_VCall<void*>(targetB, 0x30);
    int32_t targetBX = CSF_AT(int32_t, posB, 0x14);

    int32_t originBuf[2] = {0, 0};
    int32_t* origin = MenuNetType_VCall<int32_t*>(reinterpret_cast<void*>(self->field_0038), 0x160, originBuf);
    int32_t deltaY = targetAY - origin[1];
    int32_t deltaX = targetBX - origin[0];

    int32_t delta[2];
    delta[0] = deltaX;
    delta[1] = deltaY;
    return MenuNetType_VCall<int>(reinterpret_cast<void*>(self->field_0038), 0x14C, delta, 1);
}

HOOK(0x00534F20, CMenuMpNetworkType_ScrollViewportToTarget, "auto");
