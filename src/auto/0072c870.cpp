#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(unsigned int, __fastcall, AppendFourByteElementWithContext, 0x0072C780,
         (void* self, void* edx, std::uint32_t* control));

static int __fastcall CGuiPanel_AddControl(
    CGuiPanel* self, void* /*edx*/, std::uint32_t controlAddress)
{
    int result;
    if (controlAddress != 0) {
        result = self->field_0148;
        if (result == 0) {
            void* const control = reinterpret_cast<void*>(controlAddress);

            reinterpret_cast<void (__fastcall*)(
                void*, void*, std::uint32_t)>(
                CSF_AT(void**, self, 0)[0x264 / 4])(
                    self, nullptr, controlAddress);

            self->field_0148 = 1;
            reinterpret_cast<void (__fastcall*)(
                void*, void*, CGuiPanel*)>(
                CSF_AT(void**, control, 0)[0x108 / 4])(
                    control, nullptr, self);

            self->field_0148 = 0;
            AppendFourByteElementWithContext(
                reinterpret_cast<std::uint8_t*>(self) + 0x134,
                nullptr, &controlAddress);

            reinterpret_cast<void (__fastcall*)(
                void*, void*, int)>(
                CSF_AT(void**, self, 0)[0x230 / 4])(
                    self, nullptr, 0);

            result = static_cast<int>(self->field_014c);
            if (result == 0)
                self->field_014c =
                    reinterpret_cast<std::uint32_t>(control);
        }
    }
    return result;
}

HOOK(0x0072C870, CGuiPanel_AddControl, "auto");
