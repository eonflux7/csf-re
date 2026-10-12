#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(void, __fastcall, UpdateControllerPrimary, 0x004480F0,
               (void* self, void* edx, float a2));
static CSF_FUNC(uint64_t, __fastcall, UpdateControllerSecondary, 0x004482E0,
               (void* self, void* edx, float a2));
static CSF_FUNC(void, __fastcall, UpdateControllerFinalStage, 0x004486E0,
               (void* self, void* edx, float a2));

static void __fastcall S_004480f0_UpdateFinalController(
    S_004480f0* self, void* /*edx*/, float deltaTime)
{
    UpdateControllerPrimary(self, nullptr, deltaTime);
    UpdateControllerSecondary(self, nullptr, deltaTime);
    UpdateControllerFinalStage(self, nullptr, deltaTime);
}

HOOK(0x00448DA0, S_004480f0_UpdateFinalController, "auto");
