#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, Water_ResetParamsBlock, 0x0070CC90, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Water_InitSubobjectAt_DC, 0x00709AE0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, Water_FinalizeState, 0x006FE010, (void* self, void* edx));

static char __fastcall Water_InitializeShaderState(S_006fdfe0* self, void* /*edx*/)
{
    self->field_0000 = reinterpret_cast<uint32_t>(CSF_VTBL(0x86350C));
    Water_ResetParamsBlock(self, nullptr);
    Water_InitSubobjectAt_DC(reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(self) + 0xDC), nullptr);
    return Water_FinalizeState(self, nullptr);
}

HOOK(0x0070A1C0, Water_InitializeShaderState, "auto");
