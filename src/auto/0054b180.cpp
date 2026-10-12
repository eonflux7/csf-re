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

static void __cdecl TypeRegistry_InitializeRecordWords(void* record)
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

static int __cdecl TypeRegistry_CreateEmptyRecord()
{
    const int allocation = Mem_AllocDebug(0x1C, 0, 0);
    void* record = reinterpret_cast<void*>(
        static_cast<std::uintptr_t>(
            static_cast<std::uint32_t>(allocation)));

    void (__cdecl* initializeWords)(void*) =
        TypeRegistry_InitializeRecordWords;
    try {
        initializeWords(record);
    } catch (...) {
        if (allocation != 0) {
            Mem_FreeDebug(record);
        }
        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    CSF_AT(std::uint8_t, record, 0x18) = 1;
    CSF_AT(std::uint8_t, record, 0x19) = 0;
    return allocation;
}

HOOK(0x0054B180, TypeRegistry_CreateEmptyRecord, "auto");
