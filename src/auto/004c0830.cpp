#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, AudioSoundManager_ProcessArchiveContext, 0x00405450, (void* context));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));
CSF_FUNC(int, __fastcall, AiStateFactory_HasObjectReference, 0x004DB5F0, (void* self, void* edx, int a2));

CSF_GLOBAL(void*, g_ObjectReferenceManager, 0x008B0550);

static void TAlerta_SerializeOptionalReference(
    void* context, void* member, uint8_t& present, void* referenceStorage)
{
    present = 0;
    if (CSF_AT(uint8_t, context, 0x14) != 0)
    {
        present = static_cast<uint8_t>(AiStateFactory_HasObjectReference(
            g_ObjectReferenceManager, nullptr, reinterpret_cast<int>(member)));
    }

    bool failed = CSF_AT(uint8_t, context, 0) != 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* archive = CSF_AT(void*, context, 0x4);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint8_t*, void*)>(
                CSF_AT(void**, archive, 0)[0x44 / 4])(
                archive, nullptr, &present, context);
        CSF_AT(uint8_t, context, 0) = result == -1;
    }

    if (present != 0)
    {
        failed = CSF_AT(uint8_t, context, 0) != 0;
        *reinterpret_cast<void**>(referenceStorage) = member;
        CSF_AT(uint32_t, context, 0x1C) = 0;
        if (!failed)
        {
            CSF_AT(uint8_t, context, 0x15) = 1;
            CSF_AT(uint8_t, context, 0x16) = 0;
            Archive_SerializeObjectRef(
                referenceStorage, reinterpret_cast<bool*>(context));
        }
    }
}

static int __fastcall TAlerta_SerializeReferences(
    TAlerta* self, void* /*edx*/, int archiveContext)
{
    void* context = reinterpret_cast<void*>(archiveContext);
    uint32_t savedContext = CSF_AT(uint32_t, context, 0x1C);

    bool failed = false;
    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* archive = CSF_AT(void*, context, 0x4);
        failed =
            !reinterpret_cast<bool(__fastcall*)(void*, void*, int, void*)>(
                CSF_AT(void**, archive, 0)[0x80 / 4])(
                archive, nullptr, 0, context);
    }
    CSF_AT(uint8_t, context, 0) = failed;
    AudioSoundManager_ProcessArchiveContext(context);

    uint8_t present = 0;
    alignas(8) uint8_t referenceBytes[0x4];
    auto* members = reinterpret_cast<uint8_t*>(self);

    TAlerta_SerializeOptionalReference(context, members + 0x4, present, referenceBytes);
    TAlerta_SerializeOptionalReference(context, members + 0x18, present, referenceBytes);
    TAlerta_SerializeOptionalReference(context, members + 0x38, present, referenceBytes);
    TAlerta_SerializeOptionalReference(context, members + 0x5C, present, referenceBytes);
    TAlerta_SerializeOptionalReference(context, members + 0x70, present, referenceBytes);

    failed = CSF_AT(uint8_t, context, 0) != 0;
    CSF_AT(uint32_t, context, 0x1C) = savedContext;
    if (!failed)
    {
        void* archive = CSF_AT(void*, context, 0x4);
        int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, archive, 0)[0x7C / 4])(
                archive, nullptr, context);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, context, 0) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, context, 0) = 1;
    return 1;
}

HOOK(0x004C0830, TAlerta_SerializeReferences, "auto");
