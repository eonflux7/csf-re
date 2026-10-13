#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int archiveAddress, S_00691860* string));

static int __fastcall Archive_SerializeOptionalCollectionA(
    void* self, void* /*edx*/, S_00691860* collection)
{
    bool unavailable = false;
    if (!collection->pad_0000[0])
    {
        void* backend = reinterpret_cast<void*>(collection->field_0004);
        unavailable =
            !((bool(__fastcall*)(void*, void*, int, S_00691860*))
                CSF_AT(void**, backend, 0)[0x80 / 4])(
                    backend, nullptr, 0, collection);
    }

    const uint32_t savedContext = CSF_AT(uint32_t, collection, 0x1C);
    collection->pad_0000[0] = unavailable;
    CSF_AT(uint32_t, collection, 0x1C) = 0;

    if (!unavailable)
    {
        CSF_AT(uint8_t, collection, 0x15) = 0;
        CSF_AT(uint8_t, collection, 0x16) = 0;
        Archive_SerializeString(reinterpret_cast<int>(self), collection);
    }

    const bool shouldSerialize = collection->pad_0000[0] == 0;
    CSF_AT(uint32_t, collection, 0x1C) = 0;
    if (shouldSerialize)
    {
        void* backend = reinterpret_cast<void*>(collection->field_0004);
        CSF_AT(uint8_t, collection, 0x15) = 0;
        CSF_AT(uint8_t, collection, 0x16) = 0;
        collection->pad_0000[0] =
            ((int(__fastcall*)(void*, void*, void*, S_00691860*))
                CSF_AT(void**, backend, 0)[0x44 / 4])(
                    backend, nullptr,
                    static_cast<uint8_t*>(self) + 0xC, collection) == -1;
    }

    CSF_AT(uint32_t, collection, 0x1C) = savedContext;

    if (!collection->pad_0000[0])
    {
        void* backend = reinterpret_cast<void*>(collection->field_0004);
        const int result =
            ((int(__fastcall*)(void*, void*, S_00691860*))
                CSF_AT(void**, backend, 0)[0x7C / 4])(
                    backend, nullptr, collection);
        if (static_cast<uint8_t>(result) != 0)
        {
            collection->pad_0000[0] = 0;
            return result;
        }
    }

    collection->pad_0000[0] = 1;
    return 1;
}

HOOK(0x0044F0B0, Archive_SerializeOptionalCollectionA, "auto");
