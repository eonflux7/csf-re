#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());

static int __fastcall C_0082d510_ResolveMatrix(
    C_0082d510* self, void* /*edx*/, int outputAddress, int index)
{
    void* owner = reinterpret_cast<void*>(self->field_0008);
    void* collection =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, owner, 0)[0x28 / 4])(owner, nullptr);

    if (index >= 0)
    {
        int entryIndex =
            ((int (__fastcall*)(void*, void*, int))
                CSF_AT(void**, collection, 0)[0x88 / 4])(
                    collection, nullptr, index);

        owner = reinterpret_cast<void*>(self->field_0008);
        collection =
            ((void* (__fastcall*)(void*, void*))
                CSF_AT(void**, owner, 0)[0x28 / 4])(owner, nullptr);

        void* entry =
            ((void* (__fastcall*)(void*, void*, int))
                CSF_AT(void**, collection, 0)[0x80 / 4])(
                    collection, nullptr, entryIndex);

        if (entry)
        {
            ((void (__fastcall*)(void*, void*, int))
                CSF_AT(void**, entry, 0)[0x3C / 4])(
                    entry, nullptr, outputAddress);
            return outputAddress;
        }
    }
    else
    {
        void* alternateCollection =
            ((void* (__fastcall*)(void*, void*))
                CSF_AT(void**, collection, 0)[0x68 / 4])(
                    collection, nullptr);

        if (alternateCollection)
        {
            int entryIndex =
                ((int (__fastcall*)(void*, void*, int))
                    CSF_AT(void**, alternateCollection, 0)[0x68 / 4])(
                        alternateCollection, nullptr, index & 0x7FFFFFFF);

            void* entry =
                ((void* (__fastcall*)(void*, void*, int))
                    CSF_AT(void**, alternateCollection, 0)[0x6C / 4])(
                        alternateCollection, nullptr, entryIndex);

            if (entry)
            {
                ((void (__fastcall*)(void*, void*, int))
                    CSF_AT(void**, entry, 0)[0x1C / 4])(
                        entry, nullptr, outputAddress);
                return outputAddress;
            }
        }
    }

    std::uint32_t* matrix = RwMatrixCreate();
    void* output = reinterpret_cast<void*>(outputAddress);
    CSF_AT(std::uint32_t*, output, 0) = matrix;
    CSF_AT(std::uint8_t, output, 4) = 1;
    return outputAddress;
}

HOOK(0x00405830, C_0082d510_ResolveMatrix, "auto");
