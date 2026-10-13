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

CSF_FUNC(int, __fastcall, C_0082f9f8_Start, 0x004226C0, (C_0082f9f8* self, void* , float a2, char a3));

static int __fastcall AiGiveObject_StartOwnerTaskWithSpeed(void* self, void* /*edx*/)
{
    void* obj = CSF_AT(void*, self, 0x08);
    float speed = CSF_AT(float, obj, 0x3E8);
    void* owner = CSF_AT(void*, obj, 0x14);
    C_0082f9f8* target = CSF_AT(C_0082f9f8*, owner, 0x30);
    return C_0082f9f8_Start(target, nullptr, speed, 1);
}

HOOK(0x004D4D60, AiGiveObject_StartOwnerTaskWithSpeed, "auto");
