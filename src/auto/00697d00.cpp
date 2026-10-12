#include <cstdint>
#include "globals.h"
#include "hook.h"

static CSF_FUNC(int, __cdecl, CoreFiles_ProcessStringPair, 0x00697B10,
               (void* a1, void* a2, int a3, int a4, int a5));

static int __fastcall CoreFiles_ForwardStringPair(
    void* self, void* /*edx*/, void* first, void* second, int parameter)
{
    return CoreFiles_ProcessStringPair(
        first, second, parameter,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(self)), parameter);
}

HOOK(0x00697D00, CoreFiles_ForwardStringPair, "auto");
