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
CSF_FUNC(void**, __fastcall, MenuProfiles_ResolveHeldObject_6683F0, 0x006683F0, (void* self, void* edx));

// Invokes virtual slot `slot` (byte offset / 4) on a polymorphic object.
// Calling convention matches the original __thiscall virtual calls (ecx = this, stack args).
template <typename R, typename... A>
static inline R MenuProfiles_VCall(void* obj, int slot, A... args)
{
    auto fn = reinterpret_cast<R(__fastcall*)(void*, void*, A...)>((*reinterpret_cast<void***>(obj))[slot / 4]);
    return fn(obj, nullptr, args...);
}

// Checks the held object's status (slot 0x30). If it is nonzero, reads the positions of the two
// held objects, computes their offset from the scroll/viewport origin at this+0x24 (slot 0x160),
// and forwards that offset as a 2-int vector to the viewport's slot 0x14C.
// Returns the result of that forwarding call, or 0 when the status check fails.
static int __fastcall CMenuProfiles_SyncListboxScrollToSelection(CMenuProfiles* self, void* /*edx*/)
{
    uint8_t* selfBytes = reinterpret_cast<uint8_t*>(self);

    void** statusHolder = MenuProfiles_ResolveHeldObject_6683F0(selfBytes + 4, nullptr);
    int statusOk = MenuProfiles_VCall<int>(statusHolder, 0x30);
    if (statusOk == 0)
        return statusOk;

    void** holderA = MenuProfiles_ResolveHeldObject_6683F0(selfBytes + 4, nullptr);
    void** holderB = MenuProfiles_ResolveHeldObject_6683F0(selfBytes + 4, nullptr);

    void* posA = MenuProfiles_VCall<void*>(holderA, 0x30);
    int32_t posAY = CSF_AT(int32_t, posA, 0x18);

    void* posB = MenuProfiles_VCall<void*>(holderB, 0x30);
    int32_t posBX = CSF_AT(int32_t, posB, 0x14);

    void* viewport = CSF_AT(void*, selfBytes, 0x24);
    int32_t originBuf[2] = {0, 0};
    int32_t* origin = MenuProfiles_VCall<int32_t*>(viewport, 0x160, originBuf);

    int32_t deltaY = posAY - origin[1];
    int32_t deltaX = posBX - origin[0];

    // The original reloads this+0x24 before the final call.
    void* viewportAgain = CSF_AT(void*, selfBytes, 0x24);
    int32_t delta[2];
    delta[0] = deltaX;
    delta[1] = deltaY;
    return MenuProfiles_VCall<int>(viewportAgain, 0x14C, delta, 1);
}

HOOK(0x0053BBD0, CMenuProfiles_SyncListboxScrollToSelection, "auto");
