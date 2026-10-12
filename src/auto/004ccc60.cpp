#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, AudioSoundManager_ProcessArchiveContext, 0x00405450, (void* context));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));
CSF_FUNC(int, __fastcall, CoverTask_ProcessRecord, 0x004C9B10, (S_0041e5f0* self, void* , char* record));
CSF_FUNC(int, __fastcall, AiStateFactory_HasObjectReference, 0x004DB5F0, (void* self, void* edx, int a2));

CSF_GLOBAL(void*, g_ObjectReferenceManager, 0x008B0550);

static int __fastcall TFncMacroOrden_SerializeArchive(
    TFncMacroOrden* self, void* /*edx*/, uint8_t* record)
{
    const uint32_t savedContext = CSF_AT(uint32_t, record, 0x1C);
    bool beginFailed = false;
    if (CSF_AT(uint8_t, record, 0x00) == 0)
    {
        void* archive = CSF_AT(void*, record, 0x04);
        beginFailed =
            !((bool(__fastcall*)(void*, void*, int, uint8_t*))
                  CSF_AT(void**, archive, 0)[0x80 / 4])(
                archive, nullptr, 0, record);
    }
    CSF_AT(uint8_t, record, 0x00) = beginFailed;
    AudioSoundManager_ProcessArchiveContext(record);

    uint8_t hasReference = 0;
    if (CSF_AT(uint8_t, record, 0x14) != 0)
    {
        hasReference = static_cast<uint8_t>(
            AiStateFactory_HasObjectReference(
                g_ObjectReferenceManager, nullptr,
                reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x04)));
    }

    bool processField = CSF_AT(uint8_t, record, 0x00) == 0;
    CSF_AT(uint32_t, record, 0x1C) = 0;
    if (processField)
    {
        void* archive = CSF_AT(void*, record, 0x04);
        CSF_AT(uint8_t, record, 0x15) = 0;
        CSF_AT(uint8_t, record, 0x16) = 0;
        CSF_AT(uint8_t, record, 0x00) =
            ((int(__fastcall*)(void*, void*, char*, uint8_t*))
                 CSF_AT(void**, archive, 0)[0x44 / 4])(
                archive, nullptr, reinterpret_cast<char*>(&hasReference), record) == -1;
    }

    alignas(8) uint8_t referenceBytes[0x04];
    if (hasReference != 0)
    {
        processField = CSF_AT(uint8_t, record, 0x00) == 0;
        *reinterpret_cast<void**>(referenceBytes) =
            reinterpret_cast<uint8_t*>(self) + 0x04;
        CSF_AT(uint32_t, record, 0x1C) = 0;
        if (processField)
        {
            CSF_AT(uint8_t, record, 0x15) = 1;
            CSF_AT(uint8_t, record, 0x16) = 0;
            Archive_SerializeObjectRef(referenceBytes, reinterpret_cast<bool*>(record));
        }
    }

    hasReference = 0;
    if (CSF_AT(uint8_t, record, 0x14) != 0)
    {
        hasReference = static_cast<uint8_t>(
            AiStateFactory_HasObjectReference(
                g_ObjectReferenceManager, nullptr,
                reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x08)));
    }

    processField = CSF_AT(uint8_t, record, 0x00) == 0;
    CSF_AT(uint32_t, record, 0x1C) = 0;
    if (processField)
    {
        void* archive = CSF_AT(void*, record, 0x04);
        CSF_AT(uint8_t, record, 0x15) = 0;
        CSF_AT(uint8_t, record, 0x16) = 0;
        CSF_AT(uint8_t, record, 0x00) =
            ((int(__fastcall*)(void*, void*, char*, uint8_t*))
                 CSF_AT(void**, archive, 0)[0x44 / 4])(
                archive, nullptr, reinterpret_cast<char*>(&hasReference), record) == -1;
    }

    if (hasReference != 0)
    {
        processField = CSF_AT(uint8_t, record, 0x00) == 0;
        *reinterpret_cast<void**>(referenceBytes) =
            reinterpret_cast<uint8_t*>(self) + 0x08;
        CSF_AT(uint32_t, record, 0x1C) = 0;
        if (processField)
        {
            CSF_AT(uint8_t, record, 0x15) = 1;
            CSF_AT(uint8_t, record, 0x16) = 0;
            Archive_SerializeObjectRef(referenceBytes, reinterpret_cast<bool*>(record));
        }
    }

    processField = CSF_AT(uint8_t, record, 0x00) == 0;
    CSF_AT(uint32_t, record, 0x1C) = 0;
    if (processField)
    {
        CSF_AT(uint8_t, record, 0x15) = 0;
        CSF_AT(uint8_t, record, 0x16) = 0;
        CoverTask_ProcessRecord(
            reinterpret_cast<S_0041e5f0*>(reinterpret_cast<uint8_t*>(self) + 0xCC),
            nullptr, reinterpret_cast<char*>(record));
    }

    CSF_AT(uint32_t, record, 0x1C) = savedContext;
    if (CSF_AT(uint8_t, record, 0x00) == 0)
    {
        void* archive = CSF_AT(void*, record, 0x04);
        const int result =
            ((int(__fastcall*)(void*, void*, uint8_t*))
                 CSF_AT(void**, archive, 0)[0x7C / 4])(
                archive, nullptr, record);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, record, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, record, 0x00) = 1;
    return 1;
}

HOOK(0x004CCC60, TFncMacroOrden_SerializeArchive, "auto");
