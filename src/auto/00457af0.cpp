#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(char, __fastcall, CScene_SetFocusEntry, 0x0044FCE0, (CScene* self, void* , int requestedEntry));
CSF_FUNC(char, __fastcall, SceneSelection_CycleEligibleObject, 0x00451000, (unsigned int* self, void* , char backward));
CSF_FUNC(void*, __stdcall, HudPortraits_ResolveEligibleCharacter, 0x00457AB0, (int a2));
CSF_FUNC(int, __fastcall, CharActionFsm_SetCharacterFlag, 0x00498F50, (void* self, void* , int flag, char enabled));
CSF_FUNC(int, __fastcall, CharacterModel_SetUpdateMode, 0x004F7810, (void* self, void* , int mode));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(void, __fastcall, CommandoAnims_ClearToggleOnMatch, 0x004FC390, (void* self, void* , int a2));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

static char __fastcall S_00457af0_ResetSelectionAndRestoreFocus(
    S_00457af0* self, void* /*edx*/)
{
    if (self->field_001c == 0)
        return 0;

    self->field_001c = 0;
    void* animations = reinterpret_cast<void*>(GetGlobalValue_4FBDE0());
    CommandoAnims_ClearToggleOnMatch(animations, nullptr, 4);

    C_00833b88* primaryCharacter = nullptr;
    if (static_cast<int32_t>(self->field_0018) >= 0)
    {
        const int primaryHandle =
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x10);
        primaryCharacter = reinterpret_cast<C_00833b88*>(
            CHandleTable_Resolve(g_pCurrentScene, nullptr, primaryHandle));

        if (primaryCharacter)
        {
            CSF_AT(uint32_t, primaryCharacter, 0x2E4) &= ~0x80000u;
            CharActionFsm_SetCharacterFlag(primaryCharacter, nullptr, 0x80000, 0);
            primaryCharacter->vSlot6C(1);

            primaryCharacter = reinterpret_cast<C_00833b88*>(
                HudPortraits_ResolveEligibleCharacter(primaryHandle));
            if (primaryCharacter)
            {
                animations = reinterpret_cast<void*>(GetGlobalValue_4FBDE0());
                void* firstElement =
                    reinterpret_cast<void*>(GetFirstElement(animations, nullptr));
                void* provider = GetCachedObjectProvider(firstElement, nullptr);
                CharacterModel_SetUpdateMode(provider, nullptr, 0);
                CScene_SetFocusEntry(
                    reinterpret_cast<CScene*>(g_pCurrentScene), nullptr,
                    reinterpret_cast<int>(primaryCharacter));
                primaryCharacter->vSlot6C(1);

                void* referencedObject = GetReferencedObject(provider, nullptr);
                void* updateObject =
                    reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                        CSF_AT(void**, referencedObject, 0)[0x10 / 4])(
                            referencedObject, nullptr);
                reinterpret_cast<void (__fastcall*)(void*, void*, float)>(
                    CSF_AT(void**, updateObject, 0)[0x34 / 4])(
                        updateObject, nullptr, self->field_0028);
            }
        }
    }

    int result = static_cast<int32_t>(self->field_000c);
    if (result >= 0)
    {
        result = CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 4));
        if (result != 0)
        {
            void* secondaryObject = reinterpret_cast<void*>(result);
            CSF_AT(uint32_t, secondaryObject, 0x2E4) &= ~0x80000u;
            CharActionFsm_SetCharacterFlag(secondaryObject, nullptr, 0x80000, 0);

            // This path consumes AL; the generated void wrapper loses that result.
            result = reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, secondaryObject, 0)[0x6C / 4])(
                    secondaryObject, nullptr, 1);
            if (primaryCharacter)
                return static_cast<char>(result);

            void* secondaryCharacter = HudPortraits_ResolveEligibleCharacter(
                reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 4));
            if (secondaryCharacter)
            {
                animations = reinterpret_cast<void*>(GetGlobalValue_4FBDE0());
                void* firstElement =
                    reinterpret_cast<void*>(GetFirstElement(animations, nullptr));
                void* provider = GetCachedObjectProvider(firstElement, nullptr);
                CharacterModel_SetUpdateMode(provider, nullptr, 0);
                CScene_SetFocusEntry(
                    reinterpret_cast<CScene*>(g_pCurrentScene), nullptr,
                    reinterpret_cast<int>(secondaryCharacter));
                return static_cast<char>(
                    reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                        CSF_AT(void**, secondaryCharacter, 0)[0x6C / 4])(
                            secondaryCharacter, nullptr, 1));
            }
        }
        else if (primaryCharacter)
        {
            return static_cast<char>(result);
        }
    }
    else if (primaryCharacter)
    {
        return static_cast<char>(result);
    }

    return SceneSelection_CycleEligibleObject(
        reinterpret_cast<unsigned int*>(g_pCurrentScene), nullptr, 0);
}

HOOK(0x00457AF0, S_00457af0_ResetSelectionAndRestoreFocus, "auto");
