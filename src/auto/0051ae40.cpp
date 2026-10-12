#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint8_t*, __stdcall, Tables_ProcessEntry, 0x0051ADB0, (uint8_t* a1, uint8_t* a2));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666, (void* exceptionObject, void* throwInformation));

static uint8_t* __cdecl BddTables_ProcessEntryRange(
    uint8_t* source, int count, uint8_t* context, int unusedContext, int unusedArgument)
{
    (void)unusedContext;
    (void)unusedArgument;

    uint8_t* result = nullptr;
    uint32_t remaining = static_cast<uint32_t>(count);
    try {
        while (remaining != 0) {
            result = Tables_ProcessEntry(source, context);
            --remaining;
            ++source;
        }
    } catch (...) {
        RethrowCppException(nullptr, nullptr);
    }
    return result;
}

HOOK(0x0051AE40, BddTables_ProcessEntryRange, "auto");
