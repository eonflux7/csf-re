#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, ObjectAttachment_HasSoundTrigger, 0x006B8140, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetReferencedField14, 0x006B8270, (void* self, void*));
CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectAttachment, 0x006B87A0, (int objectIndex));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_CloseTargetDistanceSquared, 0x00831148);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static bool __stdcall WeaponFirearms_ProcessTraceHit(
    void* record, uint32_t* traceHit, int handle)
{
    bool result = false;

    if (!traceHit[9])
    {
        void* manager = CSF_AT(
            void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FD4);
        void* material =
            ((void* (__fastcall*)(void*, void*, uint32_t))
                CSF_AT(void**, manager, 0)[8 / 4])(
                    manager, nullptr, traceHit[7]);

        uint32_t flags = CSF_AT(uint32_t, material, 0x20);
        result = (flags & 1) == 1;
        if ((flags & 8) != 8)
            return result;
    }

    CSF_AT(uint32_t, record, 0x58) = traceHit[9];
    CSF_AT(uint32_t, record, 0x5C) = traceHit[7];
    CSF_AT(uint32_t, record, 0x34) = traceHit[1];
    CSF_AT(uint32_t, record, 0x38) = traceHit[2];
    CSF_AT(uint32_t, record, 0x3C) = traceHit[3];
    CSF_AT(uint32_t, record, 0x40) = traceHit[4];
    CSF_AT(uint32_t, record, 0x44) = traceHit[5];
    CSF_AT(uint32_t, record, 0x48) = traceHit[6];

    bool shouldDispatch = true;
    void* handleData = reinterpret_cast<void*>(handle);
    if (CSF_AT(int, handleData, 0x08) >= 0)
    {
        int handleType = CSF_AT(int, handleData, 0x0C);
        if (handleType == 1 || handleType == 2)
        {
            auto* object = reinterpret_cast<C_008342e8*>(
                CHandleTable_Resolve(g_pCurrentScene, nullptr, handle));
            if (object && object->field_0050)
            {
                float* position = GetObjectPositionVector(object, nullptr);
                float dx = position[0] - CSF_AT(float, record, 0x34);
                float dy = position[1] - CSF_AT(float, record, 0x38);
                float dz = position[2] - CSF_AT(float, record, 0x3C);
                if (dz * dz + dy * dy + dx * dx < g_CloseTargetDistanceSquared)
                    shouldDispatch = false;
            }
        }
    }

    bool soundTrigger = false;
    if (CSF_AT(int, record, 0x58))
    {
        void* attachment = reinterpret_cast<void*>(
            GfxObjectProps_GetObjectAttachment(CSF_AT(int, record, 0x58)));
        int attachmentType = GetReferencedField14(attachment, nullptr);
        CSF_AT(uint8_t, record, 0x70) |=
            static_cast<uint8_t>(attachmentType == 1 || attachmentType == 2);

        attachment = reinterpret_cast<void*>(
            GfxObjectProps_GetObjectAttachment(static_cast<int>(traceHit[9])));
        soundTrigger = static_cast<uint8_t>(
            ObjectAttachment_HasSoundTrigger(attachment, nullptr)) != 0;
    }

    if (soundTrigger)
    {
        result = false;
        void* manager = CSF_AT(
            void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FD4);
        void* globalState = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
        uint32_t materialId =
            ((uint32_t (__fastcall*)(void*, void*, uint32_t))
                CSF_AT(void**, manager, 0)[0x10 / 4])(
                    manager, nullptr, CSF_AT(uint32_t, globalState, 0x908));
        CSF_AT(uint32_t, record, 0x5C) = materialId;
    }
    else if (!shouldDispatch)
    {
        return result;
    }

    void* dispatcher = reinterpret_cast<void*>(GetGlobalObjectField184());
    DispatchRecordToGlobalObject(dispatcher, nullptr, record, 1);
    return result;
}

HOOK(0x0048ABB0, WeaponFirearms_ProcessTraceHit, "auto");
