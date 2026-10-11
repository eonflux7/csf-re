#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CharActionFsm_GetProviderFrameObject, 0x006C0500, (int a1));
CSF_FUNC(int, __cdecl, SceneLoader_GetObjectAttachment, 0x006B87A0, (int a1));
CSF_FUNC(int, __fastcall, GetUint16OrSentinelMinusOne, 0x006B80A0, (void* self, void*));

// Resolves the attachment of a char action's provider frame object and compares its
// 16-bit id against the first dword of the match slot. On a match, stores the char
// action in the slot's second dword and returns 0; otherwise returns the char action.
static int __cdecl FindMatchingAttachmentRecord(int charAction, uint32_t* matchSlot)
{
    int providerFrameObject = CharActionFsm_GetProviderFrameObject(charAction);
    C_0085ec18* objectAttachment =
        reinterpret_cast<C_0085ec18*>(SceneLoader_GetObjectAttachment(providerFrameObject));

    if (GetUint16OrSentinelMinusOne(objectAttachment, nullptr) != static_cast<int>(matchSlot[0]))
        return charAction;

    matchSlot[1] = static_cast<uint32_t>(charAction);
    return 0;
}

HOOK(0x006CA930, FindMatchingAttachmentRecord, "auto");
