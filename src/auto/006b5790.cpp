#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(char, __fastcall, sub_6B4E30, 0x006B4E30, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, sub_6B4E70, 0x006B4E70, (void* self, void* edx, unsigned int a2));
CSF_FUNC(void, __fastcall, sub_6B5460, 0x006B5460, (void* self, void* edx, void** a3));

static int __stdcall MatrixList_Serialize(int* list, void* context)
{
    const std::uint8_t savedMode = CSF_AT(std::uint8_t, context, 0x15);
    alignas(8) std::uint8_t countBytes[0x4];
    auto& count = *reinterpret_cast<unsigned int*>(countBytes);
    count = 0;
    int result = 0;

    if (CSF_AT(std::uint8_t, context, 0x14))
    {
        const std::uint32_t begin = CSF_AT(std::uint32_t, list, 0x4);
        unsigned int limit = begin
            ? static_cast<unsigned int>(
                  static_cast<std::int32_t>(
                      CSF_AT(std::uint32_t, list, 0x8) - begin) >> 3)
            : 0;
        count = limit;

        if (!CSF_AT(std::uint8_t, context, 0x0))
        {
            void* serializer = CSF_AT(void*, context, 0x4);
            const bool succeeded =
                ((bool(__fastcall*)(void*, void*, unsigned int*, void*))
                     CSF_AT(void**, serializer, 0x0)[0x78 / 4])(
                    serializer, nullptr, &count, context);
            limit = count;
            result = !succeeded;
        }
        CSF_AT(std::uint8_t, context, 0x0) = static_cast<std::uint8_t>(result);
        if (static_cast<std::uint8_t>(result))
            goto Failure;

        for (unsigned int index = 0; index < limit; ++index)
        {
            const std::uint32_t element =
                CSF_AT(std::uint32_t, list, 0x4) + 8 * index;
            const bool canProcess = !CSF_AT(std::uint8_t, context, 0x0);
            result = static_cast<std::int32_t>(element);
            CSF_AT(std::uint32_t, context, 0x1C) = 0;
            if (canProcess)
            {
                CSF_AT(std::uint8_t, context, 0x15) = savedMode;
                void* serializer = CSF_AT(void*, context, 0x4);
                CSF_AT(std::uint8_t, context, 0x16) = 0;
                const int status =
                    ((int(__fastcall*)(void*, void*, void*, void*))
                         CSF_AT(void**, serializer, 0x0)[0x14 / 4])(
                        serializer, nullptr, reinterpret_cast<void*>(element), context);
                limit = count;
                result = status == -1;
                CSF_AT(std::uint8_t, context, 0x0) =
                    static_cast<std::uint8_t>(result);
            }
        }
    }
    else
    {
        if (!CSF_AT(std::uint8_t, context, 0x0))
        {
            void* serializer = CSF_AT(void*, context, 0x4);
            result =
                !((bool(__fastcall*)(void*, void*, unsigned int*, void*))
                      CSF_AT(void**, serializer, 0x0)[0x78 / 4])(
                    serializer, nullptr, &count, context);
        }
        CSF_AT(std::uint8_t, context, 0x0) = static_cast<std::uint8_t>(result);
        if (static_cast<std::uint8_t>(result))
            goto Failure;

        if (savedMode)
        {
            result = static_cast<std::int32_t>(count);
            for (unsigned int index = 0; index < count; ++index)
            {
                const std::uint32_t element =
                    CSF_AT(std::uint32_t, list, 0x4) + 8 * index;
                const bool canProcess = !CSF_AT(std::uint8_t, context, 0x0);
                CSF_AT(std::uint32_t, context, 0x1C) = 0;
                if (canProcess)
                {
                    CSF_AT(std::uint8_t, context, 0x15) = savedMode;
                    void* serializer = CSF_AT(void*, context, 0x4);
                    CSF_AT(std::uint8_t, context, 0x16) = 0;
                    CSF_AT(std::uint8_t, context, 0x0) =
                        ((int(__fastcall*)(void*, void*, void*, void*))
                             CSF_AT(void**, serializer, 0x0)[0x14 / 4])(
                            serializer, nullptr, reinterpret_cast<void*>(element),
                            context) == -1;
                }
                result = static_cast<std::int32_t>(count);
            }
        }
        else
        {
            sub_6B4E30(list, nullptr);
            result = static_cast<std::int32_t>(sub_6B4E70(list, nullptr, count));

            alignas(8) std::uint8_t entryBytes[0x8];
            if (count)
            {
                CSF_AT(std::uint8_t, entryBytes, 0x4) = 1;
                unsigned int index = 0;
                do
                {
                    CSF_AT(void*, entryBytes, 0x0) = RwMatrixCreate();
                    sub_6B5460(list, nullptr, reinterpret_cast<void**>(entryBytes));

                    const std::uint32_t element =
                        CSF_AT(std::uint32_t, list, 0x4) + 8 * index;
                    const bool canProcess = !CSF_AT(std::uint8_t, context, 0x0);
                    CSF_AT(std::uint32_t, context, 0x1C) = 0;
                    if (canProcess)
                    {
                        void* serializer = CSF_AT(void*, context, 0x4);
                        CSF_AT(std::uint8_t, context, 0x15) = 0;
                        CSF_AT(std::uint8_t, context, 0x16) = 0;
                        CSF_AT(std::uint8_t, context, 0x0) =
                            ((int(__fastcall*)(void*, void*, void*, void*))
                                 CSF_AT(void**, serializer, 0x0)[0x14 / 4])(
                                serializer, nullptr, reinterpret_cast<void*>(element),
                                context) == -1;
                    }
                    RwMatrixDestroy(CSF_AT(std::uint32_t*, entryBytes, 0x0));
                    result = static_cast<std::int32_t>(++index);
                } while (index < count);
            }
        }
    }

    if (CSF_AT(std::uint8_t, context, 0x0))
        goto Failure;

    {
        void* serializer = CSF_AT(void*, context, 0x4);
        result = ((int(__fastcall*)(void*, void*, void*))
                      CSF_AT(void**, serializer, 0x0)[0x7C / 4])(
            serializer, nullptr, context);
    }
    CSF_AT(std::uint8_t, context, 0x0) =
        static_cast<std::uint8_t>(result) == 0;
    return result;

Failure:
    CSF_AT(std::uint8_t, context, 0x0) = 1;
    return result;
}

HOOK(0x006B5790, MatrixList_Serialize, "auto");
