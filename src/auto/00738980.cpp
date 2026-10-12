#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, FreeBlock_690F70, 0x00690F70, (void* block));

static int __fastcall C_00867358_Destroy(C_00867358* self, void* /*edx*/)
{
    uint32_t ownerCookie = self->field_0020;
    self->vtbl = CSF_VTBL(0x00867358);

    if (ownerCookie) {
        void* owner = reinterpret_cast<void*>(self->field_0024);
        // vtable slot 0x88 / 4 = 34
        void** ownerVtbl = CSF_AT(void**, owner, 0);
        auto releaseOwned = reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t)>(ownerVtbl[34]);
        releaseOwned(owner, nullptr, ownerCookie);
    }

    if (self->field_0010) {
        FreeBlock_690F70(reinterpret_cast<void*>(self->field_0010));
        self->field_0010 = 0;
    }

    if (self->field_0004) {
        FreeBlock_690F70(reinterpret_cast<void*>(self->field_0004));
        self->field_0004 = 0;
    }

    self->field_0008 = 0;
    self->field_0014 = 0;

    int result = self->vClearFields();

    self->vtbl = CSF_VTBL(0x008670CC);
    return result;
}

HOOK(0x00738980, C_00867358_Destroy, "auto");
