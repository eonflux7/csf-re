#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(uint32_t*, __stdcall, ProcessOwnerEntry, 0x0044AEB0,
         (uint32_t* a1, int a2));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

static uint32_t* __cdecl InvokeEntryCallsForOwner(
    uint32_t* firstEntry, int entryCount, int argument,
    int ownerAddr, int unusedArgument)
{
    (void)unusedArgument;

    auto currentEntry = firstEntry;
    uint32_t* result = nullptr;

    try {
        while (entryCount != 0) {
            // Preserve ECX = owner as well as the two stack arguments,
            // despite the packet's stdcall binding for this callee.
            result = reinterpret_cast<
                uint32_t* (__fastcall*)(void*, void*, uint32_t*, int)>(
                    ProcessOwnerEntry)(
                        reinterpret_cast<void*>(
                            static_cast<std::uint32_t>(ownerAddr)),
                        nullptr, currentEntry, argument);
            --entryCount;
            currentEntry += 3;
        }
    } catch (...) {
        auto entry = firstEntry;
        while (entry != currentEntry) {
            void* object = reinterpret_cast<void*>(entry);
            reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, object, 0)[0])(object, nullptr, 0);
            entry += 3;
        }
        RethrowCppException(nullptr, nullptr);
    }

    return result;
}

HOOK(0x0044B070, InvokeEntryCallsForOwner, "auto");
