#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fHudFadeStep, 0x0082E04C);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(uint32_t*, __fastcall, CharacterModel_InitializeTrace, 0x00401060, (void* self, void* edx, int a2, uint32_t* a3));
CSF_FUNC(uint32_t*, __fastcall, CharActionFsm_InitializeHitInfo, 0x0040ED90, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, CharacterModel_CombineTransforms, 0x0048E060, (void* self, void* edx, int a2, uint32_t* a3));
CSF_FUNC(char, __fastcall, C_00495880_GetField3A5, 0x00495880, (uint8_t* self, void*));
CSF_FUNC(void, __fastcall, CharacterModel_UpdatePresentation, 0x004F51B0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, CharacterModel_SetAttachmentMode, 0x004F5420, (void* self, void* edx, int a2));
CSF_FUNC(bool, __fastcall, MpSpawnPoints_IsSpawnChangeAllowed, 0x004F5740, (void* self, void* edx));
CSF_FUNC(void, __fastcall, CharacterModel_UpdateSpawnTransition, 0x004F6990, (void* self, void* edx, float a2));
CSF_FUNC(void, __fastcall, CharacterModel_UpdateSpecialState, 0x004F6F30, (void* self, void* edx, float a3));
CSF_FUNC(int, __fastcall, ObjectProvider_SetUpdateMode, 0x004F7810, (void* self, void* edx, int a3));
CSF_FUNC(char, __fastcall, CharacterModel_UpdateFromOwner, 0x004F8150, (void* self, void* edx, void* a2, int a3));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GfxMatrix_CopyTranslationAndUpdate, 0x006AD9E0, (void* self, void* , const uint32_t* src));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

static void __fastcall CharacterModel_Update(void* self, void* /*edx*/, float deltaTime)
{
    const int initialState = CSF_AT(int, self, 0x08);
    CSF_AT(float, self, 0xC8) = deltaTime + CSF_AT(float, self, 0xC8);

    if (initialState == 7)
    {
        CharacterModel_UpdatePresentation(self, nullptr);
        return;
    }

    char ownerFlag = 0;
    auto* handle = static_cast<uint8_t*>(self) + 0x74;
    if (CSF_AT(int, self, 0x7C) >= 0)
    {
        auto* owner = reinterpret_cast<uint8_t*>(
            CHandleTable_Resolve(g_pCurrentScene, nullptr, reinterpret_cast<int>(handle)));
        if (owner && !CSF_AT(uint32_t, owner, 0x50))
            ownerFlag = C_00495880_GetField3A5(owner, nullptr);
    }

    const int stateBeforeTransition = CSF_AT(int, self, 0x08);
    if (stateBeforeTransition != 0 || CSF_AT(int, self, 0x7C) < 0)
    {
        if (stateBeforeTransition != 3 || MpSpawnPoints_IsSpawnChangeAllowed(self, nullptr))
            CSF_AT(int, self, 0x0C) = 0;
        else
            CharacterModel_SetAttachmentMode(self, nullptr, 2);
    }
    else if (!ownerFlag)
    {
        CharacterModel_SetAttachmentMode(self, nullptr, 1);
    }

    switch (CSF_AT(int, self, 0x08))
    {
    case 3:
        CharacterModel_UpdateSpawnTransition(self, nullptr, deltaTime);
        break;
    case 4:
    case 6:
        break;
    case 5:
    {
        auto* provider = CSF_AT(C_0085ec18*, self, 0x04);
        void* model = provider->vGetReferencedObject();
        const auto* position =
            reinterpret_cast<const uint32_t*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, model, 0)[0x48 / 4])(model, nullptr);

        alignas(8) uint8_t positionBytes[0x0C];
        CSF_AT(uint32_t, positionBytes, 0x00) = position[0];
        CSF_AT(uint32_t, positionBytes, 0x04) = position[1];
        CSF_AT(uint32_t, positionBytes, 0x08) = 0;

        provider = CSF_AT(C_0085ec18*, self, 0x04);
        model = provider->vGetReferencedObject();
        reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, model, 0)[0x4C / 4])(model, nullptr, positionBytes);
        break;
    }
    case 9:
        CharacterModel_UpdateSpecialState(self, nullptr, deltaTime);
        break;
    default:
    {
        void* owner = reinterpret_cast<void*>(
            CHandleTable_Resolve(g_pCurrentScene, nullptr, reinterpret_cast<int>(handle)));
        if (owner)
        {
            int deltaBits;
            std::memcpy(&deltaBits, &deltaTime, sizeof(deltaBits));
            CharacterModel_UpdateFromOwner(self, nullptr, owner, deltaBits);
        }
        else
        {
            CSF_AT(int, self, 0x78) = -1;
            CSF_AT(int, self, 0x7C) = -1;
            ObjectProvider_SetUpdateMode(self, nullptr, 4);
        }
        break;
    }
    }

    void* attachment = CSF_AT(void*, self, 0x10);
    if (attachment && !ownerFlag)
    {
        void* attachmentModel =
            reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, attachment, 0)[0x68 / 4])(attachment, nullptr);

        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, attachmentModel, 0)[0x24 / 4])(attachmentModel, nullptr, 0);

        const int attachmentMode = CSF_AT(int, self, 0x0C);
        if (attachmentMode == 1)
        {
            void* frame =
                reinterpret_cast<void*(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, attachmentModel, 0)[0x6C / 4])(
                        attachmentModel, nullptr, 0);
            if (frame)
            {
                alignas(8) uint8_t modelTransform[0x08];
                alignas(8) uint8_t frameTransform[0x08];
                alignas(8) uint8_t combinedTransform[0x08];

                auto* provider = CSF_AT(C_0085ec18*, self, 0x04);
                void* model = provider->vGetReferencedObject();
                reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
                    CSF_AT(void**, model, 0)[0x90 / 4])(
                        model, nullptr, modelTransform);

                reinterpret_cast<void*(__fastcall*)(void*, void*, void*)>(
                    CSF_AT(void**, frame, 0)[0x1C / 4])(
                        frame, nullptr, frameTransform);

                CharacterModel_CombineTransforms(
                    frameTransform, nullptr, reinterpret_cast<int>(combinedTransform),
                    reinterpret_cast<uint32_t*>(modelTransform));

                if (CSF_AT(uint8_t, self, 0x20))
                {
                    void* globals = reinterpret_cast<void*>(GetGlobalInt());
                    void* controller = reinterpret_cast<void*>(
                        SceneLoader_GetSceneController(globals, nullptr));
                    void* query =
                        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                            CSF_AT(void**, controller, 0)[0x3C / 4])(
                                controller, nullptr);

                    alignas(8) uint8_t trace[0x1C];
                    CharacterModel_InitializeTrace(
                        trace, nullptr, 1114636288,
                        reinterpret_cast<uint32_t*>(
                            reinterpret_cast<uint8_t*>(
                                CSF_AT(uint32_t*, combinedTransform, 0)) + 0x30));

                    alignas(8) uint8_t filter[0x04];
                    CSF_AT(int, filter, 0) = 8;
                    void* hit =
                        reinterpret_cast<void*(__fastcall*)(void*, void*, void*, void*, int)>(
                            CSF_AT(void**, query, 0)[0x2C / 4])(
                                query, nullptr, trace, filter, 3);

                    if (hit)
                    {
                        alignas(8) uint8_t hitInfo[0x68];
                        CharActionFsm_InitializeHitInfo(hitInfo, nullptr);
                        reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
                            CSF_AT(void**, hit, 0)[0x0C / 4])(
                                hit, nullptr, hitInfo);

                        CSF_AT(float, trace, 0x18) =
                            CSF_AT(float, hitInfo, 0x18) * g_fHudFadeStep;

                        alignas(8) uint8_t translation[0x0C];
                        CSF_AT(float, translation, 0x00) =
                            CSF_AT(float, hitInfo, 0x10) * g_fHudFadeStep +
                            CSF_AT(float, hitInfo, 0x04);
                        CSF_AT(float, translation, 0x04) =
                            CSF_AT(float, hitInfo, 0x08) +
                            CSF_AT(float, hitInfo, 0x14) * g_fHudFadeStep;
                        CSF_AT(float, translation, 0x08) =
                            CSF_AT(float, hitInfo, 0x0C) +
                            CSF_AT(float, trace, 0x18);

                        GfxMatrix_CopyTranslationAndUpdate(
                            combinedTransform, nullptr,
                            reinterpret_cast<const uint32_t*>(translation));
                    }
                }

                provider = CSF_AT(C_0085ec18*, self, 0x04);
                model = provider->vGetReferencedObject();
                reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
                    CSF_AT(void**, model, 0)[0x98 / 4])(
                        model, nullptr, combinedTransform);

                if (CSF_AT(uint8_t, combinedTransform, 0x04))
                    RwMatrixDestroy(CSF_AT(uint32_t*, combinedTransform, 0));
                if (CSF_AT(uint8_t, frameTransform, 0x04))
                    RwMatrixDestroy(CSF_AT(uint32_t*, frameTransform, 0));
                if (CSF_AT(uint8_t, modelTransform, 0x04))
                    RwMatrixDestroy(CSF_AT(uint32_t*, modelTransform, 0));
            }
        }
        else if (attachmentMode == 2)
        {
            void* frame =
                reinterpret_cast<void*(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, attachmentModel, 0)[0x6C / 4])(
                        attachmentModel, nullptr, 0);
            if (frame)
            {
                auto* provider = CSF_AT(C_0085ec18*, self, 0x04);
                void* model = provider->vGetReferencedObject();
                void** modelVtable = CSF_AT(void**, model, 0);

                alignas(8) uint8_t frameTransform[0x08];
                void* transform =
                    reinterpret_cast<void*(__fastcall*)(void*, void*, void*)>(
                        CSF_AT(void**, frame, 0)[0x1C / 4])(
                            frame, nullptr, frameTransform);

                reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
                    modelVtable[0x98 / 4])(model, nullptr, transform);

                if (CSF_AT(uint8_t, frameTransform, 0x04))
                    RwMatrixDestroy(CSF_AT(uint32_t*, frameTransform, 0));
            }
        }
    }

    CharacterModel_UpdatePresentation(self, nullptr);
}

HOOK(0x004F84C0, CharacterModel_Update, "auto");
