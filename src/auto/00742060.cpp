#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x64D6B0 reads ecx on entry and pops 0 bytes, so it is bound as a __fastcall
// with only the two register arguments (no stack arguments declared).
CSF_FUNC(int, __fastcall, sub_64D6B0, 0x0064D6B0, (void* self, void* edx));
CSF_GLOBAL(int, g_NetCogibleDdlContext, 0x008BC128);

// Loads the module-wide context dword and hands it to the DDL dispatcher as its
// implicit `this` (ecx), as the original did when it loaded it into ecx.
static int __cdecl NetCogibleDdl_CallWithContext(int a1)
{
    (void)a1;
    void* context = reinterpret_cast<void*>(static_cast<uintptr_t>(g_NetCogibleDdlContext));
    return sub_64D6B0(context, nullptr);
}

HOOK(0x742060, NetCogibleDdl_CallWithContext, "auto");
