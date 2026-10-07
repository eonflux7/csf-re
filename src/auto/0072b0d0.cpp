#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static const auto CGuiControl_IsProcessingEnabled =
    reinterpret_cast<int (__fastcall*)(CGuiControl*, void*)>(0x00718700);

static const auto CGuiControl_ProcessSelf =
    reinterpret_cast<char (__fastcall*)(void*, void*, int)>(0x00718450);

static char __fastcall CGuiPanel_ProcessChildrenAndSelf(
    CGuiPanel* self, void* /*edx*/, int argument)
{
    char result = static_cast<char>(
        reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0x6C / 4])(self, nullptr));
    if (!result)
        return result;

    result = static_cast<char>(CGuiControl_IsProcessingEnabled(self, nullptr));
    if (!result)
        return result;

    result = static_cast<char>(
        reinterpret_cast<int (__fastcall*)(void*, void*)>(
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
        void* child = reinterpret_cast<void**>(self->field_0138)[index];
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, child, 0)[0x20 / 4])(
                child, nullptr, argument);
    }

    return CGuiControl_ProcessSelf(self, nullptr, argument);
}

HOOK(0x0072B0D0, CGuiPanel_ProcessChildrenAndSelf, "auto");
