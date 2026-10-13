#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, C_00863d80_Init, 0x00711CF0, (C_00863d80* self, void* , int handleValue, char enabledFlag));

// Creates the per-object effect instance when the object reports it is enabled.
// Returns the raw eax of the enable check when it is false, the init result on success,
// and 0 when the allocation fails.
static int __fastcall C_00863ba8_CreateEffectInstance(C_00863ba8* self, void* /*edx*/)
{
    // Virtual slot 0xB8 (index 46): enable check, returns a byte-sized bool.
    int result = ((int (__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0xB8 / 4])(self, nullptr);
    if ((uint8_t)result)
    {
        C_00863d80* effect = reinterpret_cast<C_00863d80*>(static_cast<uintptr_t>(CoreMemory_AllocUntagged(440)));
        if (effect)
        {
            // Virtual slot 0xC0 (index 48): returns the effect flag, called before reading field_0004.
            char enabledFlag = (char)((int (__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0xC0 / 4])(self, nullptr);
            uint32_t* initResult = C_00863d80_Init(effect, nullptr, (int)self->field_0004, enabledFlag);
            result = (int)reinterpret_cast<uintptr_t>(initResult);
            self->field_0014 = static_cast<uint32_t>(result);
        }
        else
        {
            self->field_0014 = 0;
            return 0;
        }
    }
    return result;
}

HOOK(0x70F1D0, C_00863ba8_CreateEffectInstance, "auto");
