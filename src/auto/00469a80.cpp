#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Callee at 0x00469710 is __cdecl. The original pushes four dwords before the call:
// (a1, a2, incoming ECX, a2), and the caller cleans up with `add esp, 0x10`.
CSF_FUNC(int, __cdecl, MissionScript_ParseEntry, 0x00469710,
         (int a1, int a2, std::uintptr_t context, int repeatedArgument));

// 0x00469A80: __stdcall (ret 8) thunk. Modeled as __fastcall so ECX arrives as the
// first parameter and the two stack arguments are callee-cleaned (ret 8).
static int __fastcall MissionScript_ForwardParseArguments(
    void* context, void* /*edx*/, int firstArgument, int secondArgument)
{
    return MissionScript_ParseEntry(
        firstArgument,
        secondArgument,
        reinterpret_cast<std::uintptr_t>(context),
        secondArgument);
}

HOOK(0x00469A80, MissionScript_ForwardParseArguments, "auto");
