#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char*, __fastcall, NetSession_InitializeContainerStorage, 0x006684F0, (void* self, void* edx));
CSF_FUNC(void**, __fastcall, NetSession_InitializeEmbeddedState, 0x00670020, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, NetSession_ResetSessionState, 0x0073C380, (void* self, void* edx));
CSF_FUNC(bool, __cdecl, NetSession_InitializeRuntime, 0x0074A910, ());

static int __fastcall S_0073c380_Construct(S_0073c380* self, void* /*edx*/)
{
    self->field_0024 = 7;
    self->field_0020 = 0;
    self->field_0010 = 0;
    self->field_0040 = 7;
    self->field_003c = 0;
    self->field_002c = 0;
    self->field_005c = 7;
    self->field_0058 = 0;
    self->field_0048 = 0;
    self->field_0078 = 7;
    self->field_0074 = 0;
    self->field_0064 = 0;
    self->field_0094 = 7;
    self->field_0090 = 0;
    self->field_0080 = 0;

    auto* bytes = reinterpret_cast<uint8_t*>(self);
    CSF_AT(uint32_t, self, 0xA0) = reinterpret_cast<uint32_t>(
        NetSession_InitializeContainerStorage(bytes + 0x9C, nullptr));
    CSF_AT(uint32_t, self, 0xA4) = 0;
    CSF_AT(uint32_t, self, 0xB0) = reinterpret_cast<uint32_t>(
        NetSession_InitializeContainerStorage(bytes + 0xAC, nullptr));
    CSF_AT(uint32_t, self, 0xB4) = 0;

    self->field_00f8 = 15;
    self->field_00f4 = 0;
    self->field_00e4 = 0;
    NetSession_InitializeEmbeddedState(bytes + 0x160, nullptr);

    // The packet lists this call target as a vtable; no vtable store occurs.
    (void)CSF_VTBL(0x0073C380);
    NetSession_ResetSessionState(self, nullptr);
    NetSession_InitializeRuntime();
    return reinterpret_cast<int>(self);
}

HOOK(0x0073F080, S_0073c380_Construct, "auto");
