#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(char*, __cdecl, ScreenEffects_ProcessRange, 0x006FA0C0,
         (char* a1, char* a2, char* a3, int a4, int a5));

static char* __fastcall ScreenEffects_ForwardRangeWithContext(
    void* self, void* /*edx*/, char* first, char* last, char* destination)
{
    return ScreenEffects_ProcessRange(
        first, last, destination,
        reinterpret_cast<int>(self),
        reinterpret_cast<int>(destination));
}

HOOK(0x006FA280, ScreenEffects_ForwardRangeWithContext, "auto");
