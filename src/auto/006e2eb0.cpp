#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// The second argument is a char to the callee (it reads `byte ptr [ebp+0xC]` only), but the original forwards the
// caller's whole dword, upper bytes included: bound and forwarded as a dword so the call is the original's bit for bit.
CSF_FUNC(int, __stdcall, GfxParticleRender_LoadTexture, 0x00705B40, (char* source, uint32_t mipmapping, char* alphaType));
CSF_GLOBAL(uint32_t, g_GfxIm3dTextureLoadContext, 0x008BB894);

// Forwards the three stack arguments to GfxParticleRender_LoadTexture (stdcall, pops 12).
// The original also loads a global into ecx before the call; the stdcall callee ignores ecx,
// so the value is read only to keep the data reference and is not used further.
static int __cdecl GfxIm3d_LoadTextureWithOptions(char* Source, uint32_t a2, int a3)
{
    [[maybe_unused]] uint32_t unusedContextRegister = g_GfxIm3dTextureLoadContext;
    return GfxParticleRender_LoadTexture(Source, a2, reinterpret_cast<char*>(static_cast<uintptr_t>(a3)));
}

HOOK(0x006E2EB0, GfxIm3d_LoadTextureWithOptions, "auto");
