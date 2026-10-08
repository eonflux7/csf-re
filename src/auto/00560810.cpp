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

// Invokes the virtual method at byte offset `vtblOffset` (slot = offset / 4) on a polymorphic object.
template <typename R, typename... A>
static inline R MenuSessions_VCall(void* obj, int vtblOffset, A... args)
{
    void** vtbl = *reinterpret_cast<void***>(obj);
    auto fn = reinterpret_cast<R(__fastcall*)(void*, void*, A...)>(vtbl[vtblOffset / 4]);
    return fn(obj, nullptr, args...);
}

// Scrolls the list/viewport object at this+0x40 so that it is centered on the offset between
// two held objects' positions. Returns the result of the viewport's final call, or 0 when the
// first status check (vtbl+0x30 on the held object) fails.
static int __fastcall CMenuMpSessions_ScrollListToHeldTarget(CMenuMpSessions* self, void* /*edx*/)
{
    void** heldFirst = GetReferencedObject(reinterpret_cast<uint8_t*>(self) + 4, nullptr);
    int statusOk = MenuSessions_VCall<int>(heldFirst, 0x30);
    if (statusOk == 0)
        return statusOk;

    void** sourceY = GetReferencedObject(reinterpret_cast<uint8_t*>(self) + 4, nullptr);
    void** sourceX = GetReferencedObject(reinterpret_cast<uint8_t*>(self) + 4, nullptr);

    void* posY = MenuSessions_VCall<void*>(sourceY, 0x30);
    int32_t targetY = *reinterpret_cast<int32_t*>(static_cast<uint8_t*>(posY) + 0x18);

    void* posX = MenuSessions_VCall<void*>(sourceX, 0x30);
    int32_t targetX = *reinterpret_cast<int32_t*>(static_cast<uint8_t*>(posX) + 0x14);

    int32_t origin[2] = {0, 0};
    int32_t* originPtr = MenuSessions_VCall<int32_t*>(reinterpret_cast<void*>(self->field_0040), 0x160, origin);

    int32_t deltaY = targetY - originPtr[1];
    int32_t deltaX = targetX - originPtr[0];

    int32_t delta[2];
    delta[0] = deltaX;
    delta[1] = deltaY;
    return MenuSessions_VCall<int>(reinterpret_cast<void*>(self->field_0040), 0x14C, delta, 1);
}

HOOK(0x00560810, CMenuMpSessions_ScrollListToHeldTarget, "auto");
