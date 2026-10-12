#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(char, __cdecl, MissionScript_DestroyStringRange, 0x00697A80, (int a1, int a2));

static int __fastcall CoreFiles_ForwardStringRangeArguments(
    void* context, void* /*edx*/, int* firstArgument, int* secondArgument)
{
    using ForwardedCall = int (__cdecl*)(
        int*, int*, std::uintptr_t, int*);

    return reinterpret_cast<ForwardedCall>(MissionScript_DestroyStringRange)(
        firstArgument,
        secondArgument,
        reinterpret_cast<std::uintptr_t>(context),
        secondArgument);
}

HOOK(0x00697CE0, CoreFiles_ForwardStringRangeArguments, "auto");
