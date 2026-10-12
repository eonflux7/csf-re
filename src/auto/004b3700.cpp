#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, CPlayerCharacter_GetAlternateSceneSelection, 0x004950B0, (CPlayerCharacter* self, void*));
CSF_FUNC(int, __fastcall, AiAlert_PlayLinkedAlertSound, 0x004B3310, (void* self, void* edx));
CSF_FUNC(void, __fastcall, AiCoverAttackTasks_UpdateCoverState, 0x004D1120, (float* self, void* , float a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static uint8_t* AiAlert_GetLinkedOwner(void* link)
{
    const int linked = GetLinkedObjectValue_4DB5C0(link, nullptr);
    return linked ? reinterpret_cast<uint8_t*>(linked - 4) : nullptr;
}

static int __fastcall AiAlert_UpdateLinkedCoverState(void* self, void* /*edx*/, void* link)
{
    if (!CSF_AT(uint8_t, self, 0x30))
    {
        uint8_t* owner = AiAlert_GetLinkedOwner(link);
        if (CSF_AT(int, owner, 0x618) >= 0 &&
            CHandleTable_Resolve(g_pCurrentScene, nullptr,
                                 reinterpret_cast<int>(owner + 0x610)))
        {
            uint8_t* handleOwner = AiAlert_GetLinkedOwner(link);
            owner = AiAlert_GetLinkedOwner(link);
            void* settings = CSF_AT(void*, owner, 0x2C);
            void* controller = CSF_AT(void*, settings, 0x3C);

            if (((bool(__fastcall*)(void*, void*, void*, int))
                     CSF_AT(void**, controller, 0)[0x9C / 4])(
                    controller, nullptr, handleOwner + 0x610, 3))
            {
                owner = AiAlert_GetLinkedOwner(link);
                AiAlert_PlayLinkedAlertSound(owner, nullptr);
            }
        }
        CSF_AT(uint8_t, self, 0x30) = 1;
    }

    uint8_t* owner = AiAlert_GetLinkedOwner(link);
    int result = reinterpret_cast<int>(owner);
    if (CSF_AT(int, owner, 0x60C) == -1)
        return result;

    owner = AiAlert_GetLinkedOwner(link);
    result = reinterpret_cast<int>(owner);
    if (CSF_AT(uint8_t, owner, 0x684))
        return result;

    owner = AiAlert_GetLinkedOwner(link);
    result = reinterpret_cast<int>(owner);
    if (CSF_AT(int, owner, 0x618) < 0)
        return result;

    result = CHandleTable_Resolve(g_pCurrentScene, nullptr,
                                  reinterpret_cast<int>(owner + 0x610));
    if (!result)
        return result;

    owner = AiAlert_GetLinkedOwner(link);
    result = CSF_AT(int, owner, 0x618) < 0
                 ? 0
                 : CHandleTable_Resolve(g_pCurrentScene, nullptr,
                                        reinterpret_cast<int>(owner + 0x610));
    if (CSF_AT(int, reinterpret_cast<void*>(result), 0x50))
        return result;

    owner = AiAlert_GetLinkedOwner(link);
    CPlayerCharacter* character =
        CSF_AT(int, owner, 0x618) < 0
            ? nullptr
            : reinterpret_cast<CPlayerCharacter*>(
                  CHandleTable_Resolve(g_pCurrentScene, nullptr,
                                       reinterpret_cast<int>(owner + 0x610)));
    const int selection =
        CPlayerCharacter_GetAlternateSceneSelection(character, nullptr);

    owner = AiAlert_GetLinkedOwner(link);
    void* settings = CSF_AT(void*, owner, 0x2C);
    result = reinterpret_cast<int>(settings);
    if (selection > CSF_AT(int, settings, 0x7C))
    {
        owner = AiAlert_GetLinkedOwner(link);
        result = reinterpret_cast<int>(owner);
        AiCoverAttackTasks_UpdateCoverState(
            reinterpret_cast<float*>(owner + 0x690), nullptr, -1.0f);
        // This callee leaves EAX unchanged.
    }
    return result;
}

HOOK(0x004B3700, AiAlert_UpdateLinkedCoverState, "auto");
