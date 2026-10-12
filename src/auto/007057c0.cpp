#include <cstdint>
#include "hook.h"

// Returns whether a texture dimension is one of the sizes the particle renderer accepts:
// 4, 8, 16, 32, 64, 128, 256, 512 or 1024. The argument arrives in eax and the result
// is returned in al (upper bytes of eax are preserved, as in the original).
static uint32_t ParticleRender_IsAllowedTextureDimension(hook::Regs* regs, const uint32_t* args) {
    (void)args;
    const int32_t dimension = static_cast<int32_t>(regs->eax);
    bool allowed = false;

    if (dimension <= 64) {
        if (dimension == 64) {
            allowed = true;
        } else {
            switch (dimension) {
                case 4:
                case 8:
                case 16:
                case 32:
                    allowed = true;
                    break;
                default:
                    allowed = false;
                    break;
            }
        }
    } else if (dimension > 512) {
        allowed = (dimension == 1024);
    } else {
        if (dimension == 512 || dimension == 128) {
            allowed = true;
        } else {
            allowed = (dimension == 256);
        }
    }

    return (regs->eax & 0xFFFFFF00u) | (allowed ? 1u : 0u);
}

HOOK_REGS(0x7057C0, ParticleRender_IsAllowedTextureDimension, 0, "gfx_particle_render");
