#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, AudioSoundManager_ProcessArchiveContext, 0x00405450, (void* context));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));
CSF_GLOBAL(void*, g_ObjectReferenceManager, 0x008B0550);

// __thiscall method on TAiState (ecx = self), one stack argument: the archive context pointer.
static int __fastcall AiState_SerializeWithArchiveContext(TAiState* self, void* /*edx*/, uint32_t archiveArg)
{
    uint8_t* ctx = reinterpret_cast<uint8_t*>(archiveArg);

    // Compute the new flag: not already set, and the owner's slot-32 virtual says "not blocked".
    bool newFlag = false;
    if (CSF_AT(uint8_t, ctx, 0) == 0)
    {
        void* ownerObj = reinterpret_cast<void*>(CSF_AT(uint32_t, ctx, 4));
        uint8_t blocked = ((uint8_t(__fastcall*)(void*, void*, int, void*))CSF_AT(void**, ownerObj, 0)[32])(
            ownerObj, nullptr, 0, ctx);
        newFlag = (blocked == 0);
    }

    uint32_t savedLinkField = CSF_AT(uint32_t, ctx, 0x1c);
    CSF_AT(uint8_t, ctx, 0) = newFlag ? 1 : 0;
    AudioSoundManager_ProcessArchiveContext(ctx);

    uint8_t* selfSubObject = reinterpret_cast<uint8_t*>(self) + 4;
    g_ObjectReferenceManager = selfSubObject;
    archiveArg = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(selfSubObject));

    CSF_AT(uint32_t, ctx, 0x1c) = 0;
    if (CSF_AT(uint8_t, ctx, 0) == 0)
    {
        CSF_AT(uint8_t, ctx, 0x15) = 1;
        CSF_AT(uint8_t, ctx, 0x16) = 0;
        Archive_SerializeObjectRef(&archiveArg, reinterpret_cast<bool*>(ctx));
    }
    CSF_AT(uint32_t, ctx, 0x1c) = savedLinkField;

    if (CSF_AT(uint8_t, ctx, 0) == 0)
    {
        void* ownerObj = reinterpret_cast<void*>(CSF_AT(uint32_t, ctx, 4));
        uint8_t failed = ((uint8_t(__fastcall*)(void*, void*, void*))CSF_AT(void**, ownerObj, 0)[31])(
            ownerObj, nullptr, ctx);
        if (failed)
        {
            CSF_AT(uint8_t, ctx, 0) = 0;
            return 0;
        }
    }

    CSF_AT(uint8_t, ctx, 0) = 1;
    return 1;
}

HOOK(0x004DC0B0, AiState_SerializeWithArchiveContext, "auto");
