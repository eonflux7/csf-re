#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, C_0083e9d0_ResetStateFields, 0x004E5760, (C_0083e9d0* self, void*));
CSF_FUNC(int, __fastcall, sub_44E360, 0x0044E360, (void* self, void* edx, uint32_t* a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Resets the base state, then clears the four sub-objects' entries in the
// current scene's child-manager and zeroes the four trailing state fields.
static int __fastcall C_0083eb70_ResetStateAndClearFields(C_0083eb70* self, void* /*edx*/)
{
    C_0083e9d0_ResetStateFields(self, nullptr);

    // g_pCurrentScene -> +0xB0 -> +0x114 : manager object used for all four calls
    void* sceneSubobject = CSF_AT(void*, g_pCurrentScene, 0xB0);
    void* manager = CSF_AT(void*, sceneSubobject, 0x114);

    sub_44E360(manager, nullptr, &self->field_0050);
    sub_44E360(manager, nullptr, &self->field_0060);
    sub_44E360(manager, nullptr, &self->field_0070);
    sub_44E360(manager, nullptr, &self->field_0080);

    self->field_0058 = 0;
    self->field_0068 = 0;
    self->field_0078 = 0;
    self->field_0088 = 0;
    return 0;
}

HOOK(0x004E7390, C_0083eb70_ResetStateAndClearFields, "auto");
