#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, S_00526d40_InitGlobalRequest, 0x00405A00, (S_00526d40* self, void*));
CSF_FUNC(std::uint32_t*, __fastcall, InitializePositionRecord_446040, 0x00446040, (std::uint32_t* self, void* , int firstValue, int secondValue, const void* source, const std::uint32_t* position));
CSF_FUNC(uint32_t*, __fastcall, S_00445eb0_SubmitSceneRequest, 0x00446F10, (S_00445eb0* self, void* , int requestAddress));
CSF_FUNC(const std::uint32_t*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(int, __fastcall, S_00521df0_PlaySoundRequest, 0x00523940, (S_00521df0* self, void* , void* request, float volume));
CSF_FUNC(int, __fastcall, SceneLoader_GetResourceById, 0x007567C0, (void* self, void* , int index));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall AiAlert_EmitAlertRequests(void* self, void* /*edx*/)
{
    int result = CSF_AT(int, self, 0x2C);
    if (CSF_AT(int, reinterpret_cast<void*>(result), 0x50) != 1)
        return result;

    void* link = reinterpret_cast<std::uint8_t*>(self) + 8;

    int linked = GetLinkedObjectValue_4DB5C0(link, nullptr);
    result = linked ? linked - 4 : 0;
    if (CSF_AT(int, reinterpret_cast<void*>(result), 0x608) == -1 &&
        CSF_AT(int, reinterpret_cast<void*>(result), 0x60C) == -1)
        return result;

    linked = GetLinkedObjectValue_4DB5C0(link, nullptr);
    result = linked ? linked - 4 : 0;
    if (CSF_AT(std::uint8_t, reinterpret_cast<void*>(result), 0x685))
        return result;

    linked = GetLinkedObjectValue_4DB5C0(link, nullptr);
    result = linked ? linked - 4 : 0;
    if (CSF_AT(int, reinterpret_cast<void*>(result), 0x60C) != -1)
    {
        linked = GetLinkedObjectValue_4DB5C0(link, nullptr);
        result = linked ? linked - 4 : 0;
        if (CSF_AT(int, reinterpret_cast<void*>(result), 0x60C) == 11)
            return result;
    }

    int manager = GetGlobalValue_8BC1E0();
    int resourceId = SceneLoader_GetResourceById(
        reinterpret_cast<std::uint8_t*>(manager) + 0x22A4, nullptr, 2);
    if (resourceId == -1)
        return resourceId;

    linked = GetLinkedObjectValue_4DB5C0(link, nullptr);
    int firstOwner = linked ? linked - 4 : 0;
    linked = GetLinkedObjectValue_4DB5C0(link, nullptr);
    int secondOwner = linked ? linked - 4 : 0;

    void* positionObject = CSF_AT(void*, reinterpret_cast<void*>(firstOwner), 0x2C);
    void* sourceObject = CSF_AT(void*, reinterpret_cast<void*>(secondOwner), 0x2C);
    const std::uint32_t* position = GetObjectPositionVector(positionObject, nullptr);

    alignas(8) std::uint8_t sceneRequestBytes[0x28];
    InitializePositionRecord_446040(
        reinterpret_cast<std::uint32_t*>(sceneRequestBytes), nullptr,
        11, resourceId, reinterpret_cast<std::uint8_t*>(sourceObject) + 0x1C,
        position);
    S_00445eb0_SubmitSceneRequest(
        CSF_AT(S_00445eb0*, g_pCurrentScene, 0xB0), nullptr,
        reinterpret_cast<int>(sceneRequestBytes));

    alignas(8) std::uint8_t soundRequestBytes[0x38];
    S_00526d40& soundRequest =
        *reinterpret_cast<S_00526d40*>(soundRequestBytes);
    S_00526d40_InitGlobalRequest(&soundRequest, nullptr);
    soundRequest.field_0000 = 2;
    CSF_AT(int, soundRequestBytes, 0x08) = resourceId;
    CSF_AT(float, soundRequestBytes, 0x10) = -1.0f;

    linked = GetLinkedObjectValue_4DB5C0(link, nullptr);
    int owner = linked ? linked - 4 : 0;
    void* ownerObject = CSF_AT(void*, reinterpret_cast<void*>(owner), 0x2C);
    CSF_AT(std::uint32_t, soundRequestBytes, 0x28) =
        CSF_AT(std::uint32_t, ownerObject, 0x20);
    CSF_AT(std::uint32_t, soundRequestBytes, 0x2C) =
        CSF_AT(std::uint32_t, ownerObject, 0x24);
    CSF_AT(int, soundRequestBytes, 0x04) = 3;

    int soundManager = GetGlobalValue_521BB0();
    return S_00521df0_PlaySoundRequest(
        reinterpret_cast<S_00521df0*>(soundManager), nullptr,
        soundRequestBytes, -1.0f);
}

HOOK(0x004B2F60, AiAlert_EmitAlertRequests, "auto");
