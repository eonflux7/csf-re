#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(int, __fastcall, S_00521df0_PlaySoundRequest, 0x00523940, (S_00521df0* self, void* , void* request, float volume));
CSF_FUNC(int, __fastcall, SceneLoader_GetResourceById, 0x007567C0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

namespace {
// Sound request block built on the stack (0x38 bytes).
struct ToggleSoundRequest
{
    int     type;          // +0x00
    int     category;      // +0x04
    int     resourceId;    // +0x08
    int     ownerId;       // +0x0C
    int     unk10;         // +0x10
    int     unk14;         // +0x14
    int     unk18;         // +0x18
    int     unk1C;         // +0x1C
    int     unk20;         // +0x20
    void**  handleVtbl;    // +0x24  embedded C_0082d448
    int     handleA;       // +0x28
    int     handleB;       // +0x2C
    int     unk30;         // +0x30
    uint8_t flag34;        // +0x34
    uint8_t flag35;        // +0x35
    uint8_t flag36;        // +0x36
    uint8_t pad37;         // +0x37  (never written by the original)
};
}

static int __fastcall C_00833ee8_SetToggleStateWithSound(C_00833ee8* self, void* /*edx*/, int newState)
{
    int result = newState;

    if (self->field_03d0 != (uint8_t)newState)
    {
        uint8_t suppressNotify = self->field_0228;
        self->field_03d0 = (uint8_t)newState;
        if (!suppressNotify)
            self->vSlot6C(newState);

        if (self->field_03d0)
        {
            void* sceneLoader = (uint8_t*)GetGlobalValue_8BC1E0() + 0x22A4;
            result = SceneLoader_GetResourceById(sceneLoader, nullptr, 12);
        }
        else
        {
            void* sceneLoader = (uint8_t*)GetGlobalValue_8BC1E0() + 0x22A4;
            result = SceneLoader_GetResourceById(sceneLoader, nullptr, 13);
        }

        if (result != -1)
        {
            ToggleSoundRequest request;
            request.resourceId = result;
            request.type = 4;
            request.category = 4;
            request.unk18 = 0;
            request.unk1C = 0;
            request.unk20 = 0;
            request.handleVtbl = CSF_VTBL(0x0082D448);
            request.handleA = -1;
            request.handleB = -1;
            request.unk10 = 0;
            request.unk14 = 0;
            request.flag34 = 0;
            request.flag35 = 1;
            request.flag36 = 1;
            request.ownerId = -1;
            request.unk30 = 0;

            S_00521df0* soundManager = reinterpret_cast<S_00521df0*>(GetGlobalValue_521BB0());
            return S_00521df0_PlaySoundRequest(soundManager, nullptr, &request, -1.0f);
        }
    }
    return result;
}

HOOK(0x00497080, C_00833ee8_SetToggleStateWithSound, "auto");
