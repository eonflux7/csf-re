#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Bound by address: the original tail-jumps here, forwarding ecx/edx and the stack arg unchanged.
CSF_FUNC(int, __fastcall, sub_5038B0, 0x005038B0, (void* self, void* edx, char a2));

// __thiscall(char) forwarder: `push ebp; mov ebp,esp; pop ebp; jmp 0x5038B0`.
// The callee pops the 4-byte stack argument (ret 4), so this wrapper also pops 4.
static int __fastcall CSceneLoader_ForwardToLoader(CSceneLoader* self, void* /*edx*/, char a2)
{
    return sub_5038B0(self, nullptr, a2);
}

HOOK(0x505CF0, CSceneLoader_ForwardToLoader, "auto");
