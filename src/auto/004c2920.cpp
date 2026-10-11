#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, AudioSoundManager_ProcessArchiveContext, 0x00405450, (void* context));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));
CSF_FUNC(int, __fastcall, AiStateFactory_IsReferencePresent, 0x004DB5F0, (void* self, void* edx, int a2));

CSF_GLOBAL(void*, g_ArchiveReferenceManager, 0x008B0550);

static int __fastcall TMovilAParapeto_SerializeReferences(
    TMovilAParapeto* self, void* /*edx*/, void* context)
{
    const uint32_t savedContext = CSF_AT(uint32_t, context, 0x1C);

    bool failedToBegin = false;
    if (CSF_AT(uint8_t, context, 0x00) == 0)
    {
        void* archive = CSF_AT(void*, context, 0x04);
        failedToBegin =
            !((bool(__fastcall*)(void*, void*, int, void*))
                  CSF_AT(void**, archive, 0)[0x80 / 4])(
                archive, nullptr, 0, context);
    }
    CSF_AT(uint8_t, context, 0x00) = failedToBegin;
    AudioSoundManager_ProcessArchiveContext(context);

    static constexpr std::size_t referenceOffsets[] = {
        0x04, 0x08, 0x0C, 0x10
    };
    alignas(8) uint8_t referenceBytes[0x04];

    for (std::size_t offset : referenceOffsets)
    {
        uint8_t present = 0;
        if (CSF_AT(uint8_t, context, 0x14) != 0)
        {
            present = static_cast<uint8_t>(
                AiStateFactory_IsReferencePresent(
                    g_ArchiveReferenceManager, nullptr,
                    reinterpret_cast<int>(
                        reinterpret_cast<uint8_t*>(self) + offset)));
        }

        const bool alreadyFailed = CSF_AT(uint8_t, context, 0x00) != 0;
        CSF_AT(uint32_t, context, 0x1C) = 0;
        if (!alreadyFailed)
        {
            void* archive = CSF_AT(void*, context, 0x04);
            CSF_AT(uint8_t, context, 0x15) = 0;
            CSF_AT(uint8_t, context, 0x16) = 0;
            const int result =
                ((int(__fastcall*)(void*, void*, uint8_t*, void*))
                     CSF_AT(void**, archive, 0)[0x44 / 4])(
                    archive, nullptr, &present, context);
            CSF_AT(uint8_t, context, 0x00) = result == -1;
        }

        if (present != 0)
        {
            const bool referenceFailed =
                CSF_AT(uint8_t, context, 0x00) != 0;
            *reinterpret_cast<void**>(referenceBytes) =
                reinterpret_cast<uint8_t*>(self) + offset;
            CSF_AT(uint32_t, context, 0x1C) = 0;
            if (!referenceFailed)
            {
                CSF_AT(uint8_t, context, 0x15) = 1;
                CSF_AT(uint8_t, context, 0x16) = 0;
                Archive_SerializeObjectRef(
                    referenceBytes, reinterpret_cast<bool*>(context));
            }
        }
    }

    const bool failed = CSF_AT(uint8_t, context, 0x00) != 0;
    CSF_AT(uint32_t, context, 0x1C) = savedContext;
    if (!failed)
    {
        void* archive = CSF_AT(void*, context, 0x04);
        const bool result =
            ((bool(__fastcall*)(void*, void*, void*))
                 CSF_AT(void**, archive, 0)[0x7C / 4])(
                archive, nullptr, context);
        if (result)
        {
            CSF_AT(uint8_t, context, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, context, 0x00) = 1;
    return 1;
}

HOOK(0x004C2920, TMovilAParapeto_SerializeReferences, "auto");
