#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(void*, __cdecl, AllocateDocumentMemory, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, FreeDocumentMemory, 0x00691110, (void* memory));
CSF_FUNC(void*, __fastcall, ConstructDocument, 0x00692B10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, DestroyDocument, 0x00694380, (void* self, void* edx));
CSF_FUNC(char, __fastcall, LoadDocument, 0x00694770,
         (void* self, void* edx, const char* source));

static void __fastcall C_00864508_ReloadMatchingDocuments(
    C_00864508* self, void* /*edx*/)
{
    using QueryObject = void* (__fastcall*)(void*, void*);
    using ObjectAction = void (__fastcall*)(void*, void*);
    using ObjectArgumentAction = void (__fastcall*)(void*, void*, void*);
    using DocumentAction = void (__fastcall*)(void*, void*, C_00864508*, void*);

    const std::uint32_t objectBegin = self->field_0048;
    const std::uint32_t objectCount = objectBegin
        ? static_cast<std::uint32_t>(
              static_cast<std::int32_t>(self->field_004c - objectBegin) >> 2)
        : 0;

    for (std::uint32_t objectIndex = 0; objectIndex < objectCount; ++objectIndex)
    {
        const std::uint32_t recordBegin = self->field_0014;
        if (!recordBegin)
            continue;

        std::uint32_t recordsRemaining = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(self->field_0018 - recordBegin) >> 6);
        if (!recordsRemaining)
            continue;

        std::uint32_t recordOffset = 0;
        do
        {
            void* record = reinterpret_cast<void*>(self->field_0014 + recordOffset);
            void* objectEntry = reinterpret_cast<void*>(
                self->field_0048 + objectIndex * 4);

            if (CSF_AT(std::uint32_t, record, 0x38) ==
                    CSF_AT(std::uint32_t, objectEntry, 0) &&
                CSF_AT(std::uint32_t, record, 0x3C) == 0)
            {
                bool skipDocument = false;
                const std::uint32_t nameLength =
                    CSF_AT(std::uint32_t, record, 0x14);

                if (nameLength)
                {
                    const std::uint32_t compareLength =
                        nameLength < 17 ? nameLength : 17;
                    const char* name =
                        CSF_AT(std::uint32_t, record, 0x18) < 16
                        ? reinterpret_cast<const char*>(record) + 4
                        : reinterpret_cast<const char*>(
                              CSF_AT(std::uint32_t, record, 0x04));
                    const char* excludedName =
                        CSF_STR(0x008403B4, "Menus/P_Juego.fbs");

                    bool equal = true;
                    for (std::uint32_t i = 0; i < compareLength; ++i)
                    {
                        if (static_cast<unsigned char>(name[i]) !=
                            static_cast<unsigned char>(excludedName[i]))
                        {
                            equal = false;
                            break;
                        }
                    }
                    skipDocument = equal && nameLength == 17;
                }

                if (!skipDocument)
                {
                    void* document = AllocateDocumentMemory(0x3C);
                    if (document)
                        document = ConstructDocument(document, nullptr);

                    CSF_AT(std::uint32_t, record, 0x3C) =
                        reinterpret_cast<std::uint32_t>(document);

                    const char* source =
                        CSF_AT(std::uint32_t, record, 0x34) < 16
                        ? reinterpret_cast<const char*>(record) + 0x20
                        : reinterpret_cast<const char*>(
                              CSF_AT(std::uint32_t, record, 0x20));

                    if (LoadDocument(document, nullptr, source))
                    {
                        const std::uint32_t objectOffset = objectIndex * 4;
                        void* object = reinterpret_cast<void*>(
                            CSF_AT(std::uint32_t,
                                   reinterpret_cast<void*>(self->field_0048),
                                   objectOffset));

                        void* oldInstance =
                            reinterpret_cast<QueryObject>(
                                CSF_AT(void**, object, 0)[0x20 / 4])(
                                    object, nullptr);

                        reinterpret_cast<ObjectAction>(
                            CSF_AT(void**, oldInstance, 0)[0xF4 / 4])(
                                oldInstance, nullptr);

                        void* manager =
                            reinterpret_cast<void*>(self->field_0040);
                        reinterpret_cast<ObjectArgumentAction>(
                            CSF_AT(void**, manager, 0)[0x100 / 4])(
                                manager, nullptr, oldInstance);

                        object = reinterpret_cast<void*>(
                            CSF_AT(std::uint32_t,
                                   reinterpret_cast<void*>(self->field_0048),
                                   objectOffset));
                        reinterpret_cast<ObjectAction>(
                            CSF_AT(void**, object, 0)[0x3C / 4])(
                                object, nullptr);

                        document = reinterpret_cast<void*>(
                            CSF_AT(std::uint32_t, record, 0x3C));
                        object = reinterpret_cast<void*>(
                            CSF_AT(std::uint32_t,
                                   reinterpret_cast<void*>(self->field_0048),
                                   objectOffset));
                        reinterpret_cast<DocumentAction>(
                            CSF_AT(void**, object, 0)[0x38 / 4])(
                                object, nullptr, self, document);

                        object = reinterpret_cast<void*>(
                            CSF_AT(std::uint32_t,
                                   reinterpret_cast<void*>(self->field_0048),
                                   objectOffset));
                        void* newInstance =
                            reinterpret_cast<QueryObject>(
                                CSF_AT(void**, object, 0)[0x20 / 4])(
                                    object, nullptr);

                        manager = reinterpret_cast<void*>(self->field_0040);
                        reinterpret_cast<ObjectArgumentAction>(
                            CSF_AT(void**, manager, 0)[0xF8 / 4])(
                                manager, nullptr, newInstance);

                        object = reinterpret_cast<void*>(
                            CSF_AT(std::uint32_t,
                                   reinterpret_cast<void*>(self->field_0048),
                                   objectOffset));
                        reinterpret_cast<ObjectArgumentAction>(
                            CSF_AT(void**, newInstance, 0)[0xEC / 4])(
                                newInstance, nullptr, object);

                        document = reinterpret_cast<void*>(
                            CSF_AT(std::uint32_t, record, 0x3C));
                        object = reinterpret_cast<void*>(
                            CSF_AT(std::uint32_t,
                                   reinterpret_cast<void*>(self->field_0048),
                                   objectOffset));
                        reinterpret_cast<DocumentAction>(
                            CSF_AT(void**, object, 0)[0x08 / 4])(
                                object, nullptr, self, document);
                    }

                    void* temporaryDocument = reinterpret_cast<void*>(
                        CSF_AT(std::uint32_t, record, 0x3C));
                    if (temporaryDocument)
                    {
                        DestroyDocument(temporaryDocument, nullptr);
                        FreeDocumentMemory(temporaryDocument);
                        CSF_AT(std::uint32_t, record, 0x3C) = 0;
                    }
                }
            }

            recordOffset += 0x40;
        } while (--recordsRemaining != 0);
    }
}

HOOK(0x0071AF30, C_00864508_ReloadMatchingDocuments, "auto");
