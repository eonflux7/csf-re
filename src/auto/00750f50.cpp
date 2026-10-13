#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Forwards to NetPlayerDDL_ProcessBaseField at 0x7A31A0. The original takes
// (a1, a2, a3) as stdcall, passes a1 in ecx as the object, and pushes a3 then a2,
// so the callee sees a2 as its field argument and a3 as its operation argument.
CSF_FUNC(char, __fastcall, NetPlayerDDL_ProcessBaseField, 0x007A31A0, (char* self, void* edx, char* field, int operation));

static char __stdcall NetChatDdl_ForwardToProcessBaseField(char* a1, char* a2, int a3)
{
    return NetPlayerDDL_ProcessBaseField(a1, nullptr, a2, a3);
}

HOOK(0x00750F50, NetChatDdl_ForwardToProcessBaseField, "auto");
