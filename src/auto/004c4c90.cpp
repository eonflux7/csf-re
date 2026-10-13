#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, AudioSoundManager_ProcessArchiveContext, 0x00405450, (void* context));
CSF_FUNC(unsigned int, __stdcall, AiTaskStates_ResetSoundSlotFlags, 0x0041F6C0, (uint32_t* a1, char* a2));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));
CSF_FUNC(int, __fastcall, AiStateFactory_HasObjectReference, 0x004DB5F0, (void* self, void* edx, int a2));

CSF_GLOBAL(void*, g_ObjectReferenceManager, 0x008B0550);

static int __fastcall TFncIrAParapeto_Serialize(
    TFncIrAParapeto* self, void* /*edx*/, void* context)
{
    const uint32_t savedPosition = CSF_AT(uint32_t, context, 0x1C);
    auto* objectBytes = reinterpret_cast<uint8_t*>(self);
    alignas(8) uint8_t referenceBytes[0x4];
    uint8_t hasReference = 0;

    bool failed = false;
    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* archive = CSF_AT(void*, context, 0x4);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, void*)>(
            CSF_AT(void**, archive, 0)[0x80 / 4])(
                archive, nullptr, 0, context);
    }
    CSF_AT(uint8_t, context, 0) = failed;
    AudioSoundManager_ProcessArchiveContext(context);

    if (CSF_AT(uint8_t, context, 0x14) != 0)
    {
        hasReference = static_cast<uint8_t>(
            AiStateFactory_HasObjectReference(
                g_ObjectReferenceManager, nullptr,
                reinterpret_cast<int>(objectBytes + 0x4)));
    }

    failed = CSF_AT(uint8_t, context, 0) != 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* archive = CSF_AT(void*, context, 0x4);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint8_t*, void*)>(
                CSF_AT(void**, archive, 0)[0x44 / 4])(
                    archive, nullptr, &hasReference, context) == -1;
    }

    if (hasReference != 0)
    {
        failed = CSF_AT(uint8_t, context, 0) != 0;
        CSF_AT(void*, referenceBytes, 0) = objectBytes + 0x4;
        CSF_AT(uint32_t, context, 0x1C) = 0;
        if (!failed)
        {
            CSF_AT(uint8_t, context, 0x15) = 1;
            CSF_AT(uint8_t, context, 0x16) = 0;
            Archive_SerializeObjectRef(
                referenceBytes, reinterpret_cast<bool*>(context));
        }
    }

    hasReference = 0;
    if (CSF_AT(uint8_t, context, 0x14) != 0)
    {
        hasReference = static_cast<uint8_t>(
            AiStateFactory_HasObjectReference(
                g_ObjectReferenceManager, nullptr,
                reinterpret_cast<int>(objectBytes + 0x8)));
    }

    failed = CSF_AT(uint8_t, context, 0) != 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* archive = CSF_AT(void*, context, 0x4);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint8_t*, void*)>(
                CSF_AT(void**, archive, 0)[0x44 / 4])(
                    archive, nullptr, &hasReference, context) == -1;
    }

    if (hasReference != 0)
    {
        failed = CSF_AT(uint8_t, context, 0) != 0;
        CSF_AT(void*, referenceBytes, 0) = objectBytes + 0x8;
        CSF_AT(uint32_t, context, 0x1C) = 0;
        if (!failed)
        {
            CSF_AT(uint8_t, context, 0x15) = 1;
            CSF_AT(uint8_t, context, 0x16) = 0;
            Archive_SerializeObjectRef(
                referenceBytes, reinterpret_cast<bool*>(context));
        }
    }

    failed = CSF_AT(uint8_t, context, 0) != 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        AiTaskStates_ResetSoundSlotFlags(
            &self->field_000c, reinterpret_cast<char*>(context));
    }

    failed = CSF_AT(uint8_t, context, 0) != 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* archive = CSF_AT(void*, context, 0x4);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, void*)>(
                CSF_AT(void**, archive, 0)[0x38 / 4])(
                    archive, nullptr, &self->field_001c, context) == -1;
    }

    failed = CSF_AT(uint8_t, context, 0) != 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* archive = CSF_AT(void*, context, 0x4);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint8_t*, void*)>(
                CSF_AT(void**, archive, 0)[0x44 / 4])(
                    archive, nullptr, objectBytes + 0x28, context) == -1;
    }

    CSF_AT(uint32_t, context, 0x1C) = savedPosition;
    if (CSF_AT(uint8_t, context, 0) != 0)
    {
        CSF_AT(uint8_t, context, 0) = 1;
        return 1;
    }

    void* archive = CSF_AT(void*, context, 0x4);
    const int result =
        reinterpret_cast<int(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, archive, 0)[0x7C / 4])(
                archive, nullptr, context);
    if (static_cast<uint8_t>(result) == 0)
    {
        CSF_AT(uint8_t, context, 0) = 1;
        return 1;
    }

    CSF_AT(uint8_t, context, 0) = 0;
    return result;
}

HOOK(0x004C4C90, TFncIrAParapeto_Serialize, "auto");
