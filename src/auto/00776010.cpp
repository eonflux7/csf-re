#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// 0x007754B0: __cdecl, four stack arguments (a1, a2, a3, a4).
CSF_FUNC(int, __cdecl, UpdateGameDataRecordLinks, 0x007754B0,
         (int a1, int a2, int a3, int a4));

// 0x00776010: __thiscall thunk, callee-cleaned (ret 8). The original pushes
// (a2, ECX, a2, a1), so UpdateGameDataRecordLinks sees (a1, a2, this, a2).
static int __fastcall ForwardUpdateGameDataRecordLinks(
    S_0051aa70* self, void* /*edx*/, int firstArgument, int secondArgument)
{
    return UpdateGameDataRecordLinks(
        firstArgument,
        secondArgument,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(self)),
        secondArgument);
}

HOOK(0x00776010, ForwardUpdateGameDataRecordLinks, "auto");
