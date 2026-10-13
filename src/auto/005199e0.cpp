#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_AllocDebug, 0x00690DC0,
         (std::size_t size, int flags, int tag));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(uint32_t*, __fastcall, InitializeRecordWithPayload_5199B0, 0x005199B0,
         (void* self, void* edx, int a2, int a3, int a4, uint32_t* a5, char a6));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

static uint32_t* __stdcall CreateRecordWithPayload(
    int arg1, int arg2, int arg3, uint32_t* arg4, char arg5)
{
    const int allocation = Mem_AllocDebug(0x14, 0, 0);
    uint32_t* record = reinterpret_cast<uint32_t*>(
        static_cast<std::uintptr_t>(allocation));

    try {
        if (record) {
            InitializeRecordWithPayload_5199B0(record, nullptr, arg1, arg2,
                                               arg3, arg4, arg5);
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

HOOK(0x005199E0, CreateRecordWithPayload, "auto");
