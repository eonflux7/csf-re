#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ForwardProviderFrameState, 0x004F4EA0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GlobalValue_SetSelection, 0x004FC3B0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(double, __fastcall, GetNestedObjectField9c, 0x006CAF40, (void* self, void*));

static uint32_t __fastcall C_0083f448_UpdateTimedSelection(
    C_0083f448* self, void* /*edx*/, int /*unused*/)
{
    int globalObject = GetGlobalInt();
    int sceneController = SceneLoader_GetSceneController(
        reinterpret_cast<void*>(globalObject), nullptr);

    void* entry = nullptr;
    if (self->field_001c)
        entry = CSF_AT(void*, reinterpret_cast<void*>(self->field_0014), 0);

    void* provider = GetCachedObjectProvider(entry, nullptr);
    int frameState = ForwardProviderFrameState(provider, nullptr);
    double sceneValue = GetNestedObjectField9c(
        reinterpret_cast<void*>(sceneController), nullptr);

    bool exceedsThreshold =
        sceneValue > CSF_AT(float, reinterpret_cast<void*>(frameState), 0x4);
    uint32_t result = self->field_008c;

    if (exceedsThreshold)
    {
        if (result == 0xFFFFFFFF)
            return GlobalValue_SetSelection(self, nullptr, 2);
    }
    else if (result == 2)
    {
        result = self->field_0068;
        if (result)
        {
            entry = nullptr;
            if (self->field_001c)
                entry = CSF_AT(void*, reinterpret_cast<void*>(self->field_0014), 0);

            void* target = reinterpret_cast<void*>(ResolveEntryLookupValue(entry, nullptr));
            uint32_t selectionEntry =
                CSF_AT(uint32_t, self, 0x60 + self->field_008c * 4);

            result = ((int(__fastcall*)(void*, void*, uint32_t))
                CSF_AT(void**, target, 0)[0x3C / 4])(
                    target, nullptr, selectionEntry);
        }
        self->field_008c = 0xFFFFFFFF;
    }

    return result;
}

HOOK(0x004FC740, C_0083f448_UpdateTimedSelection, "auto");
