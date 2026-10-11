#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Forwards to 0x005172C0 with this/args unchanged (original is a bare jmp tail call).
CSF_FUNC(char, __fastcall, ProfileManager_ProcessSaveRequest, 0x005172C0, (void* self, void* edx, unsigned int* a2));

static char __fastcall C_00840af8_ForwardToSaveRequest(C_00840af8* self, void* /*edx*/, unsigned int* a2)
{
    return ProfileManager_ProcessSaveRequest(self, nullptr, a2);
}

HOOK(0x005173B0, C_00840af8_ForwardToSaveRequest, "auto");
