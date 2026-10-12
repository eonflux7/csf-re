#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));
CSF_FUNC(void, __fastcall, C_00833b88_UpdateState, 0x00492C80, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, CPlayerCharacter_UpdateTimedBehavior, 0x00493BE0, (void* self, void* edx, float a2));
CSF_FUNC(void, __fastcall, CPlayerCharacter_RefreshState, 0x004947A0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, CPlayerCharacter_FinishTimedUpdate, 0x00495E80, (void* self, void* edx, float a2));
CSF_FUNC(void, __fastcall, CPlayerCharacter_RefreshAuxiliaryState, 0x004961A0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, CPlayerCharacter_UpdateActiveBehavior, 0x00496290, (void* self, void* edx, float a2));
CSF_FUNC(void, __fastcall, CPlayerCharacter_RefreshBehavior, 0x00497150, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, CPlayerCharacter_RefreshBehaviorData, 0x004974C0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, CPlayerCharacter_PrepareActiveUpdate, 0x004977E0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, CPlayerCharacter_UpdateWithGlobalTime, 0x00497930, (void* self, void* edx, float a3));
CSF_FUNC(void, __fastcall, CPlayerCharacter_RefreshActiveState, 0x004980D0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, CPlayerCharacter_RefreshActiveBehavior, 0x00498310, (void* self, void* edx));
CSF_FUNC(void, __fastcall, CPlayerCharacter_RefreshSecondaryBehavior, 0x00498880, (void* self, void* edx));
CSF_FUNC(unsigned int*, __fastcall, CPlayerCharacter_RefreshUpdateData, 0x0049BDF0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, UpdateEmbeddedTimedState, 0x004A1680, (void* self, void* edx, float a2));
CSF_FUNC(int, __fastcall, UpdateAssociatedState, 0x004A19E0, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, FinishAssociatedUpdate, 0x004A2490, (void* self, void* edx, float a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());

static int __fastcall CPlayerCharacter_Update(
    CPlayerCharacter* self, void* /*edx*/, int elapsedBits)
{
    if ((self->field_0170 & 0x10) != 0)
    {
        if (self->field_0394 != 0)
        {
            void* source = reinterpret_cast<void*>(self->field_0038);
            int value =
                reinterpret_cast<int (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, source, 0)[0xA0 / 4])(source, nullptr);
            CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_03e8), 0x0C) =
                static_cast<uint32_t>(value);
            self->field_0394 = 0;
        }

        float elapsed;
        std::memcpy(&elapsed, &elapsedBits, sizeof(elapsed));

        self->field_020c = elapsed + self->field_020c;
        self->field_0210 = elapsed + self->field_0210;
        self->field_0214 = elapsed + self->field_0214;
        self->field_0218 = elapsed + self->field_0218;
        self->field_021c = elapsed + self->field_021c;
        self->field_0220 = elapsed + self->field_0220;
        self->field_0224 = elapsed + self->field_0224;

        C_00833b88_UpdateState(self, nullptr, elapsedBits);
        UpdateAssociatedState(
            reinterpret_cast<void*>(self->field_03e8), nullptr, elapsedBits);

        if (static_cast<uint8_t>(SceneObj_IsInactive(self, nullptr)) == 0)
        {
            CPlayerCharacter_PrepareActiveUpdate(self, nullptr);
            CPlayerCharacter_UpdateTimedBehavior(self, nullptr, elapsed);

            if (reinterpret_cast<uint32_t>(self) ==
                    GetSceneMenuContext(g_pCurrentScene, nullptr) &&
                !(self->field_0224 < g_HalfDurationScale))
            {
                self->field_0224 = 0.0f;
                self->field_03dc = 0;
            }

            CPlayerCharacter_UpdateActiveBehavior(self, nullptr, elapsed);
            CPlayerCharacter_RefreshUpdateData(self, nullptr);

            void* globalState =
                reinterpret_cast<void*>(GetGlobalState_50D190());
            CPlayerCharacter_UpdateWithGlobalTime(
                self, nullptr, CSF_AT(float, globalState, 0x9C));

            CPlayerCharacter_RefreshActiveState(self, nullptr);
            CPlayerCharacter_RefreshActiveBehavior(self, nullptr);
            CPlayerCharacter_RefreshBehavior(self, nullptr);
            CPlayerCharacter_RefreshSecondaryBehavior(self, nullptr);
            CPlayerCharacter_RefreshBehaviorData(self, nullptr);
            CPlayerCharacter_RefreshAuxiliaryState(self, nullptr);
            UpdateEmbeddedTimedState(
                reinterpret_cast<uint8_t*>(self) + 0x344, nullptr, elapsed);
            CPlayerCharacter_RefreshState(self, nullptr);
            CPlayerCharacter_FinishTimedUpdate(self, nullptr, elapsed);
        }

        FinishAssociatedUpdate(
            reinterpret_cast<void*>(self->field_03e8), nullptr, elapsed);
    }

    uint32_t remaining = self->field_0400;
    self->field_03f8 = 0;
    if (remaining > 0)
    {
        --remaining;
        self->field_0400 = remaining;
    }
    return static_cast<int>(remaining);
}

HOOK(0x0049C030, CPlayerCharacter_Update, "auto");
