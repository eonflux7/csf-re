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

// Bound by address: callees of Scene_ResetOptionalGlobal.
CSF_FUNC(int, __fastcall, ReleaseEmbeddedMember, 0x0044AFF0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Resets the optional global held by the scene: releases the embedded member at +0x04,
// frees the block held at +0x18 (if any), and zeroes +0x18, +0x1C and +0x20.
static int __fastcall Scene_ResetOptionalGlobal(int* self, void* /*edx*/)
{
    // Sub-object embedded at +4 (lea ecx, [esi+4]); not a pointer load.
    ReleaseEmbeddedMember(reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(self) + 4), nullptr);

    int result = CSF_AT(int, self, 0x18);
    if (result)
        result = Mem_FreeDebug(reinterpret_cast<void*>(static_cast<uintptr_t>(result)));

    CSF_AT(int, self, 0x18) = 0;
    CSF_AT(int, self, 0x1C) = 0;
    CSF_AT(int, self, 0x20) = 0;
    return result;
}

HOOK(0x44B230, Scene_ResetOptionalGlobal, "auto");
