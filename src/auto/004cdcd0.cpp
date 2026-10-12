#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(void, __fastcall, TCharTask_UpdateSubObject, 0x0048C680, (void* self, void* edx, float a2));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_GetByte023C, 0x00495490, (char* self, void*));
CSF_FUNC(char, __fastcall, C_0083ab58_CheckSceneRule, 0x004CD510, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_0083ab58_OnActivated, 0x004CD640, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0083ab58_OnInactive, 0x004CD740, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0083ab58_TickMode3, 0x004CD7B0, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, C_0083ab58_TickMode2, 0x004CD900, (void* self, void* edx, float a2));
CSF_FUNC(void, __fastcall, C_0083ab58_TickMode4, 0x004CDA80, (void* self, void* edx, int a2));

static void __fastcall C_0083ab58_UpdateAndDispatch(C_0083ab58* self, void* /*edx*/, int a2)
{
    // Sub-object embedded at +0x50 receives the float-reinterpreted argument.
    void* subObject = reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(self) + 0x50);
    TCharTask_UpdateSubObject(subObject, nullptr, *reinterpret_cast<float*>(&a2));

    uint32_t sceneOwner = self->field_0014;
    if (sceneOwner != GetSceneMenuContext(g_pCurrentScene, nullptr) &&
        C_GetByte023C(reinterpret_cast<char*>(sceneOwner), nullptr) &&
        C_0083ab58_CheckSceneRule(self, nullptr))
    {
        C_0083ab58_OnActivated(self, nullptr);
    }
    else
    {
        C_0083ab58_OnInactive(self, nullptr);
    }

    switch (self->field_0040)
    {
    case 2u:
        C_0083ab58_TickMode2(self, nullptr, *reinterpret_cast<float*>(&a2));
        break;
    case 3u:
        C_0083ab58_TickMode3(self, nullptr, a2);
        break;
    case 4u:
        C_0083ab58_TickMode4(self, nullptr, a2);
        break;
    }
}

HOOK(0x004CDCD0, C_0083ab58_UpdateAndDispatch, "auto");
