#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(char, __fastcall, TokenizerBuffer_Refresh, 0x0069E640, (void* self, void* edx));

static int __fastcall S_0069e480_SetBuffer(S_0069e480* self, void* /*edx*/,
                                           void* a2, void* a3, int /*a4*/) {
    self->field_0008 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(a2));
    self->field_000c = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(a3));

    if (a3 == nullptr) {
        self->field_0014 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(
            CSF_STR(0x0085EB90, "No se pueden procesar textos de tama")));
        self->field_0010 = 1;
    }

    if (self->field_0010 == 0) {
        self->field_001c = 0;
        TokenizerBuffer_Refresh(self, nullptr);

        if (self->field_0010 == 0) {
            self->field_0000 = static_cast<uint32_t>(
                CoreMemory_AllocUntagged(static_cast<std::size_t>(self->field_0004 << 3)));
            self->field_0004 = 0;
            self->field_001c = 1;
            TokenizerBuffer_Refresh(self, nullptr);
        }
    }

    return self->field_0010 == 0 ? 1 : 0;
}

HOOK(0x0069E740, S_0069e480_SetBuffer, "auto");
