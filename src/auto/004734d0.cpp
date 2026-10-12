#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

static CSF_FUNC(char*, __fastcall, ReleaseObjectResources, 0x00472AB0,
               (void* self, void* edx));

static char* __fastcall C_008313f8_PrepareForDestruction(
    C_008313f8* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x008313F8);
    char* result = ReleaseObjectResources(self, nullptr);
    self->vtbl = CSF_VTBL(0x0082D410);
    return result;
}

HOOK(0x004734D0, C_008313f8_PrepareForDestruction, "auto");
