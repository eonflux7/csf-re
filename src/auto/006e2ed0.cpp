#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Stdcall loader: Source at [esp], a2 at [esp+4]; callee pops 8 bytes.
CSF_FUNC(int, __stdcall, GfxParticleRender_LoadDDSTexture, 0x00705DD0, (char* source, char* textureMode));

// Global the original loads into ecx before the call (passed as this-like register; unused by the stdcall callee).
CSF_GLOBAL(uint32_t, g_GfxParticleRenderContext, 0x008BB894);

// Loads a texture file via the DDS loader. Both arguments are forwarded unchanged.
static int __cdecl GfxIm3d_TryLoadTexture(char* Source, int a2)
{
    (void)g_GfxParticleRenderContext;
    return GfxParticleRender_LoadDDSTexture(Source, reinterpret_cast<char*>(static_cast<intptr_t>(a2)));
}

HOOK(0x006E2ED0, GfxIm3d_TryLoadTexture, "auto");
