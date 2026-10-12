#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, GfxScene_Update, 0x006CB500, (void* self, void* , float deltaTime));
CSF_FUNC(void, __fastcall, sub_6D22D0, 0x006D22D0, (void* self, void* edx, float a2));
CSF_FUNC(unsigned int*, __fastcall, sub_6DFE00, 0x006DFE00, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, sub_6D6CD0, 0x006D6CD0, (void* self, void* edx, float a2));

// The original forwards the raw dword of its stack argument to the float-taking callees.
static float FrameArgBitsToFloat(uint32_t bits)
{
    float value;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

// ecx = this, edx unused, one stack argument (ret 4).
static char __fastcall Interface_UpdateGlobalFrameTime(S_006d6800* self, void* /*edx*/, uint32_t frameArgBits)
{
    float frameDeltaTime = FrameArgBitsToFloat(frameArgBits);

    GfxScene_Update(reinterpret_cast<void*>(self->field_005c), nullptr, frameDeltaTime);
    sub_6D22D0(CSF_AT(void*, self, 0x64), nullptr, frameDeltaTime);
    sub_6DFE00(reinterpret_cast<void*>(self->field_0054), nullptr, static_cast<int>(frameArgBits));
    return sub_6D6CD0(CSF_AT(void*, self, 0x68), nullptr, frameDeltaTime);
}

HOOK(0x006B13C0, Interface_UpdateGlobalFrameTime, "auto");
