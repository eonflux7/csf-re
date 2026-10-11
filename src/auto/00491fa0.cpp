#include "hook.h"
#include "globals.h"
#include "core/memory.h"

CSF_FUNC(char, __fastcall, GetLinkedObjectState, 0x0040EC00, (void* self, void* edx));

static char __fastcall GetChildObjectState(void* self, void* /*edx*/)
{
    return GetLinkedObjectState(CSF_AT(void*, self, 0x4C), nullptr);
}

HOOK(0x00491FA0, GetChildObjectState, "auto");
