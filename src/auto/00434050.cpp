#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

static void* __cdecl Hud_CopyStringRecordsBackward(
    void* rangeBegin, void* rangeEnd, C_0082d410* destination, int /*unusedArg*/)
{
    auto* sourceCursor = static_cast<std::uint8_t*>(rangeEnd);
    auto* destinationCursor = reinterpret_cast<std::uint8_t*>(destination);

    if (rangeBegin != rangeEnd)
    {
        auto* sourceFields = sourceCursor + 0x20;
        auto* destinationFields = destinationCursor + 0x20;

        do
        {
            sourceCursor -= 0x30;
            destinationCursor -= 0x30;
            sourceFields -= 0x30;
            destinationFields -= 0x30;

            AssignStringSubstring(
                destinationCursor, nullptr, sourceCursor,
                0, static_cast<std::uint32_t>(-1));

            CSF_AT(std::uint32_t, destinationFields, -4) =
                CSF_AT(std::uint32_t, sourceFields, -4);
            CSF_AT(std::uint32_t, destinationFields, 0) =
                CSF_AT(std::uint32_t, sourceFields, 0);
            CSF_AT(std::uint32_t, destinationFields, 4) =
                CSF_AT(std::uint32_t, sourceFields, 4);
            CSF_AT(std::uint32_t, destinationFields, 8) =
                CSF_AT(std::uint32_t, sourceFields, 8);
            CSF_AT(std::uint32_t, destinationFields, 0xC) =
                CSF_AT(std::uint32_t, sourceFields, 0xC);
        } while (sourceCursor != rangeBegin);
    }

    return destinationCursor;
}

HOOK(0x00434050, Hud_CopyStringRecordsBackward, "auto");
