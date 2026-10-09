#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, S_0048f360_ResetCoreState, 0x00493120,
         (void* self, void* edx));
CSF_FUNC(double, __cdecl, GetInitialStateValue, 0x00493CF0, ());
CSF_FUNC(int, __fastcall, ResetOwnerState, 0x00499F00,
         (void* self, void* edx, char a2));
CSF_FUNC(char, __fastcall, ResetEmbeddedState, 0x004A1260,
         (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010,
         (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

static int __fastcall S_0048f360_ResetState(S_0048f360* self, void* /*edx*/)
{
    S_0048f360_ResetCoreState(self, nullptr);

    self->field_021d = 0;
    CSF_AT(float, self, 0x224) = g_ZeroVectorX;
    CSF_AT(float, self, 0x228) = g_ZeroVectorY;
    CSF_AT(float, self, 0x22C) = g_ZeroVectorZ;

    const bool hasSceneObject = self->field_0390 != 0;
    self->field_02d4 = 0;
    self->field_02d8 = 0;
    self->field_02dc = 0;
    self->field_0384 = 1065353216;
    self->field_038c = 0;

    if (hasSceneObject)
    {
        void* globals = reinterpret_cast<void*>(GetGlobalInt());
        void* sceneController = reinterpret_cast<void*>(
            SceneLoader_GetSceneController(globals, nullptr));
        reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t)>(
            CSF_AT(void**, sceneController, 0)[0x18 / 4])(
                sceneController, nullptr, self->field_0390);
        self->field_0390 = 0;
    }

    CSF_AT(uint32_t, self, 0x39C) = 0;
    CSF_AT(uint32_t, self, 0x3A0) = 0;
    self->field_0399 = 0;
    CSF_AT(uint32_t, self, 0x3A4) = 0;
    self->field_03c0 = -1;
    self->field_03c4 = 0;
    self->field_03c8 = 0;
    self->field_03cc = static_cast<float>(GetInitialStateValue());

    self->field_03e0 = 0;
    self->field_03d2 = 0;
    self->field_03d4 = 0;
    self->field_03d8 = 0;
    self->field_03f4 = 0;
    self->field_0200 = 0;
    self->field_0204 = 0;
    self->field_0208 = 0;
    self->field_020c = 0;
    self->field_0210 = 0;
    self->field_0214 = 0;
    self->field_0218 = 0;
    self->field_02e4 = self->field_02e0;
    self->field_02e8 = 0;
    self->field_02ec = 0;
    self->field_033c = 0;
    self->field_0340 = 0;

    void* owner = reinterpret_cast<uint8_t*>(self) - 12;
    reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int, int)>(
        CSF_AT(void**, owner, 0)[0x8C / 4])(
            owner, nullptr, 0, 0, -1, 0);

    ResetEmbeddedState(reinterpret_cast<uint8_t*>(self) + 0x338, nullptr, 0);

    self->field_0238 = -1;
    self->field_023c = -1;
    self->field_0244 = -1;
    self->field_0248 = -1;
    self->field_024c = 0;
    CSF_AT(float, self, 0x250) = g_ZeroVectorX;
    CSF_AT(float, self, 0x254) = g_ZeroVectorY;
    CSF_AT(float, self, 0x258) = g_ZeroVectorZ;
    self->field_025c = 0;
    self->field_0260 = 0;
    self->field_0268 = -1;
    self->field_026c = -1;
    self->field_0270 = -1;
    CSF_AT(float, self, 0x274) = g_ZeroVectorX;
    CSF_AT(float, self, 0x278) = g_ZeroVectorY;
    CSF_AT(float, self, 0x27C) = g_ZeroVectorZ;
    self->field_0284 = -1;
    self->field_0288 = -1;
    CSF_AT(float, self, 0x1BC) = g_ZeroVectorX;
    CSF_AT(float, self, 0x1C0) = g_ZeroVectorY;
    CSF_AT(float, self, 0x1C4) = g_ZeroVectorZ;

    ResetOwnerState(owner, nullptr, 0);

    self->field_03f8 = 0;
    self->field_03fc = 0;
    self->field_0400 = 0;
    CSF_AT(float, self, 0x404) = g_ZeroVectorX;
    const float zeroY = g_ZeroVectorY;
    CSF_AT(float, self, 0x408) = zeroY;
    CSF_AT(float, self, 0x40C) = g_ZeroVectorZ;

    int result;
    std::memcpy(&result, &zeroY, sizeof(result));
    return result;
}

HOOK(0x0049ABE0, S_0048f360_ResetState, "auto");
