#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Vtable addresses written by the constructor. The base CMenuScreen vtable is
// installed first, then the embedded sub-object vtable, then the final
// CMenuMpServerInfo vtable.
CSF_GLOBAL(uint8_t, g_VtblCMenuScreenEmbedded, 0x00841A98);

// Initializes the embedded CMenuScreen sub-object at this+0x004 (__thiscall, no stack args).
CSF_FUNC(void*, __fastcall, C_00830438_Construct, 0x00712CC0, (void* self, void* edx));

// Constructor of CMenuMpServerInfo (__thiscall, one stack argument, ret 4).
static CMenuMpServerInfo* __fastcall CMenuMpServerInfo_Construct(CMenuMpServerInfo* self,
                                                                  void* /*edx*/,
                                                                  int ownerHandle) {
    // Install the CMenuScreen base vtable at offset 0 (value 0x0083042C).
    CSF_AT(uint32_t, self, 0x000) = CSF_ADDR_VT(0x0083042C);
    CSF_AT(uint32_t, self, 0x000) = reinterpret_cast<uint32_t>(CSF_VTBL(0x0083042C));

    // Initialize the embedded sub-object at this+0x004 (passed as ecx).
    C_00830438_Construct(reinterpret_cast<uint8_t*>(self) + 0x004, nullptr);

    self->field_01f0 = static_cast<uint32_t>(ownerHandle);

    // Sub-object vtable slot (value 0x00841A98).
    self->field_0004 = CSF_ADDR_DATA(0x00841A98);
    self->field_0004 = reinterpret_cast<uint32_t>(&g_VtblCMenuScreenEmbedded);

    self->field_01ec = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0034 = 0;
    self->field_0040 = 0;
    self->field_0044 = 0;
    self->field_0048 = 0;
    self->field_0050 = 0;
    self->field_004c = 0.0f;
    self->field_01e8 = 0;

    // Final (most-derived) vtable at offset 0 (value 0x00841AE8).
    CSF_AT(uint32_t, self, 0x000) = CSF_ADDR_VT(0x00841AE8);
    CSF_AT(uint32_t, self, 0x000) = reinterpret_cast<uint32_t>(CSF_VTBL(0x00841AE8));

    return self;
}

HOOK(0x005309E0, CMenuMpServerInfo_Construct, "auto");
