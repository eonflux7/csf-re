#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, UpdateSineCosineTransition, 0x0072ACA0,
         (CGuiPanel* self, void* edx, float deltaTime));
CSF_FUNC(char, __fastcall, CGuiControl_Update, 0x00719530,
         (CGuiControl* self, void* edx, float deltaTime));

static char __fastcall CGuiPanel_Update(
    CGuiPanel* self, void* /*edx*/, float deltaTime)
{
    const std::uint32_t childrenBegin = self->field_0138;
    std::uint32_t childCount = 0;

    if (childrenBegin != 0)
    {
        childCount = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(self->field_013c - childrenBegin) >> 2);
    }

    for (std::uint32_t index = 0; index < childCount; ++index)
    {
        void* child = reinterpret_cast<void**>(self->field_0138)[index];
        ((void(__fastcall*)(void*, void*, float))
            CSF_AT(void**, child, 0)[0x13C / 4])(
                child, nullptr, deltaTime);
    }

    UpdateSineCosineTransition(self, nullptr, deltaTime);
    return CGuiControl_Update(
        reinterpret_cast<CGuiControl*>(self), nullptr, deltaTime);
}

HOOK(0x0072BD30, CGuiPanel_Update, "auto");
