#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_AllocDebug, 0x00690DC0,
         (std::size_t size, int flags, int tag));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(uint32_t*, __fastcall, InitializeRecordWithPayload, 0x005199B0,
         (void* self, void* edx, int a2, int a3, int a4,
          uint32_t* a5, char a6));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

static int __stdcall CreateRecordWithPayload(
    int first, int second, int third, int payload, char flag)
{
    const int allocation = Mem_AllocDebug(0x14, 0, 0);
    void* record = reinterpret_cast<void*>(
        static_cast<std::uintptr_t>(allocation));

    try {
        if (record) {
            InitializeRecordWithPayload(
                record, nullptr, first, second, third,
                reinterpret_cast<uint32_t*>(
                    static_cast<std::uintptr_t>(payload)),
                flag);
        }
    } catch (...) {
        if (record) {
            Mem_FreeDebug(record);
        }
        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return allocation;
}

HOOK(0x00733B70, CreateRecordWithPayload, "auto");
