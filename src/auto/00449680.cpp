#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, ProcessPairWithContext, 0x004490D0,
         (int a1, int a2, int a3, int a4));

static int __fastcall ForwardPairWithContext(
    void* context, void* /*edx*/, int firstArgument, int secondArgument)
{
    return ProcessPairWithContext(
        firstArgument,
        secondArgument,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(context)),
        secondArgument);
}

HOOK(0x00449680, ForwardPairWithContext, "auto");
