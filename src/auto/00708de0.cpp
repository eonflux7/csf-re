#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Binds to the original screen-effect helpers and the RenderWare camera end-update.
CSF_FUNC(int, __fastcall, GfxScreenEffects_BeginDrawing, 0x00708D10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GfxScreenEffects_DrawQuad, 0x00708AB0, (void* self, void* edx, int a2, char a3, int a4, int a5, int a6, uint8_t a7, float a8, float a9, float a10, float a11));
CSF_FUNC(void, __cdecl, RwCameraEndUpdate, 0x00628E40, (uint32_t camera));

// Draws one full-screen effect pass, ends the camera update, and hands the
// pending value over to the camera object (+0x64), clearing the pending slot.
static int __fastcall C_00862b58_ApplyEffectPass(C_00862b58* self, void* /*edx*/, int a2, char a3)
{
    GfxScreenEffects_BeginDrawing(self, nullptr);
    GfxScreenEffects_DrawQuad(
        self,
        nullptr,
        0xFFFFFF,
        a3,
        static_cast<int>(self->field_0028),
        static_cast<int>(self->field_002c),
        a2,
        1u,
        0.0f,
        0.0f,
        1.0f,
        1.0f);
    RwCameraEndUpdate(self->field_0010);

    int result = static_cast<int>(self->field_001c);
    uint8_t* camera = reinterpret_cast<uint8_t*>(self->field_0010);
    CSF_AT(uint32_t, camera, 0x64) = static_cast<uint32_t>(result);
    self->field_001c = 0;
    return result;
}

HOOK(0x708DE0, C_00862b58_ApplyEffectPass, "auto");
