#include "hook.h"
#include "globals.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, ConstructNameNode, 0x0051C130, (void* self, void* edx));
CSF_FUNC(int, __fastcall, InitializeParticleManagerNode, 0x0051DA10, (void* self, void* edx));

static C_00840f00* __fastcall C_00840f00_Construct(C_00840f00* self, void* /*edx*/)
{
    ConstructNameNode(self, nullptr);
    self->vtbl = CSF_VTBL(0x00840F00);
    InitializeParticleManagerNode(self, nullptr);
    return self;
}

HOOK(0x0051DB20, C_00840f00_Construct, "auto");
