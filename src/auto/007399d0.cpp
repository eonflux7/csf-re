#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_AllocDebug, 0x00690DC0,
         (std::size_t size, int flags, int tag));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(std::uint32_t*, __fastcall, InitializeRecordWithPayload,
         0x00739480,
         (std::uint32_t* self, void* edx, int first, int second,
          int third, const void* payload, char flag));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

static std::uint32_t* __stdcall CreateRecordWithPayload(
    int first, int second, int third, const void* payload, char flag)
{
    auto* record = reinterpret_cast<std::uint32_t*>(
        static_cast<std::uintptr_t>(Mem_AllocDebug(0x4C, 0, 0)));

    try {
        if (record) {
            InitializeRecordWithPayload(
                record, nullptr, first, second, third, payload, flag);
        }
    } catch (...) {
        if (record) {
            Mem_FreeDebug(record);
        }
        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return record;
}

HOOK(0x007399D0, CreateRecordWithPayload, "auto");
