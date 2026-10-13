#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GfxFrames_GetProviderFrameObject, 0x006C0500, (int clump));
CSF_FUNC(std::uint32_t*, __cdecl, RwFrameGetLTM, 0x00623720, (std::uint32_t frame));

// Writes the LTM pointer of the provider frame into the 5-byte out-record at a2
// (dword pointer at +0, flag byte 0 at +4) and returns a2.
static int __fastcall C_00862900_WriteFrameLtm(C_00862900* self, void* /*edx*/, int outAddress)
{
    int providerFrameObject = GfxFrames_GetProviderFrameObject(static_cast<int>(self->field_0004));

    uint8_t* providerFrame = reinterpret_cast<uint8_t*>(providerFrameObject);
    uint32_t frameHandle = CSF_AT(uint32_t, providerFrame, 4);

    uint32_t ltm = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(RwFrameGetLTM(frameHandle)));

    uint8_t* out = reinterpret_cast<uint8_t*>(outAddress);
    CSF_AT(uint32_t, out, 0) = ltm;
    CSF_AT(uint8_t, out, 4) = 0;

    return outAddress;
}

HOOK(0x006F7320, C_00862900_WriteFrameLtm, "auto");
