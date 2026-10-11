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

// Helper: reset the TCharTask base state (called first by the original).
CSF_FUNC(int, __fastcall, ResetAttackTaskBase, 0x0047B020, (void* self, void* edx));

// Setup routine at this+0x2b8, called last by the start sequence.
CSF_FUNC(unsigned int, __fastcall, ApplyAttackTaskParams, 0x0048C630, (void* self, void* edx, int a2, float a3, float a4, int a5, int a6));

// Scene/GUI accessors (bound as in the packet).
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __stdcall, GetSceneEntryIntAt0C, 0x004E9890, (uint32_t* a1));
CSF_FUNC(int, __stdcall, GetSceneObjectIntAt10, 0x004E9860, (uint32_t* a1));
CSF_FUNC(double, __stdcall, GetSceneObjectFloatAt0C, 0x004E9830, (uint32_t* a1));
CSF_FUNC(double, __stdcall, GetSceneObjectFloatAt08, 0x004E9800, (uint32_t* a1));
CSF_FUNC(double, __stdcall, GuiControl_QueryFloatValue, 0x004E98C0, (uint32_t* a1));

// Body of the original's tail-jump target (0x47BBF0), inlined here.
// Reads the scene values in the original's order and passes them to the setup at this+0x2b8.
static unsigned int __fastcall AttackTask_StartWithSceneValues(C_00831ac0* self, void* /*edx*/)
{
    uint32_t v1 = CSF_AT(uint32_t, self, 0x14);

    GetValue8B0554();
    int sceneEntryIntAt0C = GetSceneEntryIntAt0C(reinterpret_cast<uint32_t*>(v1));

    GetValue8B0554();
    int sceneObjectIntAt10 = GetSceneObjectIntAt10(reinterpret_cast<uint32_t*>(v1));

    GetValue8B0554();
    float sceneObjectFloatAt0C = static_cast<float>(GetSceneObjectFloatAt0C(reinterpret_cast<uint32_t*>(v1)));

    GetValue8B0554();
    float sceneObjectFloatAt08 = static_cast<float>(GetSceneObjectFloatAt08(reinterpret_cast<uint32_t*>(v1)));

    GetValue8B0554();
    float guiControlValue = static_cast<float>(GuiControl_QueryFloatValue(reinterpret_cast<uint32_t*>(v1)));
    int guiControlBits = 0;
    std::memcpy(&guiControlBits, &guiControlValue, sizeof guiControlBits);

    void* attackParams = reinterpret_cast<uint8_t*>(self) + 0x2b8;
    return ApplyAttackTaskParams(attackParams, nullptr,
                                 guiControlBits, sceneObjectFloatAt08, sceneObjectFloatAt0C,
                                 sceneObjectIntAt10, sceneEntryIntAt0C);
}

// Resets the attack task's state, then runs the scene-driven start sequence and returns its result.
static int __fastcall AttackTask_ResetAndStart(C_00831ac0* self, void* /*edx*/)
{
    ResetAttackTaskBase(self, nullptr);

    CSF_AT(uint8_t, self, 0x2a8) = 0;
    CSF_AT(int, self, 0x2ac) = 0;
    CSF_AT(int, self, 0x2b0) = 0;
    CSF_AT(int, self, 0x2e8) = 0;
    CSF_AT(uint8_t, self, 0x2ed) = 1;

    return static_cast<int>(AttackTask_StartWithSceneValues(self, nullptr));
}

HOOK(0x0047BEF0, AttackTask_ResetAndStart, "auto");
