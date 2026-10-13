#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, S_00526d40_InitGlobalRequest, 0x00405A00, (S_00526d40* self, void*));
CSF_FUNC(std::uint32_t*, __fastcall, InitializePositionRecord_446040, 0x00446040, (std::uint32_t* self, void* , int firstValue, int secondValue, const void* source, const std::uint32_t* position));
CSF_FUNC(uint32_t*, __fastcall, S_00445eb0_SubmitSceneRequest, 0x00446F10, (S_00445eb0* self, void* , int requestAddress));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(const std::uint32_t*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(char*, __fastcall, S_00492b20_GetPtr0x1B4, 0x00492B20, (void* self, void*));
CSF_FUNC(int, __fastcall, CPlayerCharacter_GetAlternateSceneSelection, 0x004950B0, (CPlayerCharacter* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(char, __fastcall, CheckGlobalObjectFlag, 0x00521C00, (void* self, void* edx));
CSF_FUNC(int, __fastcall, S_00521df0_SubmitSoundRequest, 0x005242B0, (S_00521df0* self, void* , void* request, int listenerInfo));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);

static int __fastcall C_008316d8_SubmitFireRequests(
    C_008316d8* self, void* /*edx*/,
    int requestId, unsigned int soundId, char requestFlag, char submitScene)
{
    if (requestId == -1)
        return requestId;

    alignas(8) std::uint8_t requestBytes[0x38];
    alignas(8) std::uint8_t positionRecordBytes[0x28];
    alignas(8) std::uint8_t listenerBytes[0x10];

    int sceneRequestType = -1;
    bool suppressSceneRequest = false;

    if (soundId == 1000002)
    {
        sceneRequestType = 8;
    }
    else if (soundId == 1000001)
    {
        sceneRequestType = 6;
        void* owner = reinterpret_cast<void*>(self->field_0004);
        if (CSF_AT(std::uint32_t, owner, 0x50) == 0)
        {
            suppressSceneRequest =
                CPlayerCharacter_GetAlternateSceneSelection(
                    reinterpret_cast<CPlayerCharacter*>(owner), nullptr) != 0;
        }
    }

    if (!suppressSceneRequest && submitScene)
    {
        void* owner = reinterpret_cast<void*>(self->field_0004);
        const void* positionSource =
            reinterpret_cast<std::uint8_t*>(owner) + 0x1C;
        const std::uint32_t* position =
            GetObjectPositionVector(owner, nullptr);

        InitializePositionRecord_446040(
            reinterpret_cast<std::uint32_t*>(positionRecordBytes),
            nullptr, sceneRequestType, requestId, positionSource, position);

        S_00445eb0_SubmitSceneRequest(
            CSF_AT(S_00445eb0*, g_pCurrentScene, 0xB0),
            nullptr, reinterpret_cast<int>(positionRecordBytes));
    }

    if (!CheckGlobalObjectFlag(
            reinterpret_cast<void*>(GetGlobalValue_521BB0()), nullptr))
        return 0;

    S_00526d40& request =
        *reinterpret_cast<S_00526d40*>(requestBytes);
    S_00526d40_InitGlobalRequest(&request, nullptr);

    void* owner = reinterpret_cast<void*>(self->field_0004);
    bool usePlayerRequest = false;

    if (CSF_AT(std::uint32_t, owner, 0x50) == 0 &&
        GetSceneMenuContext(g_pCurrentScene, nullptr) ==
            reinterpret_cast<std::uint32_t>(owner))
    {
        usePlayerRequest = true;
    }
    else if (CSF_AT(std::uint32_t, owner, 0x50) == 19)
    {
        void* context = reinterpret_cast<void*>(
            GetSceneMenuContext(g_pCurrentScene, nullptr));

        // The packet also lists this accessor address as a vtable reference.
        (void)CSF_VTBL(0x00492B20);
        char* selection = S_00492b20_GetPtr0x1B4(context, nullptr);

        usePlayerRequest =
            CSF_AT(std::uint32_t, owner, 0x20) ==
            CSF_AT(std::uint32_t, selection, 0x04);
    }

    if (usePlayerRequest)
    {
        request.field_0000 = 4;
        CSF_AT(int, requestBytes, 0x04) = 4;
    }
    else
    {
        request.field_0000 = 2;
        CSF_AT(int, requestBytes, 0x04) = 1;
    }

    owner = reinterpret_cast<void*>(self->field_0004);
    CSF_AT(int, requestBytes, 0x08) = requestId;
    CSF_AT(float, requestBytes, 0x10) = -1.0f;

    const std::uint32_t positionX = CSF_AT(std::uint32_t, owner, 0x20);
    CSF_AT(std::uint32_t, requestBytes, 0x28) = positionX;
    const std::uint32_t positionY = CSF_AT(std::uint32_t, owner, 0x24);
    CSF_AT(std::uint32_t, requestBytes, 0x2C) = positionY;
    CSF_AT(char, requestBytes, 0x35) = requestFlag;

    CSF_AT(void**, listenerBytes, 0x00) = CSF_VTBL(0x0082D448);
    CSF_AT(std::uint32_t, listenerBytes, 0x04) = positionX;
    CSF_AT(std::uint32_t, listenerBytes, 0x08) = positionY;

    // MSVC used this constant to implement the unsigned-to-float conversion.
    (void)&g_UnsignedIntegerBias;
    CSF_AT(float, listenerBytes, 0x0C) = static_cast<float>(soundId);

    auto* soundManager =
        reinterpret_cast<S_00521df0*>(GetGlobalValue_521BB0());
    return S_00521df0_SubmitSoundRequest(
        soundManager, nullptr, &request, reinterpret_cast<int>(listenerBytes));
}

HOOK(0x00477060, C_008316d8_SubmitFireRequests, "auto");
