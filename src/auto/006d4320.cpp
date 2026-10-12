#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"

// Callees bound by address (original signatures from the packet).
CSF_FUNC(int, __cdecl, RwFrameForAllChildren, 0x006237C0, (int a1, int (__cdecl* a2)(int, int), int a3));
CSF_FUNC(int, __cdecl, RwFrameRemoveChild, 0x00623750, (int a1));
CSF_FUNC(int, __cdecl, RwFrameDestroy, 0x00623B10, (int frame));
CSF_FUNC(int, __cdecl, CountFrameChildren, 0x006C03C0, (int frame));

// Recursively prunes a frame tree: visits each child with this same function,
// then destroys this frame if the helper says it may go and it has no children.
static int __cdecl GfxClumpDict_PruneFrame(int frame, int context)
{
    RwFrameForAllChildren(frame, &GfxClumpDict_PruneFrame, context);

    // sub_6D4240 takes its arguments in eax (context) and esi (frame).
    hook::Regs regs{};
    regs.eax = static_cast<uint32_t>(context);
    regs.esi = static_cast<uint32_t>(frame);
    uint32_t helperResult = hook::call_regs(CSF_ADDR_FN(0x006D4240), &regs, nullptr, 0);

    // The original tests only al.
    if ((static_cast<uint8_t>(helperResult)) == 0 && CountFrameChildren(frame) == 0)
    {
        // Frame has a parent link at +4 (stored pointer): detach before destroying.
        if (CSF_AT(uint32_t, reinterpret_cast<void*>(frame), 0x04) != 0)
            RwFrameRemoveChild(frame);
        RwFrameDestroy(frame);
    }
    return frame;
}

HOOK(0x006D4320, GfxClumpDict_PruneFrame, "auto");
