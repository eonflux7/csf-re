#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, CoreMemory_AppendReference, 0x00691670,
         (void* self, void* edx, int a2, int a3));
CSF_FUNC(char*, __fastcall, CoreMemory_FindReferenceEntry, 0x00691860,
         (void* self, void* edx, unsigned int a2));
CSF_FUNC(uint32_t*, __fastcall, CoreMemory_InsertReferenceEntry, 0x00691FC0,
         (void* self, void* edx, uint32_t* a2, int a3));
CSF_FUNC(int, __fastcall, S_0069afe0_ProcessSectionTable, 0x0069B460, (S_0069afe0* self, void*));

CSF_GLOBAL(S_0069afe0, g_ReferenceListPool, 0x008BAEC0);

static uint32_t* __fastcall S_00691860_RegisterReference(
    S_00691860* self, void* /*edx*/, uint32_t* destination,
    unsigned int identifier, int16_t offset, int /*unused*/)
{
    // The packet also classifies this lookup function's address as a vtable.
    (void)CSF_VTBL(0x00691860);

    char* entry = CoreMemory_FindReferenceEntry(self, nullptr, identifier);
    if (entry && CSF_AT(uint32_t, entry, 0x0) == identifier)
    {
        if (!CSF_AT(uint8_t, entry, 0x9))
        {
            const uint32_t previousReference = CSF_AT(uint32_t, entry, 0xC);
            if (!previousReference)
            {
                const uint32_t resolvedAddress = CSF_AT(uint32_t, entry, 0x4);
                CSF_AT(uint32_t, entry, 0xC) =
                    reinterpret_cast<uint32_t>(destination);
                CSF_AT(int16_t, entry, 0xA) = offset;
                *destination = resolvedAddress + static_cast<int32_t>(offset);
                return reinterpret_cast<uint32_t*>(resolvedAddress);
            }

            const uint16_t previousOffset = CSF_AT(uint16_t, entry, 0xA);
            const int allocation =
                S_0069afe0_ProcessSectionTable(&g_ReferenceListPool, nullptr);
            void* referenceList = reinterpret_cast<void*>(allocation);
            if (referenceList)
            {
                CSF_AT(uint32_t, referenceList, 0x0) = 0;
                CSF_AT(uint16_t, referenceList, 0x4) = 0;
                CSF_AT(uint16_t, referenceList, 0x6) = 0;
            }

            CSF_AT(uint32_t, entry, 0xC) =
                reinterpret_cast<uint32_t>(referenceList);
            CSF_AT(uint8_t, entry, 0x9) = 1;
            CoreMemory_AppendReference(
                referenceList, nullptr, static_cast<int>(previousReference),
                static_cast<int>(previousOffset));
        }

        CoreMemory_AppendReference(
            reinterpret_cast<void*>(CSF_AT(uint32_t, entry, 0xC)),
            nullptr, reinterpret_cast<int>(destination),
            static_cast<int>(static_cast<uint16_t>(offset)));

        const uint32_t resolvedAddress = CSF_AT(uint32_t, entry, 0x4);
        *destination = resolvedAddress + static_cast<int32_t>(offset);
        return reinterpret_cast<uint32_t*>(resolvedAddress);
    }

    alignas(8) uint8_t entryBytes[0x10];
    CSF_AT(uint32_t, entryBytes, 0xC) =
        reinterpret_cast<uint32_t>(destination);
    *destination = 0;
    CSF_AT(uint32_t, entryBytes, 0x0) = identifier;
    CSF_AT(uint32_t, entryBytes, 0x4) = 0;
    CSF_AT(uint8_t, entryBytes, 0x8) = 1;
    CSF_AT(uint8_t, entryBytes, 0x9) = 0;
    CSF_AT(int16_t, entryBytes, 0xA) = offset;
    return CoreMemory_InsertReferenceEntry(
        self, nullptr, reinterpret_cast<uint32_t*>(entryBytes),
        reinterpret_cast<int>(entry));
}

HOOK(0x00692290, S_00691860_RegisterReference, "auto");
