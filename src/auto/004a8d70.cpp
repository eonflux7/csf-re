#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t*, __fastcall, C_0083dd58_FindCompatibleCandidate, 0x004D0750, (C_0083dd58* self, void* , int selectionContext));

static std::uint32_t* __fastcall C_0083dd58_TryFindCandidate(C_0083dd58* self, void* /*edx*/) {
    if (!CSF_AT(std::uint8_t, self, 0x98)) {
        return nullptr;
    }

    C_00833908* loader = reinterpret_cast<C_00833908*>(self->field_0014);
    C_0082dd78* state = reinterpret_cast<C_0082dd78*>(loader->field_004c);

    if (loader->field_003c &&
        ((bool(__fastcall*)(void*, void*))self->vtbl[54])(self, nullptr) &&
        (state[1].field_0004 & 4) == 0) {
        return C_0083dd58_FindCompatibleCandidate(self, nullptr, 20);
    }
    return nullptr;
}

HOOK(0x4A8D70, C_0083dd58_TryFindCandidate, "auto");
