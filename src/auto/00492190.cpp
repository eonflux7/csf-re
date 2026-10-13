#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));

// Returns 1 when the attached child component reports true from its slot +0x40 check,
// this object's flag 0x8 is set, and the scene object is not inactive; otherwise 0.
static int __fastcall C_00833b88_IsActiveWithFlag8Set(C_00833b88* self, void* /*edx*/)
{
    if (self->vGetField016c())
    {
        void* component = reinterpret_cast<void*>(self->vGetField016c());
        bool componentReady =
            ((bool(__fastcall*)(void*, void*))CSF_AT(void**, component, 0)[0x40 / 4])(component, nullptr);
        if (componentReady)
        {
            if ((self->field_0170 & 8) != 0 && !SceneObj_IsInactive(self, nullptr))
            {
                return 1;
            }
        }
    }
    return 0;
}

HOOK(0x00492190, C_00833b88_IsActiveWithFlag8Set, "auto");
