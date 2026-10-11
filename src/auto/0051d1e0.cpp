#include "hook.h"
#include "globals.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, ConstructNameNode, 0x0051C130, (void* self, void* edx));
CSF_FUNC(void**, __fastcall, InitializeParticleStorage, 0x0051D0C0, (void* self, void* edx));

static C_00840ee0* __fastcall C_00840ee0_Construct(C_00840ee0* self, void* /*edx*/)
{
    ConstructNameNode(self, nullptr);
    self->vtbl = CSF_VTBL(0x00840EE0);
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 10;
    InitializeParticleStorage(self, nullptr);
    return self;
}

HOOK(0x0051D1E0, C_00840ee0_Construct, "auto");
