#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, AudioSoundManager_ProcessArchiveContext, 0x00405450, (void* context));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));
CSF_FUNC(int, __fastcall, Archive_IsObjectReferencePresent, 0x004DB5F0, (void* self, void* edx, int a2));

CSF_GLOBAL(void*, g_ArchiveReferenceManager, 0x008B0550);

static void __cdecl Archive_SerializeOptionalReference(uint8_t* referenceField, void* context)
{
    uint8_t present = 0;
    if (CSF_AT(uint8_t, context, 0x14) != 0)
    {
        present = static_cast<uint8_t>(Archive_IsObjectReferencePresent(
            g_ArchiveReferenceManager, nullptr, reinterpret_cast<int>(referenceField)));
    }

    bool failed = CSF_AT(uint8_t, context, 0) != 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* archive = CSF_AT(void*, context, 4);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint8_t*, void*)>(
                CSF_AT(void**, archive, 0)[0x44 / 4])(
                    archive, nullptr, &present, context) == -1;
    }

    if (present != 0)
    {
        failed = CSF_AT(uint8_t, context, 0) != 0;
        alignas(8) uint8_t referenceBytes[0x4];
        CSF_AT(uint8_t*, referenceBytes, 0) = referenceField;
        CSF_AT(uint32_t, context, 0x1C) = 0;
        if (!failed)
        {
            CSF_AT(uint8_t, context, 0x15) = 1;
            CSF_AT(uint8_t, context, 0x16) = 0;
            Archive_SerializeObjectRef(referenceBytes, reinterpret_cast<bool*>(context));
        }
    }
}

static int __fastcall TMovilAPuntoParapetandose_Serialize(
    TMovilAPuntoParapetandose* self, void* /*edx*/, int contextAddress)
{
    void* context = reinterpret_cast<void*>(contextAddress);
    const uint32_t savedContext = CSF_AT(uint32_t, context, 0x1C);

    bool failed = false;
    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* archive = CSF_AT(void*, context, 4);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, void*)>(
            CSF_AT(void**, archive, 0)[0x80 / 4])(
                archive, nullptr, 0, context);
    }
    CSF_AT(uint8_t, context, 0) = failed;
    AudioSoundManager_ProcessArchiveContext(context);

    Archive_SerializeOptionalReference(reinterpret_cast<uint8_t*>(self) + 0x4, context);
    Archive_SerializeOptionalReference(reinterpret_cast<uint8_t*>(self) + 0x8, context);
    Archive_SerializeOptionalReference(reinterpret_cast<uint8_t*>(self) + 0xC, context);
    Archive_SerializeOptionalReference(reinterpret_cast<uint8_t*>(self) + 0x14, context);

    failed = CSF_AT(uint8_t, context, 0) != 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* archive = CSF_AT(void*, context, 4);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, void*)>(
                CSF_AT(void**, archive, 0)[0x38 / 4])(
                    archive, nullptr, &CSF_AT(uint32_t, self, 0x18), context) == -1;
    }

    failed = CSF_AT(uint8_t, context, 0) != 0;
    CSF_AT(uint32_t, context, 0x1C) = savedContext;
    if (!failed)
    {
        void* archive = CSF_AT(void*, context, 4);
        const int result =
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

HOOK(0x004C19F0, TMovilAPuntoParapetandose_Serialize, "auto");
