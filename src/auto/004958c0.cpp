#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char*, __fastcall, GetToggleTimerRotation, 0x004A1200, (void* self, void* edx));
CSF_FUNC(int, __fastcall, S_004a1210_IsToggleTimerActive, 0x004A1210, (S_004a1210* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

CSF_GLOBAL(float, g_MinimumRotation, 0x008305EC);
CSF_GLOBAL(float, g_MaximumRotation, 0x008305F0);

static int __fastcall CharacterModel_UpdateSpecialCharacter(
    void* self, void* /*edx*/, int /*unused*/)
{
    void* firstElement =
        reinterpret_cast<void*>(GetFirstElement(
            reinterpret_cast<void*>(GetGlobalValue_4FBDE0()), nullptr));
    void* provider = GetCachedObjectProvider(firstElement, nullptr);
    void* referencedObject = GetReferencedObject(provider, nullptr);

    ((void* (__fastcall*)(void*, void*))
        CSF_AT(void**, referencedObject, 0)[0x10 / 4])(
            referencedObject, nullptr);

    int result = CSF_AT(int, self, 0x2E0);
    if ((result & 0x2000) == 0)
        return result;

    auto* timer = reinterpret_cast<S_004a1210*>(
        reinterpret_cast<std::uint8_t*>(self) + 0x344);

    // The packet labels this callee address as a vtable reference.
    (void)CSF_VTBL(0x004A1210);

    result = S_004a1210_IsToggleTimerActive(timer, nullptr);
    if (static_cast<std::uint8_t>(result) == 0)
        return result;

    char* rotation = GetToggleTimerRotation(timer, nullptr);
    int horizontalRotation = CSF_AT(int, rotation, 0);
    float verticalRotation = CSF_AT(float, rotation, 4);

    firstElement = reinterpret_cast<void*>(GetFirstElement(
        reinterpret_cast<void*>(GetGlobalValue_4FBDE0()), nullptr));
    provider = GetCachedObjectProvider(firstElement, nullptr);
    referencedObject = GetReferencedObject(provider, nullptr);

    void* specialCharacter =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, referencedObject, 0)[0x10 / 4])(
                referencedObject, nullptr);

    ((void (__fastcall*)(void*, void*, void*))
        CSF_AT(void**, specialCharacter, 0)[0x58 / 4])(
            specialCharacter, nullptr,
            reinterpret_cast<std::uint8_t*>(self) + 0x3A8);

    if (verticalRotation > g_MaximumRotation)
        verticalRotation = 89.0f;
    else if (verticalRotation < g_MinimumRotation)
        verticalRotation = -89.0f;

    ((void (__fastcall*)(void*, void*, int))
        CSF_AT(void**, specialCharacter, 0)[0x40 / 4])(
            specialCharacter, nullptr, horizontalRotation);

    return ((int (__fastcall*)(void*, void*, float))
        CSF_AT(void**, specialCharacter, 0)[0x3C / 4])(
            specialCharacter, nullptr, verticalRotation);
}

HOOK(0x004958C0, CharacterModel_UpdateSpecialCharacter, "auto");
