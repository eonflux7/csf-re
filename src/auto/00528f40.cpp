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

// Resolves the tracked object held behind this+0x04 (lib helper, __thiscall, no args).
CSF_FUNC(void**, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));

// Invokes the virtual method at byte offset `byteOffset` (slot = byteOffset / 4) on a polymorphic object.
template <typename R, typename... A>
static inline R MessageBox_VCall(void* obj, int byteOffset, A... args)
{
    void** vtbl = *reinterpret_cast<void***>(obj);
    auto fn = reinterpret_cast<R(__fastcall*)(void*, void*, A...)>(vtbl[byteOffset / 4]);
    return fn(obj, nullptr, args...);
}

// Computes the offset between two tracked objects' positions and forwards it, together with
// the flag 1, to the object stored at this+0x894 (vtable offset 0x14C). Returns that call's
// result, or 0 when the first status check (vtable offset 0x30 on the held object) fails.
static int __fastcall CMenuMessageBox_CenterOnTarget(CMenuMessageBox* self, void* /*edx*/)
{
    void** statusHolder = GetReferencedObject(reinterpret_cast<uint8_t*>(self) + 4, nullptr);
    int statusOk = MessageBox_VCall<int>(statusHolder, 0x30);
    if (statusOk == 0)
        return statusOk;

    void** yHolder = GetReferencedObject(reinterpret_cast<uint8_t*>(self) + 4, nullptr);
    void** xHolder = GetReferencedObject(reinterpret_cast<uint8_t*>(self) + 4, nullptr);

    void* yPos = MessageBox_VCall<void*>(yHolder, 0x30);
    int32_t targetY = *reinterpret_cast<int32_t*>(static_cast<uint8_t*>(yPos) + 0x18);

    void* xPos = MessageBox_VCall<void*>(xHolder, 0x30);
    int32_t targetX = *reinterpret_cast<int32_t*>(static_cast<uint8_t*>(xPos) + 0x14);

    int32_t scratch[2] = {0, 0};
    int32_t* origin = MessageBox_VCall<int32_t*>(reinterpret_cast<void*>(self->field_0894), 0x160, scratch);

    int32_t deltaY = targetY - origin[1];
    int32_t deltaX = targetX - origin[0];

    int32_t delta[2];
    delta[0] = deltaX;
    delta[1] = deltaY;
    return MessageBox_VCall<int>(reinterpret_cast<void*>(self->field_0894), 0x14C, delta, 1);
}

HOOK(0x00528F40, CMenuMessageBox_CenterOnTarget, "auto");
