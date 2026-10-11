#include "hook.h"
#include "globals.h"
#include "core/memory.h"

static CSF_FUNC(char, __fastcall, GetLinkedObjectState, 0x0040F490, (void* self, void* edx));

static char __fastcall GetLinkedObjectStateForwarder(void* self, void* /*edx*/)
{
    return GetLinkedObjectState(CSF_AT(void*, self, 0x4C), nullptr);
}

HOOK(0x00491FC0, GetLinkedObjectStateForwarder, "auto");
