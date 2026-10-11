#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static bool __fastcall C_00863cd4_WriteSoundBuffer(
    C_00863cd4* self, void* /*edx*/, char* source,
    unsigned int byteCount, int offset)
{
    int playCursor = 0;
    int writeCursor = 0;
    void* buffer = reinterpret_cast<void*>(self->field_0004);

    ((void(__stdcall*)(void*, int*, int*))
        CSF_AT(void**, buffer, 0)[0x10 / 4])(
            buffer, &playCursor, &writeCursor);

    bool canWrite;
    if (writeCursor > playCursor)
        canWrite = offset < playCursor || offset >= writeCursor;
    else if (writeCursor < playCursor)
        canWrite = offset < playCursor && offset >= writeCursor;
    else
        canWrite = true;

    char* lockedData = nullptr;
    int lockedBytes = 0;
    if (!canWrite)
        return false;

    buffer = reinterpret_cast<void*>(self->field_0004);
    int status = ((int(__stdcall*)(
        void*, int, unsigned int, char**, int*,
        uint32_t, uint32_t, uint32_t))
        CSF_AT(void**, buffer, 0)[0x2C / 4])(
            buffer, offset, byteCount, &lockedData, &lockedBytes, 0, 0, 0);

    if (status == -2005401450)
    {
        buffer = reinterpret_cast<void*>(self->field_0004);
        status = ((int(__stdcall*)(void*))
            CSF_AT(void**, buffer, 0)[0x50 / 4])(buffer);
        if (status != 0)
            return false;

        buffer = reinterpret_cast<void*>(self->field_0004);
        status = ((int(__stdcall*)(
            void*, int, unsigned int, char**, int*,
            uint32_t, uint32_t, uint32_t))
            CSF_AT(void**, buffer, 0)[0x2C / 4])(
                buffer, offset, byteCount, &lockedData, &lockedBytes, 0, 0, 0);
    }

    if (status != 0)
        return false;

    const unsigned int wholeBytes = 4 * (byteCount >> 2);
    if (source)
    {
        std::memcpy(lockedData, source, wholeBytes);
        std::memcpy(lockedData + wholeBytes, source + wholeBytes, byteCount & 3);
    }
    else
    {
        std::memset(lockedData, 0, wholeBytes);
        std::memset(lockedData + wholeBytes, 0, byteCount & 3);
    }

    buffer = reinterpret_cast<void*>(self->field_0004);
    ((void(__stdcall*)(void*, char*, unsigned int, uint32_t, uint32_t))
        CSF_AT(void**, buffer, 0)[0x4C / 4])(
            buffer, lockedData, byteCount, 0, 0);

    return true;
}

HOOK(0x007112E0, C_00863cd4_WriteSoundBuffer, "auto");
