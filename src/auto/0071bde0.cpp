#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, ClearRecordStringRange, 0x0071B600,
         (int firstArgument, int secondArgument, std::uintptr_t context, int repeatedArgument));

static int __fastcall ForwardArgumentsToProcessor(
    void* context, void* /*edx*/, int firstArgument, int secondArgument)
{
    return ClearRecordStringRange(
        firstArgument,
        secondArgument,
        reinterpret_cast<std::uintptr_t>(context),
        secondArgument);
}

HOOK(0x0071BDE0, ForwardArgumentsToProcessor, "auto");
