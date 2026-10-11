#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, AudioSoundManager_ProcessArchiveContext, 0x00405450, (void* context));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));
CSF_FUNC(int, __fastcall, AiStateFactory_HasObjectReference, 0x004DB5F0, (void* self, void* edx, int a2));

CSF_GLOBAL(void*, g_ObjectReferenceRegistry, 0x008B0550);

static void AiStateFactory_SerializeOptionalReference(void* member, void* context)
{
    uint8_t present = 0;
    if (CSF_AT(uint8_t, context, 0x14) != 0)
    {
        present = static_cast<uint8_t>(
            AiStateFactory_HasObjectReference(
                g_ObjectReferenceRegistry, nullptr, reinterpret_cast<int>(member)));
    }

    const bool failed = CSF_AT(uint8_t, context, 0) != 0;
    CSF_AT(int, context, 0x1C) = 0;
    if (!failed)
    {
        void* stream = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            ((int(__fastcall*)(void*, void*, uint8_t*, void*))
                CSF_AT(void**, stream, 0)[0x44 / 4])(
                stream, nullptr, &present, context) == -1;
    }

    if (present != 0)
    {
        const bool referenceFailed = CSF_AT(uint8_t, context, 0) != 0;
        alignas(8) uint8_t referenceBytes[0x4];
        *reinterpret_cast<void**>(referenceBytes) = member;
        CSF_AT(int, context, 0x1C) = 0;
        if (!referenceFailed)
        {
            CSF_AT(uint8_t, context, 0x15) = 1;
            CSF_AT(uint8_t, context, 0x16) = 0;
            Archive_SerializeObjectRef(
                referenceBytes, reinterpret_cast<bool*>(context));
        }
    }
}

static int __fastcall TFncBusqueda_Serialize(
    TFncBusqueda* self, void* /*edx*/, int archive)
{
    void* context = reinterpret_cast<void*>(archive);
    const int savedContext = CSF_AT(int, context, 0x1C);

    CSF_AT(uint8_t, context, 0) =
        CSF_AT(uint8_t, context, 0) == 0 &&
        !((bool(__fastcall*)(void*, void*, int, void*))
            CSF_AT(void**, CSF_AT(void*, context, 0x04), 0)[0x80 / 4])(
            CSF_AT(void*, context, 0x04), nullptr, 0, context);

    AudioSoundManager_ProcessArchiveContext(context);

    AiStateFactory_SerializeOptionalReference(
        reinterpret_cast<uint8_t*>(self) + 0x04, context);
    AiStateFactory_SerializeOptionalReference(
        reinterpret_cast<uint8_t*>(self) + 0x08, context);
    AiStateFactory_SerializeOptionalReference(
        reinterpret_cast<uint8_t*>(self) + 0x0C, context);

    bool failed = CSF_AT(uint8_t, context, 0) != 0;
    CSF_AT(int, context, 0x1C) = 0;
    if (!failed)
    {
        void* stream = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            ((int(__fastcall*)(void*, void*, uint32_t*, void*))
                CSF_AT(void**, stream, 0)[0x38 / 4])(
                stream, nullptr, &self->field_0010, context) == -1;
    }

    failed = CSF_AT(uint8_t, context, 0) != 0;
    CSF_AT(int, context, 0x1C) = 0;
    if (!failed)
    {
        void* stream = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            ((int(__fastcall*)(void*, void*, uint32_t*, void*))
                CSF_AT(void**, stream, 0)[0x48 / 4])(
                stream, nullptr, &self->field_001c, context) == -1;
    }

    failed = CSF_AT(uint8_t, context, 0) != 0;
    CSF_AT(int, context, 0x1C) = savedContext;
    if (!failed)
    {
        void* stream = CSF_AT(void*, context, 0x04);
        const int result =
            ((int(__fastcall*)(void*, void*, void*))
                CSF_AT(void**, stream, 0)[0x7C / 4])(
                stream, nullptr, context);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, context, 0) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, context, 0) = 1;
    return 1;
}

HOOK(0x004C1FE0, TFncBusqueda_Serialize, "auto");
