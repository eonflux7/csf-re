#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(uint32_t*, __fastcall, CopyInnerBlock, 0x004489C0,
         (void* self, void* edx, uint32_t* a2));

static uint32_t* __stdcall CopyValueAndInnerBlock(uint32_t* destination,
                                                uint32_t* source)
{
    if (destination)
    {
        *destination = *source;
        return CopyInnerBlock(destination + 1, nullptr, source + 1);
    }
    return destination;
}

HOOK(0x00449170, CopyValueAndInnerBlock, "auto");
