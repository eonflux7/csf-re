#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(char*, __fastcall, MissionScript_RegisterState, 0x00470E80,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, MissionScript_InitializeState, 0x0046F560,
         (void* self, void* edx));

static std::uint32_t* __fastcall C_00831414_Construct(
    C_00831414* self, void* /*edx*/, int context)
{
    self->vtbl = CSF_VTBL(0x00831414);
    self->field_0008 = -1;
    self->field_000c = -1;
    self->field_0004 = reinterpret_cast<std::uint32_t>(CSF_VTBL(0x0082D448));
    self->field_0010 = context;
    self->field_0020 = 0;
    self->field_0024 = reinterpret_cast<std::uint32_t>(CSF_VTBL(0x0082D478));
    self->field_002c = reinterpret_cast<std::uint32_t>(CSF_VTBL(0x0082D448));
    self->field_0030 = -1;
    self->field_0034 = -1;
    self->field_0038 = reinterpret_cast<std::uint32_t>(CSF_VTBL(0x0082D448));
    self->field_003c = -1;
    self->field_0040 = -1;
    self->field_0028 = -1;
    self->field_0044 = 0;
    self->field_0048 = 0;
    self->field_0050 = 0;
    self->field_004c = 0;
    self->field_0054 = 0;
    self->field_005c = reinterpret_cast<std::uint32_t>(CSF_VTBL(0x0082D448));
    self->field_0060 = -1;
    self->field_0064 = -1;
    self->field_0008 = -1;
    self->field_000c = -1;
    self->field_0080 = CSF_AT(
        std::uint8_t, reinterpret_cast<void*>(self->field_0010), 0x1);
    self->field_0068 = 0;
    self->field_0058 = 0;
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0014 = 0;
    self->field_0081 = 0;
    self->field_0083 = 0;
    self->field_0060 = -1;
    self->field_0064 = -1;
    self->field_0082 = 0;
    self->field_006c = -1;
    self->field_0070 = 0;
    self->field_0074 = -1;
    self->field_0078 = 0;
    self->field_007c = 0;

    void* state = Mem_Alloc(20);
    if (state)
    {
        CSF_AT(void**, state, 0x0) = CSF_VTBL(0x00831434);
        CSF_AT(std::uint32_t, state, 0x8) = 0;
        CSF_AT(std::uint32_t, state, 0xC) = 0;
        CSF_AT(std::uint32_t, state, 0x10) = 0;
    }

    self->field_0020 = reinterpret_cast<std::uint32_t>(state);
    MissionScript_RegisterState(
        CSF_AT(void*, reinterpret_cast<void*>(self->field_0010), 0x28),
        nullptr, reinterpret_cast<int>(state));
    MissionScript_InitializeState(
        reinterpret_cast<void*>(self->field_0020), nullptr);

    return reinterpret_cast<std::uint32_t*>(self);
}

HOOK(0x00467AD0, C_00831414_Construct, "auto");
