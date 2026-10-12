#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_AllocDebug, 0x00690DC0,
         (std::size_t size, int flags, int tag));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(C_0082d410*, __fastcall, InitializeCampaignEntry, 0x0043F380,
         (C_0082d410* self, void* edx, C_0082d410* a2, uint32_t a3,
          C_00833908* a4, int a5, char a6));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

static C_0082d410* __stdcall CreateCampaignEntry(
    C_0082d410* vtblArg, uint32_t arg2, C_00833908* arg3,
    int arg4, char arg5)
{
    const int allocation = Mem_AllocDebug(0x30, 0, 0);
    C_0082d410* entry = reinterpret_cast<C_0082d410*>(
        static_cast<std::uintptr_t>(allocation));

    try {
        if (entry) {
            InitializeCampaignEntry(entry, nullptr, vtblArg, arg2,
                                    arg3, arg4, arg5);
        }
    } catch (...) {
        if (entry) {
            Mem_FreeDebug(entry);
        }
        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return entry;
}

HOOK(0x0043F8E0, CreateCampaignEntry, "auto");
