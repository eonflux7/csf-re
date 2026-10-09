#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(bool, __fastcall, SceneManager_IsIdActive, 0x00468880, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SceneManager_BindObjectToId, 0x00468C60, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, SceneManager_UnbindObjectFromId, 0x00468CC0, (void* self, void* edx, int a2, int a3));

// Scene manager sub-object at g_pCurrentScene + 0xBC (slot 47 of the scene pointer array).
static void* ActiveSceneManager()
{
    return CSF_AT(void*, g_pCurrentScene, 0xBC);
}

static int AsInt(const void* pointer)
{
    return static_cast<int>(reinterpret_cast<std::intptr_t>(pointer));
}

static uint32_t ReadDwordAt(uint32_t address, uint32_t offset)
{
    return CSF_AT(uint32_t, reinterpret_cast<void*>(address), offset);
}

static uint8_t ReadByteAt(uint32_t address, uint32_t offset)
{
    return CSF_AT(uint8_t, reinterpret_cast<void*>(address), offset);
}

// requestArgs[1] -> +4 -> byte at +1 (request enabled flag).
static uint8_t ReadRequestEnabledFlag(const uint32_t* requestArgs)
{
    const uint32_t flagOwner = ReadDwordAt(requestArgs[1], 4);
    return ReadByteAt(flagOwner, 1);
}

// requestArgs[0] -> +4 -> +4 (requested id).
static uint32_t ReadRequestedId(const uint32_t* requestArgs)
{
    const uint32_t idHolder = ReadDwordAt(requestArgs[0], 4);
    return ReadDwordAt(idHolder, 4);
}

static int MarkStateIdle(S_0045d610* self, int result)
{
    self->field_0018 = 2;
    self->field_0082 = 0;
    return result;
}

static int __fastcall S_0045d610_SyncBoundIdFromRequest(S_0045d610* self, void* /*edx*/, int /*a2*/, int /*a3*/, uint32_t* requestArgs)
{
    if (ReadRequestEnabledFlag(requestArgs) == 1)
    {
        const bool requestedIdIsActive = SceneManager_IsIdActive(ActiveSceneManager(), nullptr, static_cast<int>(ReadRequestedId(requestArgs)));
        if (requestedIdIsActive)
        {
            return static_cast<int>(requestedIdIsActive);
        }
    }

    const uint32_t requestedId = ReadRequestedId(requestArgs);
    const uint32_t currentId = self->field_006c;

    if (currentId != requestedId)
    {
        self->field_006c = requestedId;
        const int bindResult = SceneManager_BindObjectToId(ActiveSceneManager(), nullptr, AsInt(self), static_cast<int>(requestedId));
        return MarkStateIdle(self, bindResult);
    }

    const int currentIdAsResult = static_cast<int>(currentId);
    if ((currentId & 0x8000000) != 0)
    {
        if (self->field_0028 != 1)
        {
            return MarkStateIdle(self, currentIdAsResult);
        }
        if (CSF_AT(uint32_t, self, 0x48) != (currentId & 0xF7FFFFFF))
        {
            return MarkStateIdle(self, currentIdAsResult);
        }
    }
    else if (currentId != self->field_0028)
    {
        return MarkStateIdle(self, currentIdAsResult);
    }

    if (ReadRequestEnabledFlag(requestArgs) != 0)
    {
        const int unbindResult = SceneManager_UnbindObjectFromId(ActiveSceneManager(), nullptr, AsInt(self), static_cast<int>(currentId));
        self->field_006c = 0xFFFFFFFFu;
        return unbindResult;
    }

    return MarkStateIdle(self, currentIdAsResult);
}

HOOK(0x0045D610, S_0045d610_SyncBoundIdFromRequest, "auto");
