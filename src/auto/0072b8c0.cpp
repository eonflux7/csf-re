#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int (__fastcall* const CGuiControl_PassesEligibilityCheck)(
    CGuiControl* self, void* edx) =
    reinterpret_cast<int (__fastcall*)(CGuiControl*, void*)>(0x00718700);

static CGuiControl* __fastcall CGuiPanel_FindFirstEligibleChild(
    CGuiPanel* self, void* /*edx*/)
{
    const std::uint32_t initialBegin = self->field_0138;
    std::uint32_t childCount = 0;
    if (initialBegin != 0) {
        const std::uint32_t byteDifference = self->field_013c - initialBegin;
        childCount = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(byteDifference) >> 2);
    }

    CGuiControl* result = nullptr;
    for (std::uint32_t index = 0;
         index < childCount && result == nullptr;
         ++index) {
        auto* child = reinterpret_cast<CGuiControl**>(
            self->field_0138)[index];

        const auto virtualPredicate =
            reinterpret_cast<std::uint8_t (__fastcall*)(void*, void*)>(
                CSF_AT(void**, child, 0)[0x6C / 4]);

        if (virtualPredicate(child, nullptr) != 0) {
            if (static_cast<std::uint8_t>(
                    CGuiControl_PassesEligibilityCheck(child, nullptr)) != 0) {
                result = child;
            }
        }
    }
    return result;
}

HOOK(0x0072B8C0, CGuiPanel_FindFirstEligibleChild, "auto");
