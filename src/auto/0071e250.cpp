#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall CGuiBuilder_SetHandler(
    CGuiBuilder* self, void* /*edx*/, int index, int handler)
{
    int result; // Original preserves incoming EAX when handler is zero.

    if (handler != 0)
    {
        result = static_cast<int>(self->field_000c);
        void* previousHandler = reinterpret_cast<void*>(
            reinterpret_cast<std::uint32_t*>(
                static_cast<std::uint32_t>(result))[index]);

        if (previousHandler != nullptr)
        {
            reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, previousHandler, 0)[0])(
                    previousHandler, nullptr, 1);

            result = static_cast<int>(self->field_000c);
            reinterpret_cast<std::uint32_t*>(
                static_cast<std::uint32_t>(result))[index] = 0;
        }

        reinterpret_cast<std::uint32_t*>(self->field_000c)[index] =
            static_cast<std::uint32_t>(handler);
    }

    return result;
}

HOOK(0x0071E250, CGuiBuilder_SetHandler, "auto");
