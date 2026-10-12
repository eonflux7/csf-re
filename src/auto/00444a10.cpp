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

CSF_FUNC(int, __fastcall, CMissionScript_Reset, 0x0046A7A0, (CMissionScript* self, void*));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Destroys a C_008310b8 object: resets the owned mission script, releases the
// nested sub-object (slot 0 virtual call with flag 1), restores the base vtable,
// and frees the block when bit 0 of the argument is set.
static C_008310b8* __fastcall C_008310b8_Destroy(C_008310b8* self, void* /*edx*/, char a2)
{
    self->vtbl = CSF_VTBL(0x008310B8);
    CMissionScript_Reset(reinterpret_cast<CMissionScript*>(self->field_0004), nullptr);

    uint32_t ownedHandle = self->field_0004;
    if (ownedHandle) {
        void* subObject = reinterpret_cast<uint8_t*>(ownedHandle) + 4;
        auto releaseFn = reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, subObject, 0)[0]);
        releaseFn(subObject, nullptr, 1);
        self->field_0004 = 0;
    }

    self->vtbl = CSF_VTBL(0x0082D410);
    if ((a2 & 1) != 0) {
        Mem_Free(self);
    }
    return self;
}

HOOK(0x444A10, C_008310b8_Destroy, "auto");
