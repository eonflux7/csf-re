#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall C_008655e8_InvokeChildCallbacks(
    C_008655e8* self, void* /*edx*/)
{
    const uint32_t initialBegin = self->field_001c;
    int result = static_cast<int32_t>(initialBegin);

    uint32_t childCount = 0;
    if (initialBegin != 0) {
        const uint32_t byteDifference = self->field_0020 - initialBegin;
        childCount = static_cast<uint32_t>(
            static_cast<int32_t>(byteDifference) >> 2);
    }

    for (uint32_t index = 0; index < childCount; ++index) {
        void* child = reinterpret_cast<void*>(
            reinterpret_cast<uint32_t*>(self->field_001c)[index]);

        result = ((int(__fastcall*)(void*, void*))
            CSF_AT(void**, child, 0)[6])(child, nullptr);
    }

    return result;
}

HOOK(0x00727160, C_008655e8_InvokeChildCallbacks, "auto");
