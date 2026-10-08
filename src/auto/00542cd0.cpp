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
CSF_FUNC(void**, __fastcall, MpOnline_ResolveHeldObject_6683F0, 0x006683F0, (void* self, void* edx));

// Reads the function pointer at byte offset `byteOffset` in the vtable of `obj` and calls it
// with the object as `this`. Byte offset / 4 is the vtable slot index.
template <typename R, typename... A>
static inline R MpOnline_VCall(void* obj, int byteOffset, A... args)
{
    void** vtbl = *reinterpret_cast<void***>(obj);
    void* fnPtr = *reinterpret_cast<void**>(reinterpret_cast<uint8_t*>(vtbl) + byteOffset);
    auto fn = reinterpret_cast<R(__fastcall*)(void*, void*, A...)>(fnPtr);
    return fn(obj, nullptr, args...);
}

// Scrolls the view controller (this+0x14) so that the difference between two tracked
// objects' positions is passed to its slot 0x14C method. Returns that call's result, or 0
// when the first status check (slot 0x30 on the first held object) fails.
static int __fastcall CMenuMpOnline_ScrollViewToTarget(CMenuMpOnline* self, void* /*edx*/)
{
    void** holderStatus = MpOnline_ResolveHeldObject_6683F0(reinterpret_cast<uint8_t*>(self) + 4, nullptr);
    int statusOk = MpOnline_VCall<int>(holderStatus, 0x30);
    if (statusOk == 0)
        return statusOk;

    void** targetA = MpOnline_ResolveHeldObject_6683F0(reinterpret_cast<uint8_t*>(self) + 4, nullptr);
    void** targetB = MpOnline_ResolveHeldObject_6683F0(reinterpret_cast<uint8_t*>(self) + 4, nullptr);

    void* posA = MpOnline_VCall<void*>(targetA, 0x30);
    int32_t targetY = *reinterpret_cast<int32_t*>(static_cast<uint8_t*>(posA) + 0x18);

    void* posB = MpOnline_VCall<void*>(targetB, 0x30);
    int32_t targetX = *reinterpret_cast<int32_t*>(static_cast<uint8_t*>(posB) + 0x14);

    void* controller = reinterpret_cast<void*>(self->field_0014);
    int32_t scratch[2];
    int32_t* origin = MpOnline_VCall<int32_t*>(controller, 0x160, scratch);
    int32_t deltaY = targetY - origin[1];
    int32_t deltaX = targetX - origin[0];

    int32_t delta[2];
    delta[0] = deltaX;
    delta[1] = deltaY;

    void* controllerAgain = reinterpret_cast<void*>(self->field_0014);
    return MpOnline_VCall<int>(controllerAgain, 0x14C, delta, 1);
}

HOOK(0x00542CD0, CMenuMpOnline_ScrollViewToTarget, "auto");
