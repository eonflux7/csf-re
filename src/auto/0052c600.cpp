#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x521BB0: int __cdecl GetGlobalValue() - returns a global object pointer.
CSF_FUNC(int, __cdecl, GetGlobalValue, 0x521BB0, ());

// 0x5225D0: __thiscall method invoked on the global object returned above.
CSF_FUNC(int, __fastcall, ProcessGlobalValue, 0x5225D0, (void* self, void* edx));

static int __cdecl MenuProgressInput_GetGlobalAndForward()
{
    return ProcessGlobalValue(reinterpret_cast<void*>(GetGlobalValue()), nullptr);
}

HOOK(0x52C600, MenuProgressInput_GetGlobalAndForward, "auto");
