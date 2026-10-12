#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, DynamicObj_GetResourceKey, 0x00442A20, (int a1));
CSF_FUNC(void*, __cdecl, DynamicObj_ConfigureResource, 0x00442B60, (int a1, char* a2));
CSF_FUNC(char, __cdecl, SceneLoader_SerializeReference, 0x00442E70, (void* a3, bool* a2));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, ResolveRwManager, 0x006B13B0, (int managerId, void* edx));

static int __fastcall DynamicObj_Serialize(void* self, void* /*edx*/, void* context)
{
    auto* object = static_cast<std::uint8_t*>(self);
    bool failed = false;
    if (CSF_AT(std::uint8_t, context, 0) == 0)
    {
        void* serializer = CSF_AT(void*, context, 0x4);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, void*)>(
            CSF_AT(void**, serializer, 0)[128 / 4])(
                serializer, nullptr, 0, context);
    }

    const int savedState = CSF_AT(int, context, 0x1C);
    CSF_AT(std::uint8_t, context, 0) = failed;
    CSF_AT(int, context, 0x1C) = 0;
    if (!failed)
    {
        void* serializer = CSF_AT(void*, context, 0x4);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        CSF_AT(std::uint8_t, context, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, serializer, 0)[88 / 4])(
                    serializer, nullptr, object + 4, context) == -1;
    }

    failed = CSF_AT(std::uint8_t, context, 0) != 0;
    CSF_AT(int, context, 0x1C) = 0;
    if (!failed)
    {
        void* serializer = CSF_AT(void*, context, 0x4);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        CSF_AT(std::uint8_t, context, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, serializer, 0)[88 / 4])(
                    serializer, nullptr, object + 40, context) == -1;
    }

    failed = CSF_AT(std::uint8_t, context, 0) != 0;
    CSF_AT(int, context, 0x1C) = 0;
    if (!failed)
    {
        void* serializer = CSF_AT(void*, context, 0x4);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        CSF_AT(std::uint8_t, context, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, serializer, 0)[72 / 4])(
                    serializer, nullptr, object + 8, context) == -1;
    }

    failed = CSF_AT(std::uint8_t, context, 0) != 0;
    CSF_AT(int, context, 0x1C) = 0;
    if (!failed)
    {
        void* serializer = CSF_AT(void*, context, 0x4);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        CSF_AT(std::uint8_t, context, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, serializer, 0)[84 / 4])(
                    serializer, nullptr, object + 16, context) == -1;
    }

    failed = CSF_AT(std::uint8_t, context, 0) != 0;
    CSF_AT(int, context, 0x1C) = 0;
    if (!failed)
    {
        void* serializer = CSF_AT(void*, context, 0x4);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        CSF_AT(std::uint8_t, context, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, serializer, 0)[84 / 4])(
                    serializer, nullptr, object + 20, context) == -1;
    }

    failed = CSF_AT(std::uint8_t, context, 0) != 0;
    CSF_AT(int, context, 0x1C) = 0;
    if (!failed)
    {
        void* serializer = CSF_AT(void*, context, 0x4);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        CSF_AT(std::uint8_t, context, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, serializer, 0)[84 / 4])(
                    serializer, nullptr, object + 24, context) == -1;
    }

    failed = CSF_AT(std::uint8_t, context, 0) != 0;
    CSF_AT(int, context, 0x1C) = 0;
    if (!failed)
    {
        void* serializer = CSF_AT(void*, context, 0x4);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        CSF_AT(std::uint8_t, context, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, serializer, 0)[84 / 4])(
                    serializer, nullptr, object + 28, context) == -1;
    }

    failed = CSF_AT(std::uint8_t, context, 0) != 0;
    CSF_AT(int, context, 0x1C) = 0;
    if (!failed)
    {
        void* serializer = CSF_AT(void*, context, 0x4);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        CSF_AT(std::uint8_t, context, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, serializer, 0)[68 / 4])(
                    serializer, nullptr, object + 32, context) == -1;
    }

    failed = CSF_AT(std::uint8_t, context, 0) != 0;
    CSF_AT(int, context, 0x1C) = 0;
    if (!failed)
    {
        void* serializer = CSF_AT(void*, context, 0x4);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        CSF_AT(std::uint8_t, context, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, serializer, 0)[72 / 4])(
                    serializer, nullptr, object + 12, context) == -1;
    }

    if (CSF_AT(std::uint8_t, context, 0x14) == 0 &&
        CSF_AT(std::uint8_t, context, 0) == 0)
    {
        alignas(8) std::uint8_t resourceKeyBytes[0x8];
        CSF_AT(int, resourceKeyBytes, 0x4) = 0;
        CSF_AT(int, resourceKeyBytes, 0) =
            DynamicObj_GetResourceKey(CSF_AT(int, self, 0x4));

        const int globalManager = GetGlobalInt();
        void* manager = reinterpret_cast<void*>(ResolveRwManager(globalManager, nullptr));
        char* resource =
            reinterpret_cast<char*(__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, manager, 0)[32 / 4])(
                    manager, nullptr, resourceKeyBytes);
        CSF_AT(char*, self, 0x24) = resource;
        if (resource && (CSF_AT(std::uint8_t, self, 0xC) & 1) != 0)
            DynamicObj_ConfigureResource(CSF_AT(int, self, 0x4), resource);
    }

    failed = CSF_AT(std::uint8_t, context, 0) != 0;
    CSF_AT(int, context, 0x1C) = 0;
    if (!failed)
    {
        CSF_AT(std::uint8_t, context, 0x15) = 1;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        SceneLoader_SerializeReference(object + 36, static_cast<bool*>(context));
    }

    CSF_AT(int, context, 0x1C) = savedState;
    if (CSF_AT(std::uint8_t, context, 0) != 0)
    {
        CSF_AT(std::uint8_t, context, 0) = 1;
        return 1;
    }

    void* serializer = CSF_AT(void*, context, 0x4);
    const int result =
        reinterpret_cast<int(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, serializer, 0)[124 / 4])(
                serializer, nullptr, context);
    if (static_cast<std::uint8_t>(result) == 0)
    {
        CSF_AT(std::uint8_t, context, 0) = 1;
        return 1;
    }

    CSF_AT(std::uint8_t, context, 0) = 0;
    return result;
}

HOOK(0x004433C0, DynamicObj_Serialize, "auto");
