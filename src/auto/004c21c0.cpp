#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, AudioSoundManager_ProcessArchiveContext, 0x00405450, (void* context));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));
CSF_FUNC(int, __fastcall, AiStateFactory_HasObjectReference, 0x004DB5F0, (void* self, void* edx, int a2));

CSF_GLOBAL(void*, g_AiObjectReferenceRegistry, 0x008B0550);

static int __fastcall TMovilAPunto_Serialize(TMovilAPunto* self, void* /*edx*/, void* context)
{
    const uint32_t savedContext = CSF_AT(uint32_t, context, 0x1C);

    bool failed = false;
    if (CSF_AT(uint8_t, context, 0) == 0) {
        void* archive = CSF_AT(void*, context, 0x4);
        failed = !((bool(__fastcall*)(void*, void*, int, void*))
            CSF_AT(void**, archive, 0)[0x80 / 4])(archive, nullptr, 0, context);
    }
    CSF_AT(uint8_t, context, 0) = failed;
    AudioSoundManager_ProcessArchiveContext(context);

    uint8_t* referenceFields[] = {
        self->pad_0004,
        self->pad_0004 + 4,
        self->pad_0004 + 8
    };
    alignas(8) uint8_t referenceBytes[0x4];

    for (uint8_t* referenceField : referenceFields) {
        uint8_t hasReference = 0;
        if (CSF_AT(uint8_t, context, 0x14) != 0) {
            hasReference = static_cast<uint8_t>(
                AiStateFactory_HasObjectReference(
                    g_AiObjectReferenceRegistry, nullptr,
                    reinterpret_cast<int>(referenceField)));
        }

        bool canSerialize = CSF_AT(uint8_t, context, 0) == 0;
        CSF_AT(uint32_t, context, 0x1C) = 0;
        if (canSerialize) {
            void* archive = CSF_AT(void*, context, 0x4);
            CSF_AT(uint8_t, context, 0x15) = 0;
            CSF_AT(uint8_t, context, 0x16) = 0;
            const int result = ((int(__fastcall*)(void*, void*, uint8_t*, void*))
                CSF_AT(void**, archive, 0)[0x44 / 4])(
                    archive, nullptr, &hasReference, context);
            CSF_AT(uint8_t, context, 0) = result == -1;
        }

        if (hasReference != 0) {
            canSerialize = CSF_AT(uint8_t, context, 0) == 0;
            CSF_AT(void*, referenceBytes, 0) = referenceField;
            CSF_AT(uint32_t, context, 0x1C) = 0;
            if (canSerialize) {
                CSF_AT(uint8_t, context, 0x15) = 1;
                CSF_AT(uint8_t, context, 0x16) = 0;
                Archive_SerializeObjectRef(referenceBytes, reinterpret_cast<bool*>(context));
            }
        }
    }

    bool canSerialize = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (canSerialize) {
        void* archive = CSF_AT(void*, context, 0x4);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        const int result = ((int(__fastcall*)(void*, void*, uint32_t*, void*))
            CSF_AT(void**, archive, 0)[0x38 / 4])(
                archive, nullptr, &self->field_0010, context);
        CSF_AT(uint8_t, context, 0) = result == -1;
    }

    canSerialize = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = savedContext;
    if (canSerialize) {
        void* archive = CSF_AT(void*, context, 0x4);
        const bool result = ((bool(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, archive, 0)[0x7C / 4])(archive, nullptr, context);
        if (result) {
            CSF_AT(uint8_t, context, 0) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, context, 0) = 1;
    return 1;
}

HOOK(0x004C21C0, TMovilAPunto_Serialize, "auto");
