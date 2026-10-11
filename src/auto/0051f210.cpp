#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(unsigned int, __fastcall, sub_51C070, 0x0051C070, (void* self, void* edx));
static CSF_FUNC(uint32_t*, __fastcall, sub_51EFD0, 0x0051EFD0, (void* self, void* edx, int a2));
static CSF_FUNC(int, __cdecl, GetGlobalValue_753A30, 0x00753A30, ());

static void __fastcall C_00840f6c_UpdateElapsedTime(C_00840f6c* self, void* /*edx*/)
{
    if (self->field_0008 != UINT32_MAX)
    {
        sub_51C070(self, nullptr);
        const uint32_t currentTime =
            CSF_AT(uint32_t, GetGlobalValue_753A30(), 0x1F0);
        const uint32_t previousTime = self->field_0014;
        if (currentTime > previousTime)
        {
            sub_51EFD0(&self->field_0010, nullptr,
                       static_cast<int>(currentTime - previousTime));
        }
    }
}

HOOK(0x0051F210, C_00840f6c_UpdateElapsedTime, "auto");
