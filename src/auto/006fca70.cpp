#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(void**, __fastcall, SceneLoader_RegisterArchivedObject, 0x0069ACB0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

static int __fastcall GfxLightObject_Archive(void* self, void* /*edx*/, void* archive)
{
    alignas(8) uint8_t heightBytes[0x8];
    int& archivedHeight = CSF_AT(int, heightBytes, 0);
    void*& archivedObject = CSF_AT(void*, heightBytes, 4);
    archivedObject = self;

    bool failed = CSF_AT(uint8_t, archive, 0) == 0 &&
        !((bool(__fastcall*)(void*, void*, int, void*))
            CSF_AT(void**, CSF_AT(void*, archive, 4), 0)[0x80 / 4])(
                CSF_AT(void*, archive, 4), nullptr, 0, archive);

    const uint32_t savedContext = CSF_AT(uint32_t, archive, 0x1C);
    CSF_AT(uint8_t, archive, 0) = failed;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (!failed)
    {
        void* serializer = CSF_AT(void*, archive, 4);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, serializer, 0)[0x0C / 4])(
                    serializer, nullptr, static_cast<uint8_t*>(self) + 0x1C, archive) == -1;
    }

    failed = CSF_AT(uint8_t, archive, 0) != 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (!failed)
    {
        void* serializer = CSF_AT(void*, archive, 4);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, serializer, 0)[0x44 / 4])(
                    serializer, nullptr, static_cast<uint8_t*>(self) + 0x4C, archive) == -1;
    }

    failed = CSF_AT(uint8_t, archive, 0) != 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (!failed)
    {
        void* serializer = CSF_AT(void*, archive, 4);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, serializer, 0)[0x20 / 4])(
                    serializer, nullptr, static_cast<uint8_t*>(self) + 0x38, archive) == -1;
    }

    failed = CSF_AT(uint8_t, archive, 0) != 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (!failed)
    {
        void* serializer = CSF_AT(void*, archive, 4);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, serializer, 0)[0x10 / 4])(
                    serializer, nullptr, static_cast<uint8_t*>(self) + 0x0C, archive) == -1;
    }

    void* display = LookupObjectById(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    archivedHeight = ((int(__fastcall*)(void*, void*))
        CSF_AT(void**, display, 0)[0x24 / 4])(display, nullptr);
    int archivedWidth = ((int(__fastcall*)(void*, void*))
        CSF_AT(void**, display, 0)[0x20 / 4])(display, nullptr);

    failed = CSF_AT(uint8_t, archive, 0) != 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (!failed)
    {
        void* serializer = CSF_AT(void*, archive, 4);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0) =
            ((int(__fastcall*)(void*, void*, int*, void*))
                CSF_AT(void**, serializer, 0)[0x58 / 4])(
                    serializer, nullptr, &archivedHeight, archive) == -1;
    }

    failed = CSF_AT(uint8_t, archive, 0) != 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (!failed)
    {
        void* serializer = CSF_AT(void*, archive, 4);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0) =
            ((int(__fastcall*)(void*, void*, int*, void*))
                CSF_AT(void**, serializer, 0)[0x58 / 4])(
                    serializer, nullptr, &archivedWidth, archive) == -1;
    }

    if (!CSF_AT(uint8_t, archive, 0x14) && !CSF_AT(uint8_t, archive, 0))
    {
        const int currentHeight = ((int(__fastcall*)(void*, void*))
            CSF_AT(void**, display, 0)[0x24 / 4])(display, nullptr);
        bool sameDimensions = archivedHeight == currentHeight;
        if (sameDimensions)
        {
            const int currentWidth = ((int(__fastcall*)(void*, void*))
                CSF_AT(void**, display, 0)[0x20 / 4])(display, nullptr);
            sameDimensions = archivedWidth == currentWidth;
        }

        if (!sameDimensions)
        {
            int dimension = ((int(__fastcall*)(void*, void*))
                CSF_AT(void**, display, 0)[0x20 / 4])(display, nullptr);
            int32_t product = static_cast<int32_t>(
                CSF_AT(uint32_t, archivedObject, 0x0C) * static_cast<uint32_t>(dimension));
            CSF_AT(uint32_t, archivedObject, 0x0C) = static_cast<uint32_t>(
                static_cast<int64_t>(static_cast<double>(product) / archivedWidth));

            dimension = ((int(__fastcall*)(void*, void*))
                CSF_AT(void**, display, 0)[0x20 / 4])(display, nullptr);
            product = static_cast<int32_t>(
                CSF_AT(uint32_t, archivedObject, 0x14) * static_cast<uint32_t>(dimension));
            CSF_AT(uint32_t, archivedObject, 0x14) = static_cast<uint32_t>(
                static_cast<int64_t>(static_cast<double>(product) / archivedWidth));

            dimension = ((int(__fastcall*)(void*, void*))
                CSF_AT(void**, display, 0)[0x24 / 4])(display, nullptr);
            product = static_cast<int32_t>(
                CSF_AT(uint32_t, archivedObject, 0x10) * static_cast<uint32_t>(dimension));
            CSF_AT(uint32_t, archivedObject, 0x10) = static_cast<uint32_t>(
                static_cast<int64_t>(static_cast<double>(product) / archivedHeight));

            dimension = ((int(__fastcall*)(void*, void*))
                CSF_AT(void**, display, 0)[0x24 / 4])(display, nullptr);
            product = static_cast<int32_t>(
                CSF_AT(uint32_t, archivedObject, 0x18) * static_cast<uint32_t>(dimension));
            CSF_AT(uint32_t, archivedObject, 0x18) = static_cast<uint32_t>(
                static_cast<int64_t>(static_cast<double>(product) / archivedHeight));
        }

        const uintptr_t objectAddress = reinterpret_cast<uintptr_t>(archivedObject);
        SceneLoader_RegisterArchivedObject(
            CSF_AT(void*, archive, 0x10), nullptr,
            objectAddress != 8 ? static_cast<int>(objectAddress) : 0);
    }

    failed = CSF_AT(uint8_t, archive, 0) != 0;
    CSF_AT(uint32_t, archive, 0x1C) = savedContext;
    if (!failed)
    {
        void* serializer = CSF_AT(void*, archive, 4);
        const int result = ((int(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, serializer, 0)[0x7C / 4])(serializer, nullptr, archive);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, archive, 0) = 0;
            return result;
        }
    }
    CSF_AT(uint8_t, archive, 0) = 1;
    return 1;
}

HOOK(0x006FCA70, GfxLightObject_Archive, "auto");
