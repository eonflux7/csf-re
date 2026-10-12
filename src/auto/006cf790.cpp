#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Callees (bound by address, signatures as given in the packet)
CSF_FUNC(int, __fastcall, CollisionShape_ReleaseSphereData, 0x006CEA20, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CollisionShape_ResetVertexBuffer, 0x006D5300, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CollisionShape_ReleaseField0014, 0x006CFBD0, (void* self, void* edx));

// Shutdown/reset of a collision shape: releases the sub-objects in order, then clears the
// "active" flag at +0x68. Returns the result of the final release call.
static int __fastcall C_00860cbc_ShutdownAndClear(C_00860cbc* self, void* /*edx*/)
{
    CollisionShape_ReleaseSphereData(reinterpret_cast<void*>(self->field_0010), nullptr);
    CollisionShape_ResetVertexBuffer(reinterpret_cast<void*>(self->field_000c), nullptr);

    // Virtual slot +0x1C (index 7) on the object at +0x08
    void* objA = reinterpret_cast<void*>(self->field_0008);
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, objA, 0)[0x1C / 4])(objA, nullptr);

    // Virtual slot +0x20 (index 8) on the object at +0x04
    void* objB = reinterpret_cast<void*>(self->field_0004);
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, objB, 0)[0x20 / 4])(objB, nullptr);

    int result = CollisionShape_ReleaseField0014(reinterpret_cast<void*>(self->field_0014), nullptr);
    self->field_0068 = 0;
    return result;
}

HOOK(0x006CF790, C_00860cbc_ShutdownAndClear, "auto");
