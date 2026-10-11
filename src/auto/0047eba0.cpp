#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666, (void* exceptionObject, void* throwInformation));

static uint32_t* __cdecl AiAttackTasks_CopyDwordRange(
    uint32_t* sourceBegin, uint32_t* sourceEnd, uint32_t* destination,
    int /*unused1*/, int /*unused2*/)
{
    try {
        while (sourceBegin != sourceEnd) {
            if (destination) {
                *destination = *sourceBegin;
            }
            destination = reinterpret_cast<uint32_t*>(
                reinterpret_cast<uintptr_t>(destination) + 4);
            sourceBegin = reinterpret_cast<uint32_t*>(
                reinterpret_cast<uintptr_t>(sourceBegin) + 4);
        }
    } catch (...) {
        RethrowCppException(nullptr, nullptr);
    }
    return destination;
}

HOOK(0x47EBA0, AiAttackTasks_CopyDwordRange, "auto");
