#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, sub_64D6B0, 0x0064D6B0, (void* self, void* edx));
CSF_GLOBAL(std::uint32_t, g_NetPlayerClassId, 0x008BC15C);

static int __cdecl NetPlayerDdl_CallWithContext(int argument)
{
    (void)argument;
    void* context = reinterpret_cast<void*>(
        static_cast<std::uintptr_t>(g_NetPlayerClassId));
    return sub_64D6B0(context, nullptr);
}

HOOK(0x00744630, NetPlayerDdl_CallWithContext, "auto");
