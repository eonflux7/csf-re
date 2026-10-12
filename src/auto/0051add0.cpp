#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(uint8_t*, __stdcall, sub_51ADB0, 0x0051ADB0, (uint8_t* a1, uint8_t* a2));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666, (void* exceptionObject, void* throwInformation));

static uint8_t* __cdecl BddTables_DispatchByteRange(
    uint8_t* source, uint8_t* sourceEnd, uint8_t* destination,
    int /*unusedContext*/, int /*unused*/)
{
    try {
        while (source != sourceEnd) {
            sub_51ADB0(destination, source);
            ++destination;
            ++source;
        }
    } catch (...) {
        RethrowCppException(nullptr, nullptr);
    }

    return destination;
}

HOOK(0x0051ADD0, BddTables_DispatchByteRange, "auto");
