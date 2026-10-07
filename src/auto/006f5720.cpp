#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall C_008627d8_DispatchToChildren(
    C_008627d8* self, void* /*edx*/, int argument)
{
    const uint32_t childCount = self->field_000c;
    int result; // Original leaves EAX unchanged when childCount is zero.

    for (uint32_t index = 0; index < childCount; ++index)
    {
        const uint32_t arrayAddress = self->field_0004;
        result = static_cast<int>(arrayAddress);

        void* child = reinterpret_cast<void**>(arrayAddress)[index];
        if (child != nullptr)
        {
            result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, child, 0)[3])(child, nullptr, argument);
        }
    }

    return result;
}

HOOK(0x006F5720, C_008627d8_DispatchToChildren, "auto");
