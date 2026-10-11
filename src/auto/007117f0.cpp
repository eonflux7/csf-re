#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static bool __fastcall C_00863d18_ReadBuffer(
    C_00863d18* self, void* /*edx*/,
    char* destination, int byteCount, int offset)
{
    void* buffer = reinterpret_cast<void*>(self->field_0008);
    const void* firstRegion = nullptr;
    std::uint32_t firstSize = 0;
    char* secondRegion = nullptr;
    std::uint32_t secondSize = 0;

    if (!buffer)
        return false;

    const int result =
        reinterpret_cast<int (__stdcall*)(
            void*, int, int, const void**, std::uint32_t*,
            char**, std::uint32_t*, std::uint32_t)>(
                CSF_AT(void**, buffer, 0)[8])(
                    buffer, offset, byteCount, &firstRegion, &firstSize,
                    &secondRegion, &secondSize, 0);

    if (result != 0)
        return false;

    std::memcpy(destination, firstRegion, firstSize);
    if (secondRegion)
        std::memcpy(destination + firstSize, secondRegion, secondSize);

    buffer = reinterpret_cast<void*>(self->field_0008);
    reinterpret_cast<void (__stdcall*)(
        void*, const void*, std::uint32_t, char*, std::uint32_t)>(
            CSF_AT(void**, buffer, 0)[11])(
                buffer, firstRegion, firstSize, secondRegion, secondSize);

    return true;
}

HOOK(0x007117F0, C_00863d18_ReadBuffer, "auto");
