#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, C_0083b668_UpdateBase, 0x004D01C0, (C_0083b668* self, void* /*edx*/, float a2));
CSF_FUNC(int, __fastcall, C_00835228_RunPhaseOne, 0x004A97A0, (void* self, void* /*edx*/, int a2));
CSF_FUNC(int, __fastcall, C_00835228_RunPhaseTwo, 0x004A9820, (void* self, void* /*edx*/, float a2));
CSF_FUNC(char, __fastcall, C_00835228_RunFallbackStep, 0x004A9450, (void* self, void* /*edx*/, int a2));

static char __fastcall C_00835228_UpdateTick(C_00835228* self, void* /*edx*/, int a2)
{
    // Result is the low byte of the last value the original leaves in eax/al.
    uint8_t result = self->field_0038;
    if (result == 0)
        return static_cast<char>(result);

    float timeDelta;
    std::memcpy(&timeDelta, &a2, sizeof(float));

    C_0083b668_UpdateBase(self, nullptr, timeDelta);

    if (self->field_0070 == 1)
    {
        C_00835228_RunPhaseOne(self, nullptr, a2);
    }
    else if (self->field_0070 == 2)
    {
        C_00835228_RunPhaseTwo(self, nullptr, timeDelta);
    }

    uint32_t activeCounter = self->field_0074;
    if (activeCounter != 0)
    {
        result = static_cast<uint8_t>(activeCounter - 1);
        if (activeCounter - 1 == 0)
        {
            result = self->field_0098;
            if (result != 0)
            {
                // Virtual slot 0xD8 / 4 = 54 on C_00835228's vtable.
                auto slotD8 = reinterpret_cast<char (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, self, 0)[0xD8 / 4]);
                result = static_cast<uint8_t>(slotD8(self, nullptr));
                if (result != 0)
                    self->field_0074 = 0;
            }
        }
    }
    else
    {
        result = static_cast<uint8_t>(C_00835228_RunFallbackStep(self, nullptr, a2));
    }

    return static_cast<char>(result);
}

HOOK(0x004A9910, C_00835228_UpdateTick, "auto");
