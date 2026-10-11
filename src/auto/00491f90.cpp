#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

static CSF_FUNC(char, __fastcall, UpdateBaseWithTime, 0x004910C0, (void* self, void* edx, float a2));

static char __fastcall C_00833a48_UpdateWithTime(C_00833a48* self, void* /*edx*/, float time)
{
    return UpdateBaseWithTime(self, nullptr, time);
}

HOOK(0x00491F90, C_00833a48_UpdateWithTime, "auto");
