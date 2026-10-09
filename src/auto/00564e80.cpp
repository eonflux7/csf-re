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
CSF_FUNC(void**, __fastcall, MenuAccounts_ResolveHeldObject_6683F0, 0x006683F0, (void* self, void* edx));

// Calls the virtual at byte offset Off of the object's vtable.
template <typename R, int Off, typename... A>
static inline R MenuAccounts_VCall(void* obj, A... args)
{
    auto fn = reinterpret_cast<R(__fastcall*)(void*, void*, A...)>((*reinterpret_cast<void***>(obj))[Off / 4]);
    return fn(obj, nullptr, args...);
}

// Computes the offset between the selected target's position and the scroll/viewport
// origin, then forwards it to the viewport object at this+0x3C (slot 0x14C).
// Returns that call's result, or 0 when the first status check (slot 0x30) fails.
static int __fastcall CMenuAccounts_ScrollToTargetDelta(CMenuAccounts* self, void* /*edx*/)
{
    void** holderA = MenuAccounts_ResolveHeldObject_6683F0(reinterpret_cast<uint8_t*>(self) + 4, nullptr);
    int statusOk = MenuAccounts_VCall<int, 0x30>(holderA);
    if (statusOk == 0)
        return statusOk;

    void** targetA = MenuAccounts_ResolveHeldObject_6683F0(reinterpret_cast<uint8_t*>(self) + 4, nullptr);
    void** targetB = MenuAccounts_ResolveHeldObject_6683F0(reinterpret_cast<uint8_t*>(self) + 4, nullptr);

    void* posA = MenuAccounts_VCall<void*, 0x30>(targetA);
    int32_t targetYA = *reinterpret_cast<int32_t*>(static_cast<uint8_t*>(posA) + 0x18);

    void* posB = MenuAccounts_VCall<void*, 0x30>(targetB);
    int32_t targetXB = *reinterpret_cast<int32_t*>(static_cast<uint8_t*>(posB) + 0x14);

    int32_t originBuf[2] = {0, 0};
    int32_t* origin = MenuAccounts_VCall<int32_t*, 0x160>(reinterpret_cast<void*>(self->field_003c), originBuf);
    int32_t deltaY = targetYA - origin[1];
    int32_t deltaX = targetXB - origin[0];

    int32_t delta[2];
    delta[0] = deltaX;
    delta[1] = deltaY;
    // Reload the viewport pointer from this+0x3C, as the original does.
    return MenuAccounts_VCall<int, 0x14C>(reinterpret_cast<void*>(self->field_003c), delta, 1);
}

HOOK(0x00564E80, CMenuAccounts_ScrollToTargetDelta, "auto");

