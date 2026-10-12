#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(float*, __fastcall, C_0083f1d8_Initialize, 0x004F91E0, (C_0083f1d8* self, void*));
CSF_FUNC(uint32_t*, __fastcall, C_00831074_Initialize, 0x00442F20, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_00831074_Reset, 0x00443310, (void* self, void* edx));

CSF_GLOBAL(const char*, g_pszWeap_d_franco, 0x00899900);
CSF_GLOBAL(const char*, g_pszWeap_d_prism, 0x00899904);
CSF_GLOBAL(const char*, g_pszWeap_d_agua, 0x00899908);

static char** __fastcall C_0083f448_Initialize(C_0083f448* self, void* /*edx*/)
{
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0083F448);

    self->field_0010 = 10;
    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0020 = 10;
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0030 = 10;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;

    CSF_AT(uint32_t, self, 0x90) = 0;
    self->field_0094 = 0;
    self->field_0098 = 0;
    CSF_AT(uint32_t, self, 0x9C) = 0;
    self->field_00a0 = 0;
    self->field_00a4 = 0;
    self->field_00a8 = 0;
    CSF_AT(uint32_t, self, 0xAC) = 0;
    self->field_00b0 = 0;
    self->field_00b4 = 0;
    self->field_00b8 = 0;
    self->field_00bc = 0;
    self->field_00c4 = 0;
    self->field_00c8 = 0;
    self->field_00cc = 0;

    self->field_0058 = 0;
    self->field_005c = 0;
    self->field_006c = 0;

    auto* animationStorage = static_cast<C_0083f1d8*>(Mem_Alloc(48));
    float* animationState = nullptr;
    if (animationStorage)
        animationState = C_0083f1d8_Initialize(animationStorage, nullptr);
    self->field_0074 = reinterpret_cast<uint32_t>(animationState);

    void* auxiliaryStorage = Mem_Alloc(28);
    uint32_t* auxiliaryState = nullptr;
    if (auxiliaryStorage)
        auxiliaryState = C_00831074_Initialize(auxiliaryStorage, nullptr);
    self->field_0078 = reinterpret_cast<uint32_t>(auxiliaryState);
    C_00831074_Reset(auxiliaryState, nullptr);

    self->field_007c = 0;
    self->field_0080 = 0;
    self->field_0084 = 0;
    self->field_0088 = 0;
    self->field_0034 = 0;
    self->field_002c = 0;
    self->field_008c = -1;
    self->field_0060 = 0;
    self->field_0064 = 0;
    self->field_0068 = 0;

    CSF_AT(uint32_t, self, 0x90) = reinterpret_cast<uint32_t>(g_pszWeap_d_franco);
    self->field_00a0 = reinterpret_cast<uint32_t>(g_pszWeap_d_prism);
    self->field_00b0 = reinterpret_cast<uint32_t>(g_pszWeap_d_agua);
    self->field_0098 = 150;
    self->field_00b8 = 150;
    self->field_0094 = 200;
    self->field_00a4 = 240;
    self->field_00a8 = 180;
    self->field_00b4 = 200;
    self->field_00bc = 100;

    return reinterpret_cast<char**>(self);
}

HOOK(0x004FD4A0, C_0083f448_Initialize, "auto");
