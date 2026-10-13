#include <cstdint>
#include "globals.h"
#include "hook.h"

// Binds the original callee (__cdecl, one stack argument, result unused).
CSF_FUNC(int, __cdecl, RtAnimAnimationStreamGetSize, 0x0061EF90, (int a1));

// _Timevec destructor: forwards the first dword of the object to
// RtAnimAnimationStreamGetSize. The return value is discarded and no
// stack bytes are popped (ret 0).
static void __fastcall Timevec_Destroy(void* self, void* /*edx*/)
{
    int streamHandle = CSF_AT(int, self, 0);
    RtAnimAnimationStreamGetSize(streamHandle);
}

HOOK(0x006C2030, Timevec_Destroy, "auto");
