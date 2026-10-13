#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666, (void* exceptionObject, void* throwInformation));

static uint32_t* __cdecl CopyQuadRepeatedToArray(
    uint32_t* destination, int count, uint32_t* source,
    int /*unused4*/, int /*unused5*/)
{
    uint32_t* cursor = destination;
    uint32_t remaining = static_cast<uint32_t>(count);

    try
    {
        while (remaining != 0)
        {
            if (cursor != nullptr)
            {
                cursor[0] = source[0];
                cursor[1] = source[1];
                cursor[2] = source[2];
                cursor[3] = source[3];
            }

            --remaining;
            cursor = reinterpret_cast<uint32_t*>(
                reinterpret_cast<uintptr_t>(cursor) + 0x10);
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
    }

    return cursor;
}

HOOK(0x732530, CopyQuadRepeatedToArray, "auto");
