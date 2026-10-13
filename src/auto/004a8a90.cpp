#include "globals.h"
#include "hook.h"

CSF_FUNC(char, __fastcall, C_00835140_ProcessRequest, 0x004A8BF0, (void* self, void* edx, int a2));

static char __fastcall C_00835060_ForwardRequest(void* self, void* edx, int a2)
{
    return C_00835140_ProcessRequest(self, edx, a2);
}

HOOK(0x004A8A90, C_00835060_ForwardRequest, "auto");
