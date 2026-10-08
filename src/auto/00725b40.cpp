#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"

CSF_FUNC(int, __fastcall, SubmitObjectState, 0x00731A60,
         (void* self, void* edx, int index, void* state));
CSF_FUNC(int, __fastcall, CommitObjectState, 0x00731A80,
         (void* self, void* edx, int index));

static int __fastcall SubmitAndCommitObjectState(void* self, void* /*edx*/)
{
    alignas(4) std::uint8_t state[0x44];

    const std::uint32_t value1DC = CSF_AT(std::uint32_t, self, 0x1DC);
    const std::uint32_t value1D8 = CSF_AT(std::uint32_t, self, 0x1D8);
    CSF_AT(std::uint32_t, state, 0x04) = value1DC;

    void* receiver = CSF_AT(void*, self, 0x1D4);
    CSF_AT(std::uint32_t, state, 0x20) = value1DC;
    const std::uint32_t value118 = CSF_AT(std::uint32_t, self, 0x118);

    CSF_AT(std::uint32_t, state, 0x24) = 0;
    CSF_AT(std::uint32_t, state, 0x28) = 0;
    CSF_AT(std::uint32_t, state, 0x2C) = 0;
    CSF_AT(std::uint32_t, state, 0x30) = 0x3F800000;
    CSF_AT(std::uint32_t, state, 0x34) = 0;
    CSF_AT(std::uint32_t, state, 0x38) = 0;
    CSF_AT(std::uint32_t, state, 0x3C) = 0;
    CSF_AT(std::uint32_t, state, 0x40) = 0x3F800000;
    CSF_AT(std::uint32_t, state, 0x1C) = 0;
    CSF_AT(std::uint8_t, state, 0x00) = 0;
    CSF_AT(std::uint8_t, state, 0x10) = 0;
    CSF_AT(std::uint8_t, state, 0x11) = 0;
    CSF_AT(std::uint32_t, state, 0x0C) = 0;
    CSF_AT(std::uint32_t, state, 0x14) = 0;
    CSF_AT(std::uint32_t, state, 0x08) = value1D8;
    CSF_AT(std::uint32_t, state, 0x18) = value118;

    SubmitObjectState(receiver, nullptr, 0, state);
    return CommitObjectState(CSF_AT(void*, self, 0x1D4), nullptr, 0);
}

HOOK(0x00725B40, SubmitAndCommitObjectState, "auto");
