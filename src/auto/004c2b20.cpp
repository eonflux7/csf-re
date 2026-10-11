#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, AudioSoundManager_ProcessArchiveContext, 0x00405450, (void* context));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));
CSF_FUNC(int, __fastcall, AiStateFactory_FindObjectReference, 0x004DB5F0, (void* self, void* edx, int a2));

CSF_GLOBAL(void*, g_ObjectReferenceRegistry, 0x008B0550);

static int __fastcall TFncAtacarDesdeParapeto_Serialize(
    TFncAtacarDesdeParapeto* self, void* /*edx*/, int archiveAddress)
{
    void* archive = reinterpret_cast<void*>(archiveAddress);
    const int savedContext = CSF_AT(int, archive, 0x1C);

    CSF_AT(uint8_t, archive, 0) =
        !CSF_AT(uint8_t, archive, 0) &&
        !((bool(__fastcall*)(void*, void*, int, void*))
            CSF_AT(void**, CSF_AT(void*, archive, 0x04), 0)[0x80 / 4])(
                CSF_AT(void*, archive, 0x04), nullptr, 0, archive);

    AudioSoundManager_ProcessArchiveContext(archive);

    const unsigned referenceOffsets[] = {
        0x04, 0x08, 0x0C, 0x10, 0x14, 0x18, 0x1C, 0x20, 0x24
    };
    alignas(8) uint8_t referenceBytes[0x04];

    for (unsigned offset : referenceOffsets)
    {
        uint8_t present = 0;
        if (CSF_AT(uint8_t, archive, 0x14) != 0)
        {
            present = static_cast<uint8_t>(AiStateFactory_FindObjectReference(
                g_ObjectReferenceRegistry, nullptr,
                reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + offset)));
        }

        const bool canSerializeFlag = CSF_AT(uint8_t, archive, 0) == 0;
        CSF_AT(int, archive, 0x1C) = 0;
        if (canSerializeFlag)
        {
            void* backend = CSF_AT(void*, archive, 0x04);
            CSF_AT(uint8_t, archive, 0x15) = 0;
            CSF_AT(uint8_t, archive, 0x16) = 0;
            CSF_AT(uint8_t, archive, 0) =
                ((int(__fastcall*)(void*, void*, uint8_t*, void*))
                    CSF_AT(void**, backend, 0)[0x44 / 4])(
                        backend, nullptr, &present, archive) == -1;
        }

        if (present != 0)
        {
            const bool canSerializeReference = CSF_AT(uint8_t, archive, 0) == 0;
            CSF_AT(void*, referenceBytes, 0) =
                reinterpret_cast<uint8_t*>(self) + offset;
            CSF_AT(int, archive, 0x1C) = 0;
            if (canSerializeReference)
            {
                CSF_AT(uint8_t, archive, 0x15) = 1;
                CSF_AT(uint8_t, archive, 0x16) = 0;
                Archive_SerializeObjectRef(
                    referenceBytes, reinterpret_cast<bool*>(archive));
            }
        }
    }

    const bool canFinish = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(int, archive, 0x1C) = savedContext;
    if (canFinish)
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        const int result =
            ((int(__fastcall*)(void*, void*, void*))
                CSF_AT(void**, backend, 0)[0x7C / 4])(
                    backend, nullptr, archive);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, archive, 0) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, archive, 0) = 1;
    return 1;
}

HOOK(0x004C2B20, TFncAtacarDesdeParapeto_Serialize, "auto");
