#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, MenuProgressInput_UpdateGlobalState, 0x00521DE0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuPause_CenterOnTarget, 0x0052F950, (CMenuPause* self, void*));
CSF_FUNC(char, __fastcall, CMenuExtras_InitializeThumbnailControls, 0x00545FE0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuExtras_RefreshThumbnailControls, 0x00546760, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuExtras_LoadThumbnails, 0x00547F10, (CMenuExtras* self, void*));
CSF_FUNC(int, __fastcall, CGuiSlider_ForwardSelfToParent, 0x00718660, (CGuiSlider* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_753A30, 0x00753A30, ());

static int __fastcall CMenuExtras_InitializeScreen(
    CMenuExtras* self, void* /*edx*/, int target, int /*unused*/)
{
    void* screen = reinterpret_cast<void*>(self->field_0018);
    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, screen, 0)[0xA8 / 4])(screen, nullptr, 0);

    self->vResetStatusFields();
    self->field_0010 = static_cast<uint32_t>(target);

    void* progressInput = reinterpret_cast<void*>(GetGlobalValue_521BB0());
    MenuProgressInput_UpdateGlobalState(progressInput, nullptr);

    screen = reinterpret_cast<void*>(self->field_0018);
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, screen, 0)[0x16C / 4])(screen, nullptr);

    CMenuPause_CenterOnTarget(reinterpret_cast<CMenuPause*>(self), nullptr);

    void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
    void* activeObject = CSF_AT(void*, globalState, 0x10);
    if (activeObject)
    {
        void* globalValue = reinterpret_cast<void*>(GetGlobalValue_753A30());
        uint32_t value = CSF_AT(uint32_t, globalValue, 0x200);
        reinterpret_cast<void (__fastcall*)(void*, void*, uint32_t)>(
            CSF_AT(void**, activeObject, 0)[0x20 / 4])(
                activeObject, nullptr, value);
    }

    void* targetObject = reinterpret_cast<void*>(self->field_0010);
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, targetObject, 0)[0xA8 / 4])(targetObject, nullptr);

    CMenuExtras_LoadThumbnails(self, nullptr);
    CMenuExtras_InitializeThumbnailControls(self, nullptr);
    CMenuExtras_RefreshThumbnailControls(self, nullptr);

    uint32_t thumbnailsBegin = self->field_0038;
    if (thumbnailsBegin)
    {
        int32_t thumbnailCount =
            static_cast<int32_t>(self->field_003c - thumbnailsBegin) / 68;
        if (thumbnailCount)
        {
            CGuiSlider* slider = CSF_AT(
                CGuiSlider*, reinterpret_cast<void*>(self->field_0038), 0x20);
            if (slider)
                CGuiSlider_ForwardSelfToParent(slider, nullptr);
        }
    }

    screen = reinterpret_cast<void*>(self->field_0018);
    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, screen, 0)[0x260 / 4])(screen, nullptr, 1);

    return static_cast<int>(self->field_0018);
}

HOOK(0x005482F0, CMenuExtras_InitializeScreen, "auto");
