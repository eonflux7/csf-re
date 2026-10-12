#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __cdecl, RwFrameCreate, 0x006235D0, ());
CSF_FUNC(void*, __cdecl, RwFrameSetIdentity, 0x006238C0, (void* frame));

static void* __fastcall GfxFrames_InitIdentityFrame(void* self, void* /*edx*/)
{
    void* frame = RwFrameCreate();
    CSF_AT(void*, self, 0x0C) = frame;
    CSF_AT(uint8_t, self, 0x10) = 1;
    return RwFrameSetIdentity(frame);
}

HOOK(0x006BF1E0, GfxFrames_InitIdentityFrame, "auto");
