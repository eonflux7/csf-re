#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __stdcall, AudioSoundManager_ProcessArchiveContext, 0x00405450, (void* context));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));
CSF_FUNC(int, __fastcall, AiStateFactory_QueryObjectReference, 0x004DB5F0, (void* self, void* edx, int a2));

CSF_GLOBAL(void*, g_ObjectReferenceManager, 0x008B0550);

static int __fastcall AiStateFactory_SerializeState(
    void* self, void* /*edx*/, void* context)
{
    auto* state = static_cast<std::uint8_t*>(self);
    const std::uint32_t savedContext = CSF_AT(std::uint32_t, context, 0x1C);

    bool failed = false;
    if (CSF_AT(std::uint8_t, context, 0) == 0)
    {
        void* backend = CSF_AT(void*, context, 4);
        failed = !reinterpret_cast<bool (__fastcall*)(void*, void*, int, void*)>(
            CSF_AT(void**, backend, 0)[0x80 / 4])(
                backend, nullptr, 0, context);
    }
    CSF_AT(std::uint8_t, context, 0) = failed;

    AudioSoundManager_ProcessArchiveContext(context);

    std::uint8_t hasReference = 0;
    alignas(8) std::uint8_t referenceBytes[0x4];

    if (CSF_AT(std::uint8_t, context, 0x14) != 0)
    {
        hasReference = static_cast<std::uint8_t>(
            AiStateFactory_QueryObjectReference(
                g_ObjectReferenceManager, nullptr,
                reinterpret_cast<int>(state + 4)));
    }

    failed = CSF_AT(std::uint8_t, context, 0) != 0;
    CSF_AT(std::uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* backend = CSF_AT(void*, context, 4);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        CSF_AT(std::uint8_t, context, 0) =
            reinterpret_cast<int (__fastcall*)(void*, void*, std::uint8_t*, void*)>(
                CSF_AT(void**, backend, 0)[0x44 / 4])(
                    backend, nullptr, &hasReference, context) == -1;
    }

    if (hasReference != 0)
    {
        failed = CSF_AT(std::uint8_t, context, 0) != 0;
        CSF_AT(void*, referenceBytes, 0) = state + 4;
        CSF_AT(std::uint32_t, context, 0x1C) = 0;
        if (!failed)
        {
            CSF_AT(std::uint8_t, context, 0x15) = 1;
            CSF_AT(std::uint8_t, context, 0x16) = 0;
            Archive_SerializeObjectRef(
                referenceBytes, reinterpret_cast<bool*>(context));
        }
    }

    hasReference = 0;
    if (CSF_AT(std::uint8_t, context, 0x14) != 0)
    {
        hasReference = static_cast<std::uint8_t>(
            AiStateFactory_QueryObjectReference(
                g_ObjectReferenceManager, nullptr,
                reinterpret_cast<int>(state + 8)));
    }

    failed = CSF_AT(std::uint8_t, context, 0) != 0;
    CSF_AT(std::uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* backend = CSF_AT(void*, context, 4);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        CSF_AT(std::uint8_t, context, 0) =
            reinterpret_cast<int (__fastcall*)(void*, void*, std::uint8_t*, void*)>(
                CSF_AT(void**, backend, 0)[0x44 / 4])(
                    backend, nullptr, &hasReference, context) == -1;
    }

    if (hasReference != 0)
    {
        failed = CSF_AT(std::uint8_t, context, 0) != 0;
        CSF_AT(void*, referenceBytes, 0) = state + 8;
        CSF_AT(std::uint32_t, context, 0x1C) = 0;
        if (!failed)
        {
            CSF_AT(std::uint8_t, context, 0x15) = 1;
            CSF_AT(std::uint8_t, context, 0x16) = 0;
            Archive_SerializeObjectRef(
                referenceBytes, reinterpret_cast<bool*>(context));
        }
    }

    failed = CSF_AT(std::uint8_t, context, 0) != 0;
    CSF_AT(std::uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* backend = CSF_AT(void*, context, 4);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        CSF_AT(std::uint8_t, context, 0) =
            reinterpret_cast<int (__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, backend, 0)[0x38 / 4])(
                    backend, nullptr, state + 12, context) == -1;
    }

    failed = CSF_AT(std::uint8_t, context, 0) != 0;
    CSF_AT(std::uint32_t, context, 0x1C) = savedContext;
    if (!failed)
    {
        void* backend = CSF_AT(void*, context, 4);
        const int result =
            reinterpret_cast<int (__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, backend, 0)[0x7C / 4])(
                    backend, nullptr, context);
        if (static_cast<std::uint8_t>(result) != 0)
        {
            CSF_AT(std::uint8_t, context, 0) = 0;
            return result;
        }
    }

    CSF_AT(std::uint8_t, context, 0) = 1;
    return 1;
}

HOOK(0x004C1890, AiStateFactory_SerializeState, "auto");
