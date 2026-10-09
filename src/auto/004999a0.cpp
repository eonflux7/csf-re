#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, sub_48F5B0, 0x0048F5B0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_499080, 0x00499080, (void* self, void* edx));

// Calls the helper on this, then forwards to the outer object that embeds this
// struct at offset +4 (this - 4), returning the outer handler's result.
static int __fastcall S_0048f5b0_ClearThenForwardToOuter(S_0048f5b0* self, void* /*edx*/)
{
    sub_48F5B0(self, nullptr);
    C_00833908* outer = reinterpret_cast<C_00833908*>(reinterpret_cast<uint8_t*>(self) - 4);
    return sub_499080(outer, nullptr);
}

HOOK(0x4999A0, S_0048f5b0_ClearThenForwardToOuter, "auto");
