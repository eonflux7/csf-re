#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static const auto Mem_FreeDebug =
    reinterpret_cast<int (__cdecl*)(void*)>(CSF_ADDR_FN(0x00690E60));

static const auto CGuiSlider_ForwardSelfToParent =
    reinterpret_cast<int (__fastcall*)(void*, void*)>(CSF_ADDR_FN(0x00718660));

static void* __fastcall CMenuEula_UpdateNavigationButtons(
    CMenuEula* self, void* /*edx*/)
{
    using SetButtonState = void (__fastcall*)(void*, void*, uint32_t, int, int);
    using GetButtonState = uint32_t (__fastcall*)(void*, void*);
    using GetTextInfo = void* (__fastcall*)(void*, void*, void*, int);

    uint32_t previousState = self->field_0038;
    previousState = (previousState & 0xFFFFFF00u) |
                    (static_cast<int32_t>(previousState) > 0 ? 1u : 0u);

    void* previousButton = reinterpret_cast<void*>(self->field_002c);
    reinterpret_cast<SetButtonState>(
        CSF_AT(void**, previousButton, 0)[0x68 / 4])(
            previousButton, nullptr, previousState, 1, 0);

    // Temporary small-string storage: only these members are initialized.
    alignas(4) uint8_t temporaryText[0x1C];
    CSF_AT(uint32_t, temporaryText, 0x18) = 7;
    CSF_AT(uint32_t, temporaryText, 0x14) = 0;
    CSF_AT(uint16_t, temporaryText, 0x04) = 0;

    void* textControl = reinterpret_cast<void*>(self->m_textControl);
    void* textInfo = reinterpret_cast<GetTextInfo>(
        CSF_AT(void**, textControl, 0)[0x24C / 4])(
            textControl, nullptr, temporaryText, 0);

    uint32_t nextState = CSF_AT(uint32_t, textInfo, 0x14);
    bool canAdvance = false;
    if (static_cast<int32_t>(nextState) > 0)
    {
        const uint32_t lastPosition = self->field_0050 - nextState;
        canAdvance = self->field_0038 != lastPosition;
    }
    nextState = (nextState & 0xFFFFFF00u) | (canAdvance ? 1u : 0u);

    void* nextButton = reinterpret_cast<void*>(self->field_0030);
    reinterpret_cast<SetButtonState>(
        CSF_AT(void**, nextButton, 0)[0x68 / 4])(
            nextButton, nullptr, nextState, 1, 0);

    nextButton = reinterpret_cast<void*>(self->field_0030);
    uint32_t result = reinterpret_cast<GetButtonState>(
        CSF_AT(void**, nextButton, 0)[0x6C / 4])(nextButton, nullptr);

    if ((result & 0xFFu) == 0)
    {
        result = static_cast<uint32_t>(CGuiSlider_ForwardSelfToParent(
            reinterpret_cast<void*>(self->field_002c), nullptr));
    }

    const bool usesInlineStorage = CSF_AT(uint32_t, temporaryText, 0x18) < 8;
    self->field_0018 = 0xFFFFFFFFu;

    if (!usesInlineStorage)
    {
        result = CSF_AT(uint32_t, temporaryText, 0x04);
        if (result != 0)
        {
            result = static_cast<uint32_t>(
                Mem_FreeDebug(reinterpret_cast<void*>(result)));
        }
    }

    return reinterpret_cast<void*>(result);
}

HOOK(0x00532ED0, CMenuEula_UpdateNavigationButtons, "auto");
