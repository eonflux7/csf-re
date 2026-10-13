#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(char, __fastcall, CScene_SetFocusEntry, 0x0044FCE0, (CScene* self, void* , int requestedEntry));
CSF_FUNC(char, __fastcall, ResetGlobalValueObject, 0x00457AF0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneObject_IsSecondCheckTrue, 0x0048E140, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_GetByte023C, 0x00495490, (char* self, void*));
CSF_FUNC(int, __fastcall, CharActionFsm_SetCharacterFlag, 0x00498F50, (void* self, void* , int flag, char enabled));
CSF_FUNC(int, __fastcall, C_0085ec18_GetProviderObject, 0x004F4F30, (void* self, void* edx, int value));
CSF_FUNC(void, __fastcall, C_0085ec18_TailCallReferencedObjectSlot50, 0x004F58F0, (void* self, void* edx, void* value));
CSF_FUNC(int, __fastcall, CharacterModel_SetUpdateMode, 0x004F7810, (void* self, void* , int mode));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(void, __fastcall, C_0083f448_SetToggleMode, 0x004FC210, (C_0083f448* self, void* , int mode));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static char __fastcall S_00457af0_ActivatePortrait(
    S_00457af0* self, void* /*edx*/)
{
    // The packet's vtable reference is actually the fallback call target.
    (void)CSF_VTBL(0x00457AF0);

    void* character = reinterpret_cast<void*>(CHandleTable_Resolve(
        g_pCurrentScene, nullptr,
        reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x10)));

    if (!character ||
        !C_GetByte023C(static_cast<char*>(character), nullptr) ||
        !static_cast<uint8_t>(
            SceneObject_IsSecondCheckTrue(character, nullptr)))
    {
        return ResetGlobalValueObject(self, nullptr);
    }

    CharActionFsm_SetCharacterFlag(character, nullptr, 0x80000, 1);
    CScene_SetFocusEntry(
        static_cast<CScene*>(g_pCurrentScene), nullptr,
        reinterpret_cast<int>(character));

    void* globalObject = reinterpret_cast<void*>(GetGlobalValue_4FBDE0());
    void* firstElement =
        reinterpret_cast<void*>(GetFirstElement(globalObject, nullptr));
    void* provider = GetCachedObjectProvider(firstElement, nullptr);

    CharacterModel_SetUpdateMode(provider, nullptr, 7);
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, character, 0)[0x6C / 4])(
            character, nullptr, 0);

    if ((CSF_AT(uint32_t, character, 0x2E0) & 0x2000) == 0)
    {
        alignas(8) uint8_t orientationBytes[0x8];
        alignas(8) uint8_t positionBytes[0xC];

        reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, character, 0)[0x38 / 4])(
                character, nullptr, orientationBytes);
        C_0085ec18_TailCallReferencedObjectSlot50(
            provider, nullptr, orientationBytes);

        int position = reinterpret_cast<int(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, character, 0)[0x34 / 4])(
                character, nullptr, positionBytes);
        C_0085ec18_GetProviderObject(provider, nullptr, position);
    }

    void* referencedObject = GetReferencedObject(provider, nullptr);
    void* model = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
        CSF_AT(void**, referencedObject, 0)[0x10 / 4])(
            referencedObject, nullptr);
    self->field_0028 = static_cast<float>(
        reinterpret_cast<double(__fastcall*)(void*, void*)>(
            CSF_AT(void**, model, 0)[0x28 / 4])(model, nullptr));

    referencedObject = GetReferencedObject(provider, nullptr);
    model = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
        CSF_AT(void**, referencedObject, 0)[0x10 / 4])(
            referencedObject, nullptr);
    reinterpret_cast<void(__fastcall*)(void*, void*, float)>(
        CSF_AT(void**, model, 0)[0x40 / 4])(
            model, nullptr, -self->field_0024);

    if ((self->field_000c & 0x80000000) == 0)
    {
        void* entry = reinterpret_cast<void*>(CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 4)));
        if (entry)
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, entry, 0)[0x6C / 4])(
                    entry, nullptr, 1);
        }
    }

    auto* toggleObject =
        reinterpret_cast<C_0083f448*>(GetGlobalValue_4FBDE0());
    C_0083f448_SetToggleMode(toggleObject, nullptr, 4);
    return 0;
}

HOOK(0x00457D90, S_00457af0_ActivatePortrait, "auto");
