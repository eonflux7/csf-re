#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(uint8_t, g_StringConstant, 0x0082E65C);

CSF_FUNC(void*, __fastcall, CMenuScreen_ConstructSubobject, 0x00712CC0,
         (void* self, void* edx));

CSF_FUNC(void*, __fastcall, MenuSceneSelect_InitializeString, 0x00429B60,
         (void* self, int argument, char* text, unsigned int length));

static CMenuSceneSelect* __fastcall CMenuSceneSelect_Construct(
    CMenuSceneSelect* self, void* /*edx*/, int argument)
{
    CSF_AT(void**, self, 0) =
        CSF_VTBL(0x0083042C);
    CMenuScreen_ConstructSubobject(
        reinterpret_cast<uint8_t*>(self) + 4, nullptr);

    CSF_AT(void**, self, 0) =
        CSF_VTBL(0x00841978);
    self->field_0004 =
        static_cast<uint32_t>(CSF_ADDR_VT(0x00841928));

    self->field_0030 = 15;
    self->field_002c = 0;
    *reinterpret_cast<uint8_t*>(&self->field_001c) = 0;
    self->field_003c = 0;
    self->field_0040 = 0;
    self->field_0044 = 0;
    self->field_0048 = static_cast<uint32_t>(argument);
    self->field_0014 = 0;
    self->field_0034 = 0;
    self->field_004c = 0;
    self->field_0050 = 0;

    MenuSceneSelect_InitializeString(
        reinterpret_cast<uint8_t*>(self) + 0x18,
        argument, reinterpret_cast<char*>(&g_StringConstant), 0);

    ((void(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[16])(
        self, nullptr);

    return self;
}

HOOK(0x0052FDA0, CMenuSceneSelect_Construct, "auto");
