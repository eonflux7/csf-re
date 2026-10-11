#include <cstdint>

#include "hook.h"
#include "core/memory.h"
#include "globals.h"

// 0x54ECE0: __cdecl, no args; returns the global value (0 when absent).
CSF_FUNC(int, __cdecl, GetGlobalValue_54ECE0, 0x0054ECE0, ());

// 0x550150: __fastcall, takes the object in ecx (edx unused); returns a char in al.
CSF_FUNC(char, __fastcall, ApplyGlobalObjectChange_550150, 0x00550150, (void* self, void* edx));

// 0x50A080: fetches the global value; if nonzero, fetches it again and tail-jumps into 0x550150
// with that second value as `this` (ecx). Otherwise returns the first (zero) result unchanged.
// The tail jump forwards no stack arguments (the callee is __fastcall), so the original's `ret` pops 0.
static int __cdecl AppMain_ForwardGlobalValueTo550150()
{
    int result = GetGlobalValue_54ECE0();
    if (result)
    {
        int globalValue = GetGlobalValue_54ECE0();
        return static_cast<int>(ApplyGlobalObjectChange_550150(reinterpret_cast<void*>(static_cast<uintptr_t>(globalValue)), nullptr));
    }
    return result;
}

HOOK(0x50A080, AppMain_ForwardGlobalValueTo550150, "auto");
