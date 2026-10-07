#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static void __fastcall C_008655e8_ProcessChildrenInStateOne(
    C_008655e8* self, void* /*edx*/)
{
    self->field_0028 = 1;

    const std::uint32_t begin = self->field_001c;
    const std::uint32_t count = begin
        ? static_cast<std::uint32_t>(
              static_cast<std::int32_t>(self->field_0020 - begin) >> 2)
        : 0;

    for (std::uint32_t index = 0; index < count; ++index)
    {
        void* child = reinterpret_cast<void**>(self->field_001c)[index];
        const int state =
            reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, child, 0)[5])(child, nullptr);

        if (state == 1)
        {
            child = reinterpret_cast<void**>(self->field_001c)[index];
            reinterpret_cast<void(__fastcall*)(void*, void*)>(
                CSF_AT(void**, child, 0)[2])(child, nullptr);
        }
    }

    self->field_0028 = 0;
}

HOOK(0x007270F0, C_008655e8_ProcessChildrenInStateOne, "auto");
