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
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));

// Computes the offset between two tracked objects' positions and forwards it, together with
// the flag 1, to the object stored at this+0x13C (vtable offset 0x14C). Returns that call's
// result, or 0 when the first status check (vtable offset 0x30 on the held object) fails.
static int __fastcall InstancingSceneResult_ForwardOffset(void* self, void* /*edx*/)
{
    // Status check on the object tracked behind this+0x04.
    void* statusHolder = GetReferencedObject(static_cast<uint8_t*>(self) + 4, nullptr);
    int statusOk = reinterpret_cast<int(__fastcall*)(void*, void*)>(
        CSF_AT(void**, statusHolder, 0)[0x30 / 4])(statusHolder, nullptr);
    if (statusOk == 0)
        return statusOk;

    // Two positions: first holder supplies Y, second supplies X.
    void* yHolder = GetReferencedObject(static_cast<uint8_t*>(self) + 4, nullptr);
    void* xHolder = GetReferencedObject(static_cast<uint8_t*>(self) + 4, nullptr);

    void* yPos = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
        CSF_AT(void**, yHolder, 0)[0x30 / 4])(yHolder, nullptr);
    int32_t targetY = CSF_AT(int32_t, yPos, 0x18);

    void* xPos = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
        CSF_AT(void**, xHolder, 0)[0x30 / 4])(xHolder, nullptr);
    int32_t targetX = CSF_AT(int32_t, xPos, 0x14);

    // Origin reported by the object at this+0x13C (vtable offset 0x160).
    int32_t originBuf[2] = {0, 0};
    void* originOwner = CSF_AT(void*, self, 0x13C);
    int32_t* origin = reinterpret_cast<int32_t*(__fastcall*)(void*, void*, int32_t*)>(
        CSF_AT(void**, originOwner, 0)[0x160 / 4])(originOwner, nullptr, originBuf);

    int32_t deltaY = targetY - origin[1];
    int32_t deltaX = targetX - origin[0];

    int32_t delta[2];
    delta[0] = deltaX;
    delta[1] = deltaY;

    // The object at this+0x13C is reloaded before the forwarding call.
    void* forwardTarget = CSF_AT(void*, self, 0x13C);
    return reinterpret_cast<int(__fastcall*)(void*, void*, int32_t*, int)>(
        CSF_AT(void**, forwardTarget, 0)[0x14C / 4])(forwardTarget, nullptr, delta, 1);
}

HOOK(0x00536440, InstancingSceneResult_ForwardOffset, "auto");
