#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, C_008311e8_TriggerObjectSound, 0x0044D370, (int a1, int a2));
CSF_FUNC(int, __fastcall, C_008311e8_TriggerAttachmentSound, 0x0044D4D0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, GetUint16OrSentinelMinusOne, 0x006B80A0, (void* self, void*));
CSF_FUNC(int, __fastcall, ObjectAttachment_HasSoundTrigger, 0x006B8140, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectAttachment, 0x006B87A0, (int objectIndex));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void __fastcall C_008311e8_HandleSoundTriggerEvent(
    C_008311e8* self, void* /*edx*/, uint32_t* event)
{
    if (event[1] != 4)
        return;

    void* object = nullptr;
    if (static_cast<int32_t>(event[7]) >= 0)
    {
        object = reinterpret_cast<void*>(CHandleTable_Resolve(
            g_pCurrentScene, nullptr, reinterpret_cast<int>(event + 5)));
    }

    if (static_cast<int32_t>(event[4]) >= 0)
    {
        CHandleTable_Resolve(
            g_pCurrentScene, nullptr, reinterpret_cast<int>(event + 2));
    }

    int objectIndex;
    if (object)
    {
        if ((CSF_AT(uint32_t, object, 0x170) & 0x2000) != 0)
            return;

        void* model =
            reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, object, 0)[0x28 / 4])(object, nullptr);
        if (!model || event[26] == static_cast<uint32_t>(-1))
            return;

        void* entry =
            reinterpret_cast<void* (__fastcall*)(void*, void*, uint32_t)>(
                CSF_AT(void**, model, 0)[0x80 / 4])(
                    model, nullptr, event[26]);
        objectIndex = CSF_AT(int, entry, 0xC);
    }
    else
    {
        objectIndex = static_cast<int>(event[22]);
    }

    if (!objectIndex)
        return;

    void* attachment = reinterpret_cast<void*>(
        GfxObjectProps_GetObjectAttachment(objectIndex));
    if (!attachment)
        return;

    if (static_cast<uint8_t>(
            ObjectAttachment_HasSoundTrigger(attachment, nullptr)) == 0)
        return;

    if (object)
    {
        C_008311e8_TriggerObjectSound(
            reinterpret_cast<int>(object), static_cast<int>(event[26]));
    }
    else
    {
        int attachmentId = GetUint16OrSentinelMinusOne(attachment, nullptr);
        C_008311e8_TriggerAttachmentSound(self, nullptr, attachmentId);
    }
}

HOOK(0x0044D5A0, C_008311e8_HandleSoundTriggerEvent, "auto");
