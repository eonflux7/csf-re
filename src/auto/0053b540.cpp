#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Applies the three stored toggle bytes (+0x14, +0x15, +0x16) to the
// option object referenced by +0x24, via vtable slots 7, 9 and 11.
// Returns the last slot's result, or 0 when the option object is null.
static int __fastcall CMenuGameOptions_ApplyToggleSettings(CMenuGameOptions* self, void* /*edx*/)
{
    int result = static_cast<int>(self->field_0024);
    if (result)
    {
        void* voiceOption = reinterpret_cast<void*>(self->field_0024);
        uint8_t invertAxisValue = self->field_0014;
        ((void(__fastcall*)(void*, void*, int))(CSF_AT(void**, voiceOption, 0)[7]))(
            voiceOption, nullptr, static_cast<int>(invertAxisValue));

        voiceOption = reinterpret_cast<void*>(self->field_0024);
        uint8_t subtitlesValue = self->field_0015;
        ((void(__fastcall*)(void*, void*, int))(CSF_AT(void**, voiceOption, 0)[9]))(
            voiceOption, nullptr, static_cast<int>(subtitlesValue));

        voiceOption = reinterpret_cast<void*>(self->field_0024);
        uint8_t voiceValue = self->field_0016;
        result = ((int(__fastcall*)(void*, void*, int))(CSF_AT(void**, voiceOption, 0)[11]))(
            voiceOption, nullptr, static_cast<int>(voiceValue));
    }
    return result;
}

HOOK(0x0053B540, CMenuGameOptions_ApplyToggleSettings, "auto");
