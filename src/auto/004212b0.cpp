#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callee at 0x004DB140, bound as given in the packet.
// __thiscall on the same `this`, one stack argument (ret 4).
CSF_FUNC(int, __fastcall, CharTask_ResolveSubobjectName, 0x004DB140, (void* self, void* edx, int a2));

// 0x4212B0: TFncGirar::GetName-style forwarder. Passes `this` through in ecx and
// hands the embedded sub-object at +0x3C to the callee as its single stack argument.
// The callee's return value is returned unchanged; the caller's stack arg (a2) is not used.
static int __fastcall TFncGirar_GetNameFromSubobject(TFncGirar* self, void* /*edx*/, int /*a2*/)
{
    int subobjectAddress = static_cast<int>(reinterpret_cast<uintptr_t>(reinterpret_cast<uint8_t*>(self) + 0x3C));
    return CharTask_ResolveSubobjectName(self, nullptr, subobjectAddress);
}

HOOK(0x4212B0, TFncGirar_GetNameFromSubobject, "auto");
