#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, AudioSoundManager_ProcessArchiveContext, 0x00405450, (void* context));
CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int archiveAddress, S_00691860* string));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));
CSF_FUNC(int, __fastcall, AiStateFactory_QueryObjectReference, 0x004DB5F0, (void* self, void* edx, int a2));

CSF_GLOBAL(void*, g_ObjectReferenceManager, 0x008B0550);

static int __fastcall TFncInteractivo_Serialize(
    TFncInteractivo* self, void* /*edx*/, int archiveAddress)
{
    void* archive = reinterpret_cast<void*>(archiveAddress);
    const uint32_t savedContext = CSF_AT(uint32_t, archive, 0x1C);

    CSF_AT(uint8_t, archive, 0x00) =
        !CSF_AT(uint8_t, archive, 0x00) &&
        !((bool(__fastcall*)(void*, void*, int, int))
            CSF_AT(void**, CSF_AT(void*, archive, 0x04), 0)[128 / 4])(
                CSF_AT(void*, archive, 0x04), nullptr, 0, archiveAddress);

    AudioSoundManager_ProcessArchiveContext(archive);

    alignas(8) uint8_t referenceBytes[0x4];

    for (uint32_t offset = 0x04; offset <= 0x10; offset += 4)
    {
        uint8_t present = 0;
        if (CSF_AT(uint8_t, archive, 0x14))
        {
            present = static_cast<uint8_t>(
                AiStateFactory_QueryObjectReference(
                    g_ObjectReferenceManager, nullptr,
                    reinterpret_cast<int>(
                        reinterpret_cast<uint8_t*>(self) + offset)));
        }

        bool failed = CSF_AT(uint8_t, archive, 0x00) != 0;
        CSF_AT(uint32_t, archive, 0x1C) = 0;
        if (!failed)
        {
            void* stream = CSF_AT(void*, archive, 0x04);
            CSF_AT(uint8_t, archive, 0x15) = 0;
            CSF_AT(uint8_t, archive, 0x16) = 0;
            CSF_AT(uint8_t, archive, 0x00) =
                ((int(__fastcall*)(void*, void*, char*, int))
                    CSF_AT(void**, stream, 0)[68 / 4])(
                        stream, nullptr,
                        reinterpret_cast<char*>(&present), archiveAddress) == -1;
        }

        if (present)
        {
            failed = CSF_AT(uint8_t, archive, 0x00) != 0;
            CSF_AT(void*, referenceBytes, 0) =
                reinterpret_cast<uint8_t*>(self) + offset;
            CSF_AT(uint32_t, archive, 0x1C) = 0;
            if (!failed)
            {
                CSF_AT(uint8_t, archive, 0x15) = 1;
                CSF_AT(uint8_t, archive, 0x16) = 0;
                Archive_SerializeObjectRef(
                    referenceBytes, reinterpret_cast<bool*>(archive));
            }
        }
    }

    bool failed = CSF_AT(uint8_t, archive, 0x00) != 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (!failed)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0x00) =
            ((int(__fastcall*)(void*, void*, uint32_t*, int))
                CSF_AT(void**, stream, 0)[56 / 4])(
                    stream, nullptr, &self->field_0020, archiveAddress) == -1;
    }

    failed = CSF_AT(uint8_t, archive, 0x00) != 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (!failed)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeString(
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x14),
            reinterpret_cast<S_00691860*>(archive));
    }

    CSF_AT(uint32_t, archive, 0x1C) = savedContext;
    if (CSF_AT(uint8_t, archive, 0x00))
    {
        CSF_AT(uint8_t, archive, 0x00) = 1;
        return 1;
    }

    void* stream = CSF_AT(void*, archive, 0x04);
    const int result =
        ((int(__fastcall*)(void*, void*, int))
            CSF_AT(void**, stream, 0)[124 / 4])(
                stream, nullptr, archiveAddress);

    if (static_cast<uint8_t>(result) == 0)
    {
        CSF_AT(uint8_t, archive, 0x00) = 1;
        return 1;
    }

    CSF_AT(uint8_t, archive, 0x00) = 0;
    return result;
}

HOOK(0x004C31B0, TFncInteractivo_Serialize, "auto");
