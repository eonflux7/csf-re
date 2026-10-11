#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, MissionScript_HasPendingState, 0x0069AF10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0085ee28_RefillBuffer, 0x006A2BB0, (void* self, void* edx));

static unsigned int __fastcall C_0085ee28_ReadBufferedBytes(
    C_0085ee28* self, void* /*edx*/, char* destination,
    unsigned int requestedBytes)
{
    if (self->field_2424 &&
        MissionScript_HasPendingState(
            reinterpret_cast<void*>(self->field_2424), nullptr))
    {
        void* pendingObject = reinterpret_cast<void*>(self->field_2424);
        reinterpret_cast<void (__fastcall*)(void*, void*)>(
            CSF_AT(void**, pendingObject, 0)[1])(pendingObject, nullptr);
    }

    unsigned int bytesRead = 0;
    if (!requestedBytes)
        return 0;

    for (;;)
    {
        unsigned int chunkBytes = self->field_0018 - self->field_0010;
        if (!chunkBytes)
            return 0;

        const unsigned int remainingBytes = requestedBytes - bytesRead;
        if (chunkBytes > remainingBytes)
            chunkBytes = remainingBytes;

        std::memcpy(
            destination,
            &CSF_AT(char, self, 0x20) + self->field_0010,
            chunkBytes);

        const unsigned int nextPosition = self->field_0010 + chunkBytes;
        const unsigned int bufferEnd = self->field_0018;
        bytesRead += chunkBytes;
        self->field_0010 = nextPosition;

        if (nextPosition == bufferEnd)
        {
            if (C_0085ee28_RefillBuffer(self, nullptr) == -1)
                return bytesRead;
        }

        destination += chunkBytes;
        if (bytesRead >= requestedBytes)
            return bytesRead;
    }
}

HOOK(0x006A2C60, C_0085ee28_ReadBufferedBytes, "auto");
