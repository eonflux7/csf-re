#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Bound by address: S_0045d610_UpdateFrameController at 0x00469800 (__fastcall, ret 4).
CSF_FUNC(unsigned int, __fastcall, S_0045d610_UpdateFrameController, 0x00469800, (S_0045d610* self, void* , float a2));

// Entry 0x00444930: ecx is the incoming object pointer. Its +4 slot holds the frame controller
// object, which is forwarded (tail-jumped) to UpdateFrameController together with the float
// argument. The forwarded call's `ret 4` pops the single stack argument, so this function
// does not pop anything itself.
static unsigned int __fastcall S_0045d610_UpdateSecondaryController(S_0045d610* self, void* /*edx*/, float a1)
{
    S_0045d610* frameController = CSF_AT(S_0045d610*, self, 4);
    return S_0045d610_UpdateFrameController(frameController, nullptr, a1);
}

HOOK(0x00444930, S_0045d610_UpdateSecondaryController, "auto");
