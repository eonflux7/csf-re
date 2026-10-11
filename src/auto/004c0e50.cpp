#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, AudioSoundManager_ProcessArchiveContext, 0x00405450, (void* context));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));
CSF_FUNC(int, __fastcall, AiStateFactory_IsObjectReferenceRegistered, 0x004DB5F0, (void* self, void* edx, int a2));

CSF_GLOBAL(void*, g_ObjectReferenceRegistry, 0x008B0550);

static void AiStateFactory_SerializeOptionalObjectReference(void* member, void* context)
{
    uint8_t present = 0;
    if (CSF_AT(uint8_t, context, 0x14) != 0)
    {
        present = static_cast<uint8_t>(
            AiStateFactory_IsObjectReferenceRegistered(
                g_ObjectReferenceRegistry, nullptr, reinterpret_cast<int>(member)));
    }

    const bool failed = CSF_AT(uint8_t, context, 0) != 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* archive = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            ((int(__fastcall*)(void*, void*, uint8_t*, void*))
                CSF_AT(void**, archive, 0)[0x44 / 4])(
                    archive, nullptr, &present, context) == -1;
    }

    if (present != 0)
    {
        const bool referenceFailed = CSF_AT(uint8_t, context, 0) != 0;
        alignas(8) uint8_t referenceBytes[0x04];
        CSF_AT(void*, referenceBytes, 0) = member;
        CSF_AT(uint32_t, context, 0x1C) = 0;
        if (!referenceFailed)
        {
            CSF_AT(uint8_t, context, 0x15) = 1;
            CSF_AT(uint8_t, context, 0x16) = 0;
            Archive_SerializeObjectRef(referenceBytes, reinterpret_cast<bool*>(context));
        }
    }
}

static int __fastcall TMovilAPuntoRastreo_Serialize(
    TMovilAPuntoRastreo* self, void* /*edx*/, void* context)
{
    const uint32_t savedContext = CSF_AT(uint32_t, context, 0x1C);

    bool failed = false;
    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* archive = CSF_AT(void*, context, 0x04);
        failed = !((bool(__fastcall*)(void*, void*, int, void*))
            CSF_AT(void**, archive, 0)[0x80 / 4])(
                archive, nullptr, 0, context);
    }
    CSF_AT(uint8_t, context, 0) = failed;
    AudioSoundManager_ProcessArchiveContext(context);

    AiStateFactory_SerializeOptionalObjectReference(
        &CSF_AT(uint32_t, self, 0x04), context);
    AiStateFactory_SerializeOptionalObjectReference(
        &CSF_AT(uint32_t, self, 0x08), context);
    AiStateFactory_SerializeOptionalObjectReference(
        &CSF_AT(uint32_t, self, 0x0C), context);
    AiStateFactory_SerializeOptionalObjectReference(
        &CSF_AT(uint32_t, self, 0x10), context);
    AiStateFactory_SerializeOptionalObjectReference(
        &CSF_AT(uint32_t, self, 0x14), context);

    failed = CSF_AT(uint8_t, context, 0) != 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* archive = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            ((int(__fastcall*)(void*, void*, uint32_t*, void*))
                CSF_AT(void**, archive, 0)[0x38 / 4])(
                    archive, nullptr, &self->field_0018, context) == -1;
    }

    failed = CSF_AT(uint8_t, context, 0) != 0;
    CSF_AT(uint32_t, context, 0x1C) = savedContext;
    if (!failed)
    {
        void* archive = CSF_AT(void*, context, 0x04);
        const int result = ((int(__fastcall*)(void*, void*, void*))
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

HOOK(0x004C0E50, TMovilAPuntoRastreo_Serialize, "auto");
