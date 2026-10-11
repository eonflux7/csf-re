#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(uint16_t*, __fastcall, DdlSubobject_ForwardCall, 0x00740900, (void* self, void* edx, char* a2));

// Original 0x7411B0: add ecx, 0x458 ; jmp 0x740900
// Tail jump: the sub-object embedded at +0x458 receives the same single argument.
static uint16_t* __fastcall NetCogibleDDL_ForwardToInnerSubobject(char* self, void* /*edx*/, char* a2)
{
    return DdlSubobject_ForwardCall((uint8_t*)self + 0x458, nullptr, a2);
}

HOOK(0x007411B0, NetCogibleDDL_ForwardToInnerSubobject, "auto");
