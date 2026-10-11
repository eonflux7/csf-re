#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "types/auto_classes.h"

static char __fastcall S_0070e170_UpdateAudioBuffer(
    S_0070e170* self, void* /*edx*/, char clearBuffer)
{
    uint32_t activeHalf;
    if (self->field_0039)
    {
        void* soundBuffer = reinterpret_cast<void*>(self->field_0018);
        activeHalf =
            reinterpret_cast<uint32_t(__fastcall*)(void*, void*)>(
                CSF_AT(void**, soundBuffer, 0)[0x28 / 4])(
                soundBuffer, nullptr);
    }
    else
    {
        activeHalf = 1 - self->field_0020;
    }

    const uint32_t writeHalf = self->field_0020;
    if (writeHalf == activeHalf)
        return 0;

    const uint32_t bufferSize = self->field_001c;
    void* soundBuffer = reinterpret_cast<void*>(self->field_0018);
    const uint32_t sourceBuffer = self->field_0014;

    if (reinterpret_cast<bool(__fastcall*)(void*, void*, uint32_t,
                                           uint32_t, uint32_t)>(
            CSF_AT(void**, soundBuffer, 0)[0x2C / 4])(
            soundBuffer, nullptr, sourceBuffer, bufferSize,
            writeHalf * bufferSize))
    {
        if (!self->field_0039)
        {
            soundBuffer = reinterpret_cast<void*>(self->field_0018);
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, soundBuffer, 0)[0x04 / 4])(
                soundBuffer, nullptr, 1);
            self->field_0039 = 1;
        }

        self->field_0020 = 1 - self->field_0020;
        self->field_0038 = 0;

        if (clearBuffer)
        {
            const uint32_t clearCount = self->field_0034;
            if (static_cast<int32_t>(clearCount) < 2)
            {
                const uint32_t clearSize = self->field_001c;
                void* buffer = reinterpret_cast<void*>(self->field_0014);
                self->field_0034 = clearCount + 1;
                std::memset(buffer, 0, clearSize);
                self->field_0038 = 1;
            }
        }
    }

    return 1;
}

HOOK(0x0070E550, S_0070e170_UpdateAudioBuffer, "auto");
