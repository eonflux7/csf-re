#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, C_008310f8_Initialize, 0x00446550, (C_008310f8* self, void* , int owner));
CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));
CSF_FUNC(int, __fastcall, SubmitSceneEffect, 0x0048B280, (void* self, void* edx, float* arg0));
CSF_FUNC(uint32_t*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(int64_t, __cdecl, MpSpawn_QueryAndCommitSpawn, 0x005516E0, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_558150, 0x00558150, ());

CSF_GLOBAL(float, g_AxisY, 0x008A7D78);
CSF_GLOBAL(float, g_AxisY_Y, 0x008A7D7C);
CSF_GLOBAL(float, g_AxisY_Z, 0x008A7D80);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static char __fastcall SceneEffects_SubmitExpiredObjectEffects(void* self, void* /*edx*/)
{
    void* owner = CSF_AT(void*, self, 0x14);
    if (!owner)
        return 0;

    const uintptr_t attachment = CSF_AT(uintptr_t, owner, 0x18);
    if (!attachment)
        return 0;

    void* timedObject = reinterpret_cast<void*>(attachment - 128);
    if (!timedObject)
        return static_cast<char>(attachment);

    const uint32_t deadlineLow = CSF_AT(uint32_t, timedObject, 0x68);
    const uint32_t deadlineHigh = CSF_AT(uint32_t, timedObject, 0x6C);
    if ((deadlineHigh & deadlineLow) == 0xFFFFFFFF)
        return static_cast<char>(deadlineLow);

    GetGlobal8B0688();
    const uint64_t currentTime =
        static_cast<uint64_t>(MpSpawn_QueryAndCommitSpawn());
    const uint64_t deadline =
        (static_cast<uint64_t>(deadlineHigh) << 32) | deadlineLow;

    if (currentTime <= deadline)
        return static_cast<char>(currentTime);

    const char alreadySubmitted = CSF_AT(char, self, 0x58);
    if (alreadySubmitted)
        return alreadySubmitted;

    alignas(8) uint8_t effectBytes[0x28] = {};
    owner = CSF_AT(void*, self, 0x14);
    CSF_AT(uint32_t, effectBytes, 0x04) = 0;
    CSF_AT(uint32_t, effectBytes, 0x08) = 0;
    CSF_AT(uint32_t, effectBytes, 0x0C) = 0;
    CSF_AT(float, effectBytes, 0x00) = 0.0f;

    uint32_t* position = GetObjectPositionVector(owner, nullptr);
    CSF_AT(uint32_t, effectBytes, 0x04) = position[0];
    CSF_AT(uint32_t, effectBytes, 0x08) = position[1];
    CSF_AT(uint32_t, effectBytes, 0x0C) = position[2];

    CSF_AT(uint32_t, effectBytes, 0x20) =
        CSF_AT(uint32_t, reinterpret_cast<void*>(GetGlobalValue_558150()), 0x1C8);
    CSF_AT(int, effectBytes, 0x14) = -1;
    CSF_AT(uint32_t, effectBytes, 0x18) =
        CSF_AT(uint32_t, reinterpret_cast<void*>(GetGlobalValue_558150()), 0x1C4);
    CSF_AT(uint32_t, effectBytes, 0x1C) = 0;

    // Preserve the packet's binding for this referenced address.
    (void)CSF_VTBL(0x0048B280);
    SubmitSceneEffect(CSF_AT(void*, g_pCurrentScene, 0xA8), nullptr,
                      reinterpret_cast<float*>(effectBytes));

    alignas(8) uint8_t recordBytes[0x68] = {};
    C_008310f8& record = *reinterpret_cast<C_008310f8*>(recordBytes);
    C_008310f8_Initialize(&record, nullptr, 15);

    owner = CSF_AT(void*, self, 0x14);
    record.field_0024 = -1;
    position = GetObjectPositionVector(owner, nullptr);
    record.field_0034 = position[0];
    record.field_0038 = position[1];
    record.field_003c = position[2];
    record.field_0028 = 0;
    record.field_0040 = 0;
    std::memcpy(&record.field_0048, &g_AxisY, sizeof(float));
    std::memcpy(&record.field_004c, &g_AxisY_Y, sizeof(float));
    std::memcpy(&record.field_0050, &g_AxisY_Z, sizeof(float));
    record.field_0044 = 0;
    record.field_0054 =
        CSF_AT(uint32_t, reinterpret_cast<void*>(GetGlobalValue_558150()), 0x1C4);
    record.field_0058 =
        CSF_AT(uint32_t, reinterpret_cast<void*>(GetGlobalValue_558150()), 0x1C8);

    void* dispatcher = reinterpret_cast<void*>(GetGlobalObjectField184());
    const uint32_t result =
        DispatchRecordToGlobalObject(dispatcher, nullptr, &record, 1);
    CSF_AT(char, self, 0x58) = 1;
    return static_cast<char>(result);
}

HOOK(0x004CFE50, SceneEffects_SubmitExpiredObjectEffects, "auto");
