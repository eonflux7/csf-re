#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666, (void* exceptionObject, void* throwInformation));

static uint32_t* __cdecl CopyFourDwordBlocks(
    uint32_t* source, uint32_t* sourceEnd, uint32_t* destination,
    int /*unused*/, int /*unused*/)
{
    try {
        while (source != sourceEnd) {
            if (destination) {
                destination[0] = source[0];
                destination[1] = source[1];
                destination[2] = source[2];
                destination[3] = source[3];
            }
            destination = reinterpret_cast<uint32_t*>(
                reinterpret_cast<uintptr_t>(destination) + 0x10);
            source += 4;
        }
    } catch (...) {
        RethrowCppException(nullptr, nullptr);
    }
    return destination;
}

HOOK(0x7323D0, CopyFourDwordBlocks, "auto");
