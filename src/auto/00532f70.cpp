#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60,
         (void* block));
CSF_FUNC(int, __fastcall, CGuiSlider_ForwardSelfToParent, 0x00718660,
         (void* self, void* edx));

static void* __fastcall CMenuEula_UpdateNavigationButtons(
    CMenuEula* self, void* /*edx*/)
{
    uint32_t previousArgument = self->field_0038;
    void* previousButton = reinterpret_cast<void*>(self->field_002c);
    previousArgument = (previousArgument & 0xffffff00u) |
        (static_cast<int32_t>(previousArgument) > 0 ? 1u : 0u);

    reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t, int, int)>(
        CSF_AT(void**, previousButton, 0)[0x68 / 4])(
            previousButton, nullptr, previousArgument, 1, 0);

    void* textControl = reinterpret_cast<void*>(self->field_0034);
    alignas(4) uint8_t temporaryText[0x1c];
    CSF_AT(uint32_t, temporaryText, 0x18) = 7;
    CSF_AT(uint32_t, temporaryText, 0x14) = 0;
    CSF_AT(uint16_t, temporaryText, 0x04) = 0;

    void* textInfo =
        reinterpret_cast<void*(__fastcall*)(void*, void*, void*, int)>(
            CSF_AT(void**, textControl, 0)[0x24c / 4])(
                textControl, nullptr, temporaryText, 0);

    uint32_t nextArgument = CSF_AT(uint32_t, textInfo, 0x14);
    bool canAdvance = false;
    if (static_cast<int32_t>(nextArgument) > 0)
    {
        const uint32_t lastPosition = self->field_0050 - nextArgument;
        canAdvance = self->field_0038 != lastPosition;
    }
    nextArgument = (nextArgument & 0xffffff00u) |
        (canAdvance ? 1u : 0u);

    void* nextButton = reinterpret_cast<void*>(self->field_0030);
    reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t, int, int)>(
        CSF_AT(void**, nextButton, 0)[0x68 / 4])(
            nextButton, nullptr, nextArgument, 1, 0);

    previousButton = reinterpret_cast<void*>(self->field_002c);
    self->field_0018 = 0xffffffffu;
    uint32_t result =
        reinterpret_cast<uint32_t(__fastcall*)(void*, void*)>(
            CSF_AT(void**, previousButton, 0)[0x6c / 4])(
                previousButton, nullptr);

    if ((result & 0xffu) == 0)
    {
        result = static_cast<uint32_t>(CGuiSlider_ForwardSelfToParent(
            reinterpret_cast<void*>(self->field_0030), nullptr));
    }

    if (CSF_AT(uint32_t, temporaryText, 0x18) >= 8)
    {
        void* block = CSF_AT(void*, temporaryText, 0x04);
        result = reinterpret_cast<uintptr_t>(block);
        if (block)
            result = static_cast<uint32_t>(Mem_FreeDebug(block));
    }

    return reinterpret_cast<void*>(result);
}

HOOK(0x00532F70, CMenuEula_UpdateNavigationButtons, "auto");
