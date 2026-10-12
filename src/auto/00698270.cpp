#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(char, __cdecl, MissionScript_DestroyStringRange, 0x00697A80, (int a1, int a2));

static std::uint32_t* __fastcall S_00698270_Initialize(
    S_00698270* self, void* /*edx*/, char initialMode)
{
    self->field_001c = 15;
    self->field_0018 = 0;
    self->field_0008 = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_002c = 10;
    self->field_0034 = 0;
    self->field_0038 = 0;
    self->field_003c = 0;
    self->field_0054 = 0;
    self->field_0058 = 0;
    self->field_005c = 0;

    self->field_0000 = initialMode;
    self->field_0002 = 0;
    self->field_0001 = 0;
    self->field_0040 = 1;
    self->field_0041 = 0;
    self->field_0044 = 0;
    self->field_0048 = 0;
    self->field_004c = 0;
    self->field_0060 = 0;
    self->field_0064 = 0;
    self->field_0068 = 0;

    if (self->field_0054)
    {
        MissionScript_DestroyStringRange(
            static_cast<int>(self->field_0054),
            static_cast<int>(self->field_0058));

        if (self->field_0054)
            Mem_FreeDebug(reinterpret_cast<void*>(self->field_0054));
    }

    self->field_0054 = 0;
    self->field_0058 = 0;
    self->field_005c = 0;
    return reinterpret_cast<std::uint32_t*>(self);
}

HOOK(0x00698270, S_00698270_Initialize, "auto");
