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
CSF_FUNC(void**, __fastcall, MenuSaveGame_ResolveHeldObject_6683F0, 0x006683F0, (void* self, void* edx));

// Calls the virtual at byte offset Off of the object's vtable.
template <typename R, int Off, typename... A>
static inline R MenuSaveGame_VCall(void* obj, A... args)
{
    auto fn = reinterpret_cast<R(__fastcall*)(void*, void*, A...)>((*reinterpret_cast<void***>(obj))[Off / 4]);
    return fn(obj, nullptr, args...);
}

// Scrolls the save-game viewport so that the tracked item (object at this+0x04 chain)
// is positioned relative to the viewport origin. Returns the result of the viewport
// scroll call, or 0 when the initial status check (slot 0x30 on the first held object) fails.
static int __fastcall CMenuSaveGame_ScrollToTrackedItem(CMenuSaveGame* self, void* /*edx*/)
{
    void** statusHolder = MenuSaveGame_ResolveHeldObject_6683F0(reinterpret_cast<uint8_t*>(self) + 4, nullptr);
    int statusOk = MenuSaveGame_VCall<int, 0x30>(statusHolder);
    if (statusOk == 0)
        return statusOk;

    void** posHolder = MenuSaveGame_ResolveHeldObject_6683F0(reinterpret_cast<uint8_t*>(self) + 4, nullptr);
    void** itemHolder = MenuSaveGame_ResolveHeldObject_6683F0(reinterpret_cast<uint8_t*>(self) + 4, nullptr);

    void* posA = MenuSaveGame_VCall<void*, 0x30>(posHolder);
    int32_t itemY = *reinterpret_cast<int32_t*>(static_cast<uint8_t*>(posA) + 0x18);

    void* posB = MenuSaveGame_VCall<void*, 0x30>(itemHolder);
    int32_t itemX = *reinterpret_cast<int32_t*>(static_cast<uint8_t*>(posB) + 0x14);

    void* viewport = reinterpret_cast<void*>(self->field_0044);
    int32_t originBuf[2] = {0, 0};
    int32_t* origin = MenuSaveGame_VCall<int32_t*, 0x160>(viewport, originBuf);

    int32_t delta[2];
    delta[0] = itemX - origin[0];
    delta[1] = itemY - origin[1];

    return MenuSaveGame_VCall<int, 0x14C>(reinterpret_cast<void*>(self->field_0044), delta, 1);
}

HOOK(0x005449D0, CMenuSaveGame_ScrollToTrackedItem, "auto");

