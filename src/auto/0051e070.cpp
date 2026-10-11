#include "hook.h"
#include "globals.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, ConstructNameNode, 0x0051C130, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, InitializeParticleStorage, 0x0051E010, (void* self, void* edx));

static C_00840f28* __fastcall C_00840f28_Construct(C_00840f28* self, void* /*edx*/)
{
    ConstructNameNode(self, nullptr);
    self->vtbl = CSF_VTBL(0x00840F28);
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = 10;
    InitializeParticleStorage(self, nullptr);
    return self;
}

HOOK(0x0051E070, C_00840f28_Construct, "auto");
