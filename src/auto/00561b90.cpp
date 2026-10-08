#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static void** const g_MenuScreenVtable =
    reinterpret_cast<void**>(0x0083042C);
static void** const g_MpSessionsVtable =
    reinterpret_cast<void**>(0x00845990);
static void** const g_MpSessionsSecondaryVtable =
    reinterpret_cast<void**>(0x00845940);

CSF_FUNC(std::uint32_t*, __fastcall, MenuMpSessions_ConstructEmbeddedBase,
         0x00712CC0, (std::uint32_t* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuMpSessions_InitializeState,
         0x00560140, (CMenuMpSessions* self, void* edx));

static int __fastcall CMenuMpSessions_Construct(
    CMenuMpSessions* self, void* /*edx*/, int context)
{
    CSF_AT(void**, self, 0x00) = g_MenuScreenVtable;

    MenuMpSessions_ConstructEmbeddedBase(
        reinterpret_cast<std::uint32_t*>(
            reinterpret_cast<std::uint8_t*>(self) + 4),
        nullptr);

    self->field_0048 = static_cast<std::uint32_t>(context);
    self->field_01e0 = 5;
    self->field_0020 = 5;

    CSF_AT(void**, self, 0x00) = g_MpSessionsVtable;
    self->field_0004 =
        reinterpret_cast<std::uint32_t>(g_MpSessionsSecondaryVtable);

    self->field_0040 = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_00ac = 0;
    self->field_00a8 = 0;
    self->field_0094 = 0;
    self->field_0098 = 0;
    self->field_009c = 0;
    self->field_00a0 = 0;
    self->field_0078 = 0;
    self->field_004c = 0;
    self->field_0050 = 0;
    self->field_0054 = 0;
    self->field_0058 = 0;
    self->field_005c = 0;
    self->field_0060 = 0;
    self->field_0064 = 0;
    self->field_0068 = 0;
    self->field_006c = 0;
    self->field_0070 = 0;
    self->field_0034 = 0;

    CMenuMpSessions_InitializeState(self, nullptr);
    return reinterpret_cast<int>(self);
}

HOOK(0x00561B90, CMenuMpSessions_Construct, "auto");
