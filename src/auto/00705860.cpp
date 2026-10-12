#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(uint8_t, g_ParticleSuffixTerminator, 0x008631C2);

static uint32_t __cdecl GfxParticleRender_BuildVariantName(
    hook::Regs* regs, const uint32_t* args)
{
    const char* source = reinterpret_cast<const char*>(regs->eax);
    char* destination = reinterpret_cast<char*>(regs->edx);
    const char* extension = reinterpret_cast<const char*>(args[1]);

    char suffix[5] = {109, 49, 0, 0, 0};
    std::memcpy(suffix, CSF_STR(0x008631C0, "m1"), 2);
    suffix[2] = static_cast<char>(g_ParticleSuffixTerminator);
    suffix[1] = static_cast<char>(
        static_cast<uint8_t>(suffix[1]) +
        static_cast<uint8_t>(args[0]) - 1);

    char* output = destination;
    char character;
    do
    {
        character = *source++;
        *output++ = character;
    } while (character);

    std::strcat(destination, suffix);

    const uint32_t bytes = static_cast<uint32_t>(std::strlen(extension) + 1);
    std::memcpy(destination + std::strlen(destination), extension, bytes);
    return bytes;
}

HOOK_REGS(0x00705860, GfxParticleRender_BuildVariantName, 0, "gfx_particle_render");
