#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, SceneMissionSetup_ConstructObjectController, 0x004A4280, (void* self, void* edx, int a2));

// Constructs the scene-object controller: runs the base setup, clears the trailing
// state fields, and installs this class's vtable plus the secondary base vtable.
static C_008346e8* __fastcall SceneObject_CreateController(C_008346e8* self, void* /*edx*/, int a2)
{
    SceneMissionSetup_ConstructObjectController(self, nullptr, a2);

    CSF_AT(int, self, 0x7C) = 0;
    CSF_AT(uint8_t, self, 0x80) = 0;
    CSF_AT(uint8_t, self, 0x81) = 0;

    self->vtbl = CSF_VTBL(0x008346E8);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x008346C8));

    return self;
}

HOOK(0x004A66C0, SceneObject_CreateController, "auto");
