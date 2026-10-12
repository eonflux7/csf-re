#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_AllocDebug, 0x00690DC0,
         (std::size_t size, int flags, int tag));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(uint32_t*, __fastcall, TypeRegistry_InitializeEntry, 0x0054AEC0,
         (void* self, void* edx, int a2, int a3, int a4,
          uint32_t* a5, char a6));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

static uint32_t* __stdcall TypeRegistry_CreateEntry(
    int arg1, int arg2, int arg3, uint32_t* arg4, char arg5)
{
    const int allocation = Mem_AllocDebug(0x1C, 0, 0);
    uint32_t* entry = reinterpret_cast<uint32_t*>(
        static_cast<std::uintptr_t>(allocation));

    try {
        if (entry) {
            TypeRegistry_InitializeEntry(
                entry, nullptr, arg1, arg2, arg3, arg4, arg5);
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

HOOK(0x0054B0E0, TypeRegistry_CreateEntry, "auto");
