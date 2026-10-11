#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, UpdateParticleState, 0x0051EB20,
         (void* self, void* edx, float a2));
CSF_FUNC(uint32_t*, __fastcall, PrepareParticleUpdate, 0x0051F240,
         (void* self, void* edx, int a2));

static int __fastcall C_00840f6c_UpdateIfActive(
    C_00840f6c* self, void* /*edx*/, int argument)
{
    const int active = self->vSlot14();
    if (static_cast<char>(active))
    {
        PrepareParticleUpdate(self, nullptr, argument);

        float updateArgument;
        std::memcpy(&updateArgument, &argument, sizeof(updateArgument));
        return UpdateParticleState(self, nullptr, updateArgument);
    }
    return active;
}

HOOK(0x0051F610, C_00840f6c_UpdateIfActive, "auto");
