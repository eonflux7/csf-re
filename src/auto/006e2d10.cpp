#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint8_t*, __cdecl, SceneLoader_CreateLoadTimer, 0x0069CDC0, ());

static char __fastcall S_006e2d10_InitializeAuxiliaryResource(S_006e2d10* self, void* /*edx*/)
{
    if (self->field_0004)
    {
        void** previousTimer = reinterpret_cast<void**>(self->field_003c);
        if (previousTimer)
        {
            // vtable slot 0 (deleting/release call), this = previousTimer, arg = 1
            ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, previousTimer, 0)[0])(previousTimer, nullptr, 1);
            self->field_003c = 0;
        }
        self->field_0004 = 0;
    }

    uint8_t* loadTimer = SceneLoader_CreateLoadTimer();
    self->field_003c = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(loadTimer));

    // vtable slot 2 of the load timer, this = loadTimer
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, loadTimer, 0)[2])(loadTimer, nullptr);

    // vtable slot 2 of self, this = self
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[2])(self, nullptr);

    self->field_0004 = 1;
    return 1;
}

HOOK(0x006E2D10, S_006e2d10_InitializeAuxiliaryResource, "auto");
