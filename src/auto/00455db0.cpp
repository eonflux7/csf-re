#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Forwarded callee: __fastcall, one stack arg, ret 4.
CSF_FUNC(uint32_t*, __fastcall, sub_4553E0, 0x004553E0, (void* self, void* edx, uint32_t* a3));

// __thiscall(CGuiControl* this, int a2, uint32_t* a3), ret 8 -> __fastcall with two stack args (callee pops 8).
static uint32_t* __fastcall CGuiControl_SubmitVertexRecord(CGuiControl* self, void* /*edx*/, int a2, uint32_t* a3)
{
    uint32_t callArgs[5];
    callArgs[0] = static_cast<uint32_t>(a2);
    callArgs[1] = a3[0];
    callArgs[2] = a3[1];
    callArgs[3] = a3[2];
    callArgs[4] = a3[3];

    // Sub-object embedded at +0xFC: its address is passed as `this` to the callee.
    void* rendererSub = static_cast<uint8_t*>(static_cast<void*>(self)) + 0xFC;
    return sub_4553E0(rendererSub, nullptr, callArgs);
}

HOOK(0x455DB0, CGuiControl_SubmitVertexRecord, "auto");
