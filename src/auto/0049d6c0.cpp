#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));
CSF_FUNC(int, __fastcall, S_0048b280_SubmitEffectFromParameters, 0x0048B560, (S_0048b280* self, void* , float value, float* parameters, float* position));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_ConfigureFocusEntry, 0x0048E460, (void* self, void* edx, int* a2));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, FxParticleManager_FindSceneEntryAtPosition, 0x004EF6E0, (void* self, void* edx, void* a2, char a3));

CSF_GLOBAL(float, g_MessageBoxAnimationScale, 0x0082D7C0);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static float FloatFromBits(uint32_t bits)
{
    float value;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

static int __fastcall C_008342e8_SubmitEffectAndDispatchRecord(
    C_008342e8* self, void* /*edx*/)
{
    alignas(8) uint8_t recordBytes[0x6C]{};
    alignas(8) uint8_t positionBytes[0x10]{};
    float* position = reinterpret_cast<float*>(positionBytes);

    void* gameState = reinterpret_cast<void*>(GetValue8B0554());
    void* effectTable = CSF_AT(void*, gameState, 0x1FC0);
    void* entry = ((void* (__fastcall*)(void*, void*, uint32_t))
        CSF_AT(void**, effectTable, 0)[8 / 4])(
            effectTable, nullptr, CSF_AT(uint32_t, self, 0x194));
    uint8_t* parameters = static_cast<uint8_t*>(entry) + 4;

    CSF_AT(float, recordBytes, 0x60) =
        FloatFromBits(self->field_019c) * g_MessageBoxAnimationScale;
    CSF_AT(float, recordBytes, 0x64) =
        FloatFromBits(self->field_01a0) * g_MessageBoxAnimationScale;
    CSF_AT(float, recordBytes, 0x68) =
        FloatFromBits(self->field_01a4) * g_MessageBoxAnimationScale;

    float* objectPosition = GetObjectPositionVector(self, nullptr);
    position[2] = CSF_AT(float, recordBytes, 0x68) + objectPosition[2];
    position[1] = CSF_AT(float, recordBytes, 0x64) + objectPosition[1];
    position[0] = CSF_AT(float, recordBytes, 0x60) + objectPosition[0];

    void* owner = reinterpret_cast<void*>(self->field_004c);
    if (CSF_AT(uint8_t, owner, 0x75) == 0)
    {
        void* focus = reinterpret_cast<void*>(CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x188)));
        if (focus)
        {
            float* focusPosition = GetObjectPositionVector(focus, nullptr);
            SceneLoader_ConfigureFocusEntry(
                self, nullptr, reinterpret_cast<int*>(focusPosition));
            focusPosition = GetObjectPositionVector(focus, nullptr);
            position[0] = focusPosition[0];
            position[1] = focusPosition[1];
            position[2] = focusPosition[2];
        }
    }

    S_0048b280_SubmitEffectFromParameters(
        CSF_AT(S_0048b280*, g_pCurrentScene, 0xA8), nullptr,
        FloatFromBits(reinterpret_cast<uint32_t>(self)),
        reinterpret_cast<float*>(parameters), position);

    gameState = reinterpret_cast<void*>(GetValue8B0554());
    void* particleManager = CSF_AT(void*, gameState, 0x1FDC);
    objectPosition = GetObjectPositionVector(self, nullptr);
    int sceneEntry = FxParticleManager_FindSceneEntryAtPosition(
        particleManager, nullptr, objectPosition, 1);

    CSF_AT(int, recordBytes, 0x0C) = -1;
    CSF_AT(int, recordBytes, 0x10) = -1;
    CSF_AT(int, recordBytes, 0x18) = -1;
    CSF_AT(int, recordBytes, 0x1C) = -1;
    uint32_t ownerHandle = self->field_0050;
    CSF_AT(void**, recordBytes, 0x08) = CSF_VTBL(0x0082D448);
    CSF_AT(void**, recordBytes, 0x14) = CSF_VTBL(0x0082D448);
    CSF_AT(float, recordBytes, 0x34) = position[0];
    CSF_AT(uint32_t, recordBytes, 0x24) = ownerHandle;
    CSF_AT(float, recordBytes, 0x3C) = position[2];
    uint32_t directionX = self->field_019c;
    CSF_AT(uint32_t, recordBytes, 0x28) = 0;
    CSF_AT(uint32_t, recordBytes, 0x48) = 0;
    CSF_AT(uint32_t, recordBytes, 0x4C) = 0;
    CSF_AT(uint32_t, recordBytes, 0x50) = 0;
    CSF_AT(uint32_t, recordBytes, 0x54) = 0;
    CSF_AT(uint32_t, recordBytes, 0x58) = 0;
    CSF_AT(uint8_t, recordBytes, 0x5C) = 0;
    CSF_AT(float, recordBytes, 0x38) = position[1];
    CSF_AT(int, recordBytes, 0x04) = 14;
    CSF_AT(uint32_t, recordBytes, 0x20) = 0;
    CSF_AT(uint32_t, recordBytes, 0x2C) = 0;
    CSF_AT(uint8_t, recordBytes, 0x30) = 0;
    CSF_AT(void**, recordBytes, 0x00) = CSF_VTBL(0x008310F8);
    CSF_AT(uint32_t, recordBytes, 0x28) = CSF_AT(uint32_t, parameters, 0x1A0);
    uint32_t directionY = self->field_01a0;
    CSF_AT(uint32_t, recordBytes, 0x48) = directionX;
    uint32_t directionZ = self->field_01a4;
    CSF_AT(C_008342e8*, recordBytes, 0x40) = self;
    CSF_AT(uint32_t, recordBytes, 0x44) = 0;
    CSF_AT(uint32_t, recordBytes, 0x4C) = directionY;
    CSF_AT(uint32_t, recordBytes, 0x50) = directionZ;
    CSF_AT(uint32_t, recordBytes, 0x54) = CSF_AT(uint32_t, parameters, 0x120);
    uint32_t effectFlags = CSF_AT(uint32_t, parameters, 0x114);
    CSF_AT(uint8_t, recordBytes, 0x5C) = sceneEntry != -1;
    CSF_AT(uint32_t, recordBytes, 0x58) = effectFlags;

    void* dispatcher = reinterpret_cast<void*>(GetGlobalObjectField184());
    DispatchRecordToGlobalObject(dispatcher, nullptr, recordBytes, 1);

    int result = ((int (__fastcall*)(C_008342e8*, void*))
        self->vtbl[0xA0 / 4])(self, nullptr);
    CSF_AT(uint8_t, self, 0x184) = 1;
    return result;
}

HOOK(0x0049D6C0, C_008342e8_SubmitEffectAndDispatchRecord, "auto");
