#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(char, __fastcall, S_0069b1c0_TryReleaseBlock, 0x0069B590, (S_0069b1c0* self, void* , unsigned int address));
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));

CSF_GLOBAL(S_0069b1c0, g_ReferenceListPool, 0x008BAEC0);
CSF_GLOBAL(S_0069b1c0, g_ReferenceEntryPool, 0x008BAF04);

static void* __fastcall CoreMemory_FinalizeReferenceArchive(
    uint32_t* self, void* /*edx*/)
{
    alignas(8) uint8_t listErrorBytes[0x200];
    alignas(8) uint8_t singleErrorBytes[0x21C];

    const uint32_t archiveCount = CSF_AT(uint32_t, self, 0xC);
    for (uint32_t archiveIndex = 0; archiveIndex < archiveCount; ++archiveIndex)
    {
        void* archive = CSF_AT(
            void*, CSF_AT(void*, self, 0x4), archiveIndex * 4);

        if (CSF_AT(uint32_t, archive, 0x8) != 0)
        {
            uint32_t recordIndex = 0;
            uint32_t recordOffset = 0;
            do
            {
                auto* record =
                    static_cast<uint8_t*>(CSF_AT(void*, archive, 0x0)) +
                    recordOffset;
                uint32_t* reference = CSF_AT(uint32_t*, record, 0xC);

                if (reference)
                {
                    if (CSF_AT(uint8_t, record, 0x9) != 0)
                    {
                        void* list = CSF_AT(void*, record, 0xC);
                        const uint32_t referenceCount =
                            CSF_AT(uint16_t, list, 0x4);

                        for (uint32_t referenceIndex = 0;
                             referenceIndex < referenceCount;
                             ++referenceIndex)
                        {
                            auto* entry =
                                static_cast<uint8_t*>(CSF_AT(void*, list, 0x0)) +
                                referenceIndex * 8;
                            auto* currentRecord =
                                static_cast<uint8_t*>(CSF_AT(void*, archive, 0x0)) +
                                recordOffset;
                            const uint32_t base =
                                CSF_AT(uint32_t, currentRecord, 0x4);
                            const int32_t displacement =
                                CSF_AT(int16_t, entry, 0x4);
                            uint32_t* destination =
                                CSF_AT(uint32_t*, entry, 0x0);
                            const uint32_t resolved =
                                base + static_cast<uint32_t>(displacement);
                            const uint32_t obtained = *destination;

                            if (obtained == 0 || obtained == resolved)
                            {
                                *destination = resolved;
                            }
                            else
                            {
                                _sprintf(
                                    reinterpret_cast<char*>(listErrorBytes),
                                    CSF_STR(0x0085E5E0, "Error resolviendo referencias. El objeto [ID:%#x REAL:%#x(%s) REF:%#x(%s) OBTENIDO:%#x]  ha sido movido o borrado"),
                                    CSF_AT(uint32_t, currentRecord, 0x0),
                                    resolved,
                                    CSF_STR(0x0085E654, "NOSYM"),
                                    destination,
                                    CSF_STR(0x0085E654, "NOSYM"),
                                    obtained);
                            }
                        }

                        if (list)
                        {
                            if (CSF_AT(uint16_t, list, 0x6) == 4)
                            {
                                S_0069b1c0_TryReleaseBlock(
                                    &g_ReferenceEntryPool, nullptr,
                                    CSF_AT(uint32_t, list, 0x0));
                            }
                            else if (CSF_AT(void*, list, 0x0))
                            {
                                Mem_Free(CSF_AT(void*, list, 0x0));
                                CSF_AT(uint32_t, list, 0x0) = 0;
                            }

                            S_0069b1c0_TryReleaseBlock(
                                &g_ReferenceListPool, nullptr,
                                static_cast<unsigned int>(
                                    reinterpret_cast<uintptr_t>(list)));
                        }

                        CSF_AT(uint8_t, CSF_AT(void*, archive, 0x0),
                               recordOffset + 0x9) = 0;
                        CSF_AT(uint32_t, CSF_AT(void*, archive, 0x0),
                               recordOffset + 0xC) = 0;
                    }
                    else
                    {
                        const int32_t displacement =
                            CSF_AT(int16_t, record, 0xA);
                        const uint32_t resolved =
                            CSF_AT(uint32_t, record, 0x4) +
                            static_cast<uint32_t>(displacement);
                        const uint32_t obtained = *reference;

                        if (obtained == 0 || obtained == resolved)
                        {
                            *reference = resolved;
                        }
                        else
                        {
                            _sprintf(
                                reinterpret_cast<char*>(singleErrorBytes),
                                CSF_STR(0x0085E5E0, "Error resolviendo referencias. El objeto [ID:%#x REAL:%#x(%s) REF:%#x(%s) OBTENIDO:%#x]  ha sido movido o borrado"),
                                CSF_AT(uint32_t, record, 0x0),
                                resolved,
                                CSF_STR(0x0085E654, "NOSYM"),
                                reference,
                                CSF_STR(0x0085E654, "NOSYM"),
                                *reference);
                        }
                    }
                }

                const uint32_t recordCount =
                    CSF_AT(uint32_t, archive, 0x8);
                ++recordIndex;
                recordOffset += 0x10;
                if (recordIndex >= recordCount)
                    break;
            }
            while (true);
        }

        if (CSF_AT(void*, archive, 0x0))
        {
            Mem_Free(CSF_AT(void*, archive, 0x0));
            CSF_AT(uint32_t, archive, 0x0) = 0;
        }
        Mem_Free(archive);
    }

    void* result = CSF_AT(void*, self, 0x4);
    CSF_AT(uint32_t, self, 0x8) = 0;
    CSF_AT(uint32_t, self, 0xC) = 0;
    if (result)
    {
        result = reinterpret_cast<void*>(
            static_cast<uintptr_t>(Mem_Free(result)));
        CSF_AT(uint32_t, self, 0x4) = 0;
    }
    return result;
}

HOOK(0x00691E00, CoreMemory_FinalizeReferenceArchive, "auto");
