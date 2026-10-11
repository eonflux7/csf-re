#include <cstdint>
#include "globals.h"
#include "hook.h"

// Original 0x00406780: __fastcall, one stack arg (char), ret 4.
CSF_FUNC(uint32_t*, __fastcall, C_0082d70c_HandleSecondaryBaseCall, 0x00406780, (void* self, void* edx, char a2));

// 0x00406680: __thiscall stub. Adjusts `this` back by 4 (the sub-object at +4
// lives inside the outer object) and tail-jumps to 0x00406780 with the same
// stack argument. Pops 4 bytes, matching the callee's `ret 4`.
static uint32_t* __fastcall C_0082d70c_ForwardFromSecondaryBase(void* self, void* /*edx*/, char a2)
{
    void* outerSelf = static_cast<uint8_t*>(self) - 4;
    return C_0082d70c_HandleSecondaryBaseCall(outerSelf, nullptr, a2);
}

HOOK(0x00406680, C_0082d70c_ForwardFromSecondaryBase, "auto");
