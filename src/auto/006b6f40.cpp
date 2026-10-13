#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_AllocDebug, 0x00690DC0,
         (std::size_t size, int flags, int tag));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(uint32_t*, __fastcall, GfxWorld_InitializeWorldObject, 0x006B6CB0,
         (void* self, void* edx, int a2, int a3, int a4, uint32_t* a5, char a6));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

static uint32_t* __stdcall GfxWorld_CreateWorldObject(
    int arg1, int arg2, int arg3, uint32_t* arg4, char arg5)
{
    const int allocation = Mem_AllocDebug(0x18, 0, 0);
    uint32_t* worldObject = reinterpret_cast<uint32_t*>(
        static_cast<std::uintptr_t>(allocation));

    try {
        if (worldObject) {
            GfxWorld_InitializeWorldObject(worldObject, nullptr,
                                           arg1, arg2, arg3, arg4, arg5);
        }
    } catch (...) {
        if (worldObject) {
            Mem_FreeDebug(worldObject);
        }
        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return worldObject;
}

HOOK(0x006B6F40, GfxWorld_CreateWorldObject, "auto");
