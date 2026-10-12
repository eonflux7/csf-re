#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, S_00526d40_InitGlobalRequest, 0x00405A00, (S_00526d40* self, void*));
CSF_FUNC(std::uint32_t*, __fastcall, InitializePositionRecord_446040, 0x00446040, (std::uint32_t* self, void* , int firstValue, int secondValue, const void* source, const std::uint32_t* position));
CSF_FUNC(uint32_t*, __fastcall, S_00445eb0_SubmitSceneRequest, 0x00446F10, (S_00445eb0* self, void* , int requestAddress));
CSF_FUNC(const std::uint32_t*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(char, __fastcall, CoverAttack_CheckState_4ADAE0, 0x004ADAE0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(int, __fastcall, S_00521df0_PlaySoundRequest, 0x00523940, (S_00521df0* self, void* , void* request, float volume));
CSF_FUNC(int, __fastcall, SceneLoader_GetResourceById, 0x007567C0, (void* self, void* , int index));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void __fastcall AiAlert_UpdateCoverAttackFeedback(
    void* self, void* /*edx*/, float elapsedTime)
{
    auto* bytes = static_cast<std::uint8_t*>(self);
    const float remaining = CSF_AT(float, self, 0x6A4) - elapsedTime;
    CSF_AT(float, self, 0x6A4) = remaining;
    if (!(remaining < g_fZero))
        return;

    auto* owner = CSF_AT(C_008342e8*, self, 0x2C);
    CSF_AT(float, self, 0x6A4) = 5.0f;
    if (owner->field_0050 != 1)
        return;

    int linkedValue = GetLinkedObjectValue_4DB5C0(bytes + 8, nullptr);
    void* linkedObject = reinterpret_cast<void*>(linkedValue ? linkedValue - 4 : 0);
    if (CSF_AT(int, linkedObject, 0x608) == -1 &&
        CSF_AT(int, linkedObject, 0x60C) == -1)
        return;
    if (CSF_AT(int, self, 0x608) == -1)
        return;

    void* attackPosition = bytes + 0x628;
    if (CSF_AT(int, self, 0x630) < 0)
        attackPosition = bytes + 0x610;
    if (!CoverAttack_CheckState_4ADAE0(
            bytes + 0x5DC, nullptr, reinterpret_cast<int>(attackPosition)))
        return;
    if (!CSF_AT(std::uint8_t, self, 0x684))
        return;

    int sceneLoader = GetGlobalValue_8BC1E0();
    int resourceId = SceneLoader_GetResourceById(
        reinterpret_cast<std::uint8_t*>(sceneLoader) + 0x22A4, nullptr, 2);
    if (resourceId == -1)
        return;

    alignas(8) std::uint8_t sceneRequestBytes[0x28];
    owner = CSF_AT(C_008342e8*, self, 0x2C);
    const std::uint32_t* position = GetObjectPositionVector(owner, nullptr);
    InitializePositionRecord_446040(
        reinterpret_cast<std::uint32_t*>(sceneRequestBytes),
        nullptr, 11, resourceId, &owner->field_001c, position);
    S_00445eb0_SubmitSceneRequest(
        CSF_AT(S_00445eb0*, g_pCurrentScene, 0xB0),
        nullptr, reinterpret_cast<int>(sceneRequestBytes));

    if (CSF_AT(std::uint8_t, self, 0x6A0))
        return;

    alignas(8) std::uint8_t soundRequestBytes[0x38];
    S_00526d40& soundRequest =
        *reinterpret_cast<S_00526d40*>(soundRequestBytes);
    S_00526d40_InitGlobalRequest(&soundRequest, nullptr);
    soundRequest.field_0000 = 2;
    CSF_AT(int, soundRequestBytes, 0x08) = resourceId;
    CSF_AT(float, soundRequestBytes, 0x10) = -1.0f;

    linkedValue = GetLinkedObjectValue_4DB5C0(bytes + 8, nullptr);
    linkedObject = reinterpret_cast<void*>(linkedValue ? linkedValue - 4 : 0);
    auto* linkedOwner = CSF_AT(C_00833908*, linkedObject, 0x2C);
    CSF_AT(std::uint32_t, soundRequestBytes, 0x28) = linkedOwner->field_0020;
    CSF_AT(std::uint32_t, soundRequestBytes, 0x2C) = linkedOwner->field_0024;
    CSF_AT(int, soundRequestBytes, 0x04) = 3;

    auto* soundManager =
        reinterpret_cast<S_00521df0*>(GetGlobalValue_521BB0());
    S_00521df0_PlaySoundRequest(
        soundManager, nullptr, soundRequestBytes, -1.0f);
    CSF_AT(std::uint8_t, self, 0x6A0) = 1;
}

HOOK(0x004B30D0, AiAlert_UpdateCoverAttackFeedback, "auto");
