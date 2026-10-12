#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Calls the virtual method in slot 4 (+0x10) of the object whose pointer is stored at
// this->field_0030, passing &this->field_0030 as `this`. That method returns a pointer to
// a 3-dword record, which is copied into this->field_0050..0058. Returns &this->field_0050.
static uint32_t* __fastcall C_00860288_CopyInnerTriple(C_00860288* self, void* /*edx*/)
{
    void* innerObject = reinterpret_cast<void*>(self->field_0030);
    uint32_t* recordSource =
        ((uint32_t* (__fastcall*)(void*, void*))CSF_AT(void**, innerObject, 0)[0x10 / 4])(
            &self->field_0030, nullptr);

    self->field_0050 = recordSource[0];
    CSF_AT(uint32_t, self, 0x54) = recordSource[1];
    CSF_AT(uint32_t, self, 0x58) = recordSource[2];
    return &self->field_0050;
}

HOOK(0x6B37F0, C_00860288_CopyInnerTriple, "auto");


