#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_AllocDebug, 0x00690DC0,
         (std::size_t size, int flags, int tag));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

static void __cdecl InitializeRecordWords(void* record)
{
    const auto address = static_cast<std::uint32_t>(
        reinterpret_cast<std::uintptr_t>(record));

    if (address != 0) {
        CSF_AT(volatile std::uint32_t, record, 0x00) = 0;
    }
    if (address + 4u != 0) {
        CSF_AT(volatile std::uint32_t, record, 0x04) = 0;
    }
    if (address + 8u != 0) {
        CSF_AT(volatile std::uint32_t, record, 0x08) = 0;
    }
}

static int __cdecl CreateEmptyRecord()
{
    const int allocation = Mem_AllocDebug(0x4C, 0, 0);
    const auto address = static_cast<std::uint32_t>(allocation);
    void* record = reinterpret_cast<void*>(
        static_cast<std::uintptr_t>(address));

    // Keep construction opaque so optimization retains the unwind calls.
    void (__cdecl* volatile initializeWords)(void*) = InitializeRecordWords;
    try {
        initializeWords(record);
    } catch (...) {
        if (address != 0) {
            Mem_FreeDebug(record);
        }
        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    CSF_AT(volatile std::uint8_t, record, 0x48) = 1;
    CSF_AT(volatile std::uint8_t, record, 0x49) = 0;
    return allocation;
}

HOOK(0x00739A70, CreateEmptyRecord, "auto");
