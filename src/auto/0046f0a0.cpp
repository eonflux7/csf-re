#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(uint32_t*, __stdcall, InitializeScriptEntry, 0x0046EEE0,
         (uint32_t* a1, int a2));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

// Walks an array of 12-byte script entries, initializing each one with the
// owner in ECX. If initialization throws, the entries already initialized are
// torn down through their vtable deleting destructor (slot 0, flag 0) and the
// exception is rethrown.
static uint32_t* __cdecl InitScriptEntryArray(
    uint32_t* firstEntryPtr, unsigned int entryCount, int argument,
    int ownerAddr, int unusedArgument)
{
    (void)unusedArgument;

    constexpr std::uint32_t kEntrySize = 12;

    const auto firstEntry = static_cast<std::uint32_t>(
        reinterpret_cast<std::uintptr_t>(firstEntryPtr));
    auto currentEntry = firstEntry;
    uint32_t* result = nullptr;

    try {
        while (entryCount != 0) {
            // ECX carries the owner; the two stack arguments follow.
            result = reinterpret_cast<
                uint32_t* (__fastcall*)(void*, void*, uint32_t*, int)>(
                    InitializeScriptEntry)(
                        reinterpret_cast<void*>(
                            static_cast<std::uint32_t>(ownerAddr)),
                        nullptr,
                        reinterpret_cast<uint32_t*>(currentEntry),
                        argument);
            --entryCount;
            currentEntry += kEntrySize;
        }
    } catch (...) {
        auto entry = firstEntry;
        while (entry != currentEntry) {
            void* object = reinterpret_cast<void*>(entry);
            reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, object, 0)[0])(object, nullptr, 0);
            entry += kEntrySize;
        }
        RethrowCppException(nullptr, nullptr);
    }

    return result;
}

HOOK(0x0046F0A0, InitScriptEntryArray, "auto");
