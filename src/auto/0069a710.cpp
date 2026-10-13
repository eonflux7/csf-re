#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_AllocDebug, 0x00690DC0,
         (std::size_t size, int flags, int tag));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(uint32_t*, __fastcall, GfxWorld_InitRecord, 0x006B6CB0,
         (void* self, void* edx, int a2, int a3, int a4, uint32_t* a5, char a6));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

static int __stdcall AllocateGfxWorldRecord(int a1, int a2, int a3, int a4, char a5)
{
    const int allocation = Mem_AllocDebug(0x18, 0, 0);
    void* record = reinterpret_cast<void*>(static_cast<std::uintptr_t>(allocation));

    try {
        if (record) {
            GfxWorld_InitRecord(record, nullptr, a1, a2, a3,
                                reinterpret_cast<uint32_t*>(static_cast<std::uintptr_t>(a4)),
                                a5);
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

HOOK(0x0069A710, AllocateGfxWorldRecord, "auto");
