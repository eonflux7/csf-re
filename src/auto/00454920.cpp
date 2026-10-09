#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __stdcall, InitializeEntry, 0x00453BD0,
         (int a1, int a2));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

static void* __cdecl InitStructArrayForOwner(
    int firstEntryAddr, unsigned int entryCount, int argument,
    int ownerAddr, int unusedArgument)
{
    (void)unusedArgument;

    const auto firstEntry = static_cast<std::uint32_t>(firstEntryAddr);
    auto currentEntry = firstEntry;
    void* result = nullptr;

    try {
        while (entryCount != 0) {
            // Preserve ECX = owner as well as the two stack arguments,
            // despite the packet's stdcall binding for this callee.
            result = reinterpret_cast<
                void* (__fastcall*)(void*, void*, int, int)>(
                    InitializeEntry)(
                        reinterpret_cast<void*>(
                            static_cast<std::uint32_t>(ownerAddr)),
                        nullptr, static_cast<int>(currentEntry), argument);
            --entryCount;
            currentEntry += 48;
        }
    } catch (...) {
        auto entry = firstEntry;
        while (entry != currentEntry) {
            void* object = reinterpret_cast<void*>(entry);
            reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, object, 0)[0])(object, nullptr, 0);
            entry += 48;
        }
        RethrowCppException(nullptr, nullptr);
    }

    return result;
}

HOOK(0x00454920, InitStructArrayForOwner, "auto");
