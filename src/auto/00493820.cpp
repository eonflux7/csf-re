#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// 0x004910C0 - component update for the C_00833908 base; its char result is not used here.
CSF_FUNC(char, __fastcall, C_00833908_UpdateComponents, 0x004910C0,
         (C_00833908* self, void* /*edx*/, float deltaTime));

// 0x00490CD0 - per-frame timer update of C_00833cf8.
CSF_FUNC(void, __fastcall, C_00833cf8_UpdateTimer, 0x00490CD0,
         (C_00833cf8* self, void* /*edx*/, float elapsed));

// 0x00493820 - base update for C_00833cf8: updates the base components first,
// then advances this class's timer with the same delta.
static void __fastcall C_00834198_UpdateBase(C_00833cf8* self, void* /*edx*/, float a2)
{
    (void)C_00833908_UpdateComponents(self, nullptr, a2);
    C_00833cf8_UpdateTimer(self, nullptr, a2);
}

HOOK(0x493820, C_00834198_UpdateBase, "auto");
