#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee at 0x6F0710: forwards with the same stack argument; it pops 4 (ret 4).
CSF_FUNC(uint32_t*, __fastcall, PTank_SetupAtomicAndMaterial, 0x006F0710, (void* self, void* edx, char a2));

// Original: sub ecx, 4 ; jmp 0x6F0710
// Tail call: this is rebased 4 bytes back and the stack argument is forwarded unchanged.
static uint32_t* __fastcall PTank_ForwardFromInnerSubobject(char* self, void* /*edx*/, char a2) {
    return PTank_SetupAtomicAndMaterial(self - 4, nullptr, a2);
}

HOOK(0x006F06A0, PTank_ForwardFromInnerSubobject, "auto");
