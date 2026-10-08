#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, CGuiControl_ProcessSelectionInput_718360, 0x00718360,
         (CGuiControl* self, void* edx, int input));
CSF_FUNC(int, __fastcall, CGuiSlider_IsSelectableAndNotType10, 0x00718700,
         (CGuiControl* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, EraseFourByteVectorElement, 0x0072BA90,
         (uint32_t* self, void* edx, uint32_t* result, char* position));
CSF_FUNC(uint32_t*, __fastcall, InsertSingleElementAndReturnIterator, 0x0072C530,
         (uint32_t* self, void* edx, uint32_t* result,
          uint32_t* position, uint32_t* element));

static char __fastcall CGuiPanel_ProcessSelectionInput(
    CGuiPanel* self, void* /*edx*/, int input)
{
    char result = static_cast<char>(
        ((int(__fastcall*)(void*, void*))
             CSF_AT(void**, self, 0)[0x6C / 4])(self, nullptr));
    if (!result)
        return result;

    result = static_cast<char>(
        CGuiSlider_IsSelectableAndNotType10(self, nullptr));
    if (!result)
        return result;

    result = static_cast<char>(
        ((int(__fastcall*)(void*, void*))
             CSF_AT(void**, self, 0)[0xC4 / 4])(self, nullptr));
    if (!result)
        return result;

    const uint32_t childrenBegin = self->field_0138;
    const uint32_t childCount = childrenBegin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(self->field_013c - childrenBegin) >> 2)
        : 0;

    for (uint32_t index = 0; index < childCount; ++index)
    {
        uint32_t childAddress =
            reinterpret_cast<uint32_t*>(self->field_0138)[index];
        CGuiControl* child = reinterpret_cast<CGuiControl*>(childAddress);

        if (!static_cast<uint8_t>(
                ((int(__fastcall*)(void*, void*))
                     CSF_AT(void**, child, 0)[0x6C / 4])(child, nullptr)))
            continue;

        if (!static_cast<uint8_t>(
                CGuiSlider_IsSelectableAndNotType10(child, nullptr)))
            continue;

        if (!static_cast<uint8_t>(
                ((int(__fastcall*)(void*, void*))
                     CSF_AT(void**, child, 0)[0xC4 / 4])(child, nullptr)))
            continue;

        const uint8_t stateBefore = static_cast<uint8_t>(
            ((int(__fastcall*)(void*, void*))
                 CSF_AT(void**, child, 0)[0x140 / 4])(child, nullptr));

        ((void(__fastcall*)(void*, void*, int))
             CSF_AT(void**, child, 0)[0x18 / 4])(child, nullptr, input);

        const uint8_t stateAfter = static_cast<uint8_t>(
            ((int(__fastcall*)(void*, void*))
                 CSF_AT(void**, child, 0)[0x140 / 4])(child, nullptr));

        const bool previouslySet =
            (stateBefore & 4) != 0 && (stateBefore & 1) != 0;
        const bool nowCleared =
            (stateAfter & 4) == 0 && (stateAfter & 1) != 0;

        if (!self->field_018a && previouslySet && nowCleared)
        {
            auto* childrenVector = reinterpret_cast<uint32_t*>(
                reinterpret_cast<uint8_t*>(self) + 0x134);
            uint32_t eraseResult;
            uint32_t insertResult;

            EraseFourByteVectorElement(
                childrenVector, nullptr, &eraseResult,
                reinterpret_cast<char*>(self->field_0138 + index * 4u));

            InsertSingleElementAndReturnIterator(
                childrenVector, nullptr, &insertResult,
                reinterpret_cast<uint32_t*>(self->field_0138),
                &childAddress);
        }
    }

    return CGuiControl_ProcessSelectionInput_718360(self, nullptr, input);
}

HOOK(0x0072C610, CGuiPanel_ProcessSelectionInput, "auto");
