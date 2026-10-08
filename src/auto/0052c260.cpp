#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base constructor of the embedded CMenuScreen sub-object at +0x04 (no stack args).
CSF_FUNC(void*, __fastcall, C_00830438_Construct, 0x00712CC0, (void* self, void* edx));

// Constructor of the sub-object embedded at +0x34; one stack argument (ret 4).
CSF_FUNC(void*, __fastcall, TextInputSubobject_Init, 0x00697130, (void* self, void* edx, uint8_t* arg));

CSF_GLOBAL(uint8_t, g_VtblCMenuScreenBase, 0x0083042C);
CSF_GLOBAL(uint8_t, g_VtblCMenuTextInputSecondary, 0x00841400);
CSF_GLOBAL(uint8_t, g_VtblCMenuTextInput, 0x00841450);
CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static uint32_t AddressOf(const void* p)
{
    return static_cast<uint32_t>(reinterpret_cast<uintptr_t>(p));
}

static CMenuTextInput* __fastcall CMenuTextInput_Construct(CMenuTextInput* self, void* /*edx*/, int ownerArg)
{
    // Sub-object at +0x04 (secondary base); the original passes it as ecx.
    void* secondary = reinterpret_cast<uint8_t*>(self) + 4;

    // Primary vtable of the CMenuScreen base (0x0083042C), stored at [this+0].
    CSF_AT(uint32_t, self, 0x00) = CSF_ADDR_VT(0x0083042C);
    (void)AddressOf(&g_VtblCMenuScreenBase);
    C_00830438_Construct(secondary, nullptr);

    // Final CMenuTextInput vtable (0x00841450) at [this+0], secondary vtable (0x00841400) at [this+4].
    CSF_AT(uint32_t, self, 0x00) = CSF_ADDR_VT(0x00841450);
    self->field_0004 = CSF_ADDR_DATA(0x00841400);
    (void)AddressOf(&g_VtblCMenuTextInput);
    (void)AddressOf(&g_VtblCMenuTextInputSecondary);

    // Sub-object at +0x34: fields written here are not declared, use CSF_AT.
    void* subobject = reinterpret_cast<uint8_t*>(self) + 0x34;
    CSF_AT(uint32_t, subobject, 0x18) = 7;
    CSF_AT(uint32_t, subobject, 0x14) = 0;
    CSF_AT(uint16_t, subobject, 0x04) = 0;

    self->field_0018 = static_cast<uint32_t>(ownerArg);
    self->field_0014 = 0;
    self->field_0054 = 0;
    self->field_0010 = 0;
    self->field_001c = 0;
    self->field_0050 = 0;

    TextInputSubobject_Init(subobject, nullptr, &g_byte_82E65C);

    self->field_0030 = 0;
    self->field_0058 = 0;
    return self;
}

HOOK(0x0052C260, CMenuTextInput_Construct, "auto");
