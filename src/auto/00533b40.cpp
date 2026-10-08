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

// Resolves the helper object held behind this+0x04 (lib, __thiscall, no stack args).
CSF_FUNC(void**, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));

// Calls the virtual method at byte offset `byteOffset` (slot = offset / 4) on a polymorphic object.
template <typename R, typename... A>
static inline R MenuPause_VCall(void* obj, int byteOffset, A... args)
{
    void* method = (*reinterpret_cast<void***>(obj))[byteOffset / 4];
    auto fn = reinterpret_cast<R(__fastcall*)(void*, void*, A...)>(method);
    return fn(obj, nullptr, args...);
}

// Scrolls the view object stored at this+0x30 by the offset between the tracked
// target's position (via two held objects) and the view's current origin.
// Returns the result of the view's slot 0x14C call, or 0 when the first status check fails.
static int __fastcall MenuPause_ScrollViewToTrackedDelta(void* self, void* /*edx*/)
{
    uint8_t* base = static_cast<uint8_t*>(self);

    void** statusHolder = GetReferencedObject(base + 4, nullptr);
    int statusOk = MenuPause_VCall<int>(statusHolder, 0x30);
    if (statusOk == 0)
        return statusOk;

    void** positionHolder = GetReferencedObject(base + 4, nullptr);
    void** targetHolder = GetReferencedObject(base + 4, nullptr);

    void* positionObj = MenuPause_VCall<void*>(positionHolder, 0x30);
    int32_t trackedY = *reinterpret_cast<int32_t*>(static_cast<uint8_t*>(positionObj) + 0x18);

    void* targetObj = MenuPause_VCall<void*>(targetHolder, 0x30);
    int32_t trackedX = *reinterpret_cast<int32_t*>(static_cast<uint8_t*>(targetObj) + 0x14);

    int32_t origin[2] = {0, 0};
    int32_t* originPtr = MenuPause_VCall<int32_t*>(CSF_AT(void*, self, 0x30), 0x160, origin);

    int32_t deltaY = static_cast<int32_t>(static_cast<uint32_t>(trackedY) - static_cast<uint32_t>(originPtr[1]));
    int32_t deltaX = static_cast<int32_t>(static_cast<uint32_t>(trackedX) - static_cast<uint32_t>(originPtr[0]));

    int32_t delta[2];
    delta[0] = deltaX;
    delta[1] = deltaY;
    return MenuPause_VCall<int>(CSF_AT(void*, self, 0x30), 0x14C, delta, 1);
}

HOOK(0x00533B40, MenuPause_ScrollViewToTrackedDelta, "auto");
