#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_AllocDebug, 0x00690DC0,
         (std::size_t size, int flags, int tag));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

static void __cdecl BddLoader_InitializeRecordWords(void* record)
{
    const auto address = static_cast<std::uint32_t>(
        reinterpret_cast<std::uintptr_t>(record));

    if (address != 0) {
        CSF_AT(std::uint32_t, record, 0x00) = 0;
    }
    if (address + 4u != 0) {
        CSF_AT(std::uint32_t, record, 0x04) = 0;
    }
    if (address + 8u != 0) {
        CSF_AT(std::uint32_t, record, 0x08) = 0;
    }
}

static int __cdecl BddLoader_CreateEmptyRecord()
{
    const int allocation = Mem_AllocDebug(0x14, 0, 0);
    const auto address = static_cast<std::uint32_t>(allocation);
    void* record = reinterpret_cast<void*>(
        static_cast<std::uintptr_t>(address));

    // Initializer is called through a pointer so the guarded unwind path
    // below is kept in the compiled function instead of being optimized away.
    void (__cdecl* initializeWords)(void*) = BddLoader_InitializeRecordWords;
    try {
        initializeWords(record);
    } catch (...) {
        // Unwind action: free the block if one was allocated, then rethrow
        // the active exception with null arguments (original: push 0; push 0; call throw).
        if (allocation != 0) {
            Mem_FreeDebug(record);
        }
        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    CSF_AT(std::uint8_t, record, 0x10) = 1;
    CSF_AT(std::uint8_t, record, 0x11) = 0;
    return allocation;
}

HOOK(0x0050E9D0, BddLoader_CreateEmptyRecord, "auto");
