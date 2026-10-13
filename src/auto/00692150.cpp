#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char*, __fastcall, S_00691860_FindEntry, 0x00691860,
         (void* self, void* edx, unsigned int a2));
CSF_FUNC(uint32_t*, __fastcall, S_00691860_InsertEntry, 0x00691FC0,
         (void* self, void* edx, uint32_t* a2, int a3));

// The packet also labels the search function's address as a vtable.
[[maybe_unused]] static const auto g_PacketSearchVtable = CSF_VTBL(0x00691860);

static int __fastcall S_00691860_CreateAndStoreValue(
    S_00691860* self, void* /*edx*/, int argument, unsigned int key, int options)
{
    char* entry = S_00691860_FindEntry(self, nullptr, key);

    if (entry && CSF_AT(uint32_t, entry, 0) == key)
    {
        void* factory = reinterpret_cast<void*>(self->field_0014);
        void* object =
            ((void*(__fastcall*)(void*, void*, int, int))
                CSF_AT(void**, factory, 0)[0])(
                factory, nullptr, argument, options);

        if (object)
        {
            int value =
                ((int(__fastcall*)(void*, void*))
                    CSF_AT(void**, object, 0)[0x14 / 4])(object, nullptr);
            CSF_AT(int, entry, 4) = value;
            return value;
        }

        CSF_AT(int, entry, 4) = 0;
        return CSF_AT(int, entry, 4);
    }

    void* factory = reinterpret_cast<void*>(self->field_0014);
    auto createObject =
        (void*(__fastcall*)(void*, void*, int, int))
            CSF_AT(void**, factory, 0)[0];

    alignas(8) uint8_t entryBytes[0x10];
    CSF_AT(uint32_t, entryBytes, 0) = key;
    CSF_AT(uint8_t, entryBytes, 8) = 1;
    CSF_AT(uint8_t, entryBytes, 9) = 0;
    CSF_AT(int, entryBytes, 0xC) = 0;

    void* object = createObject(factory, nullptr, argument, options);
    int value = 0;
    if (object)
    {
        value =
            ((int(__fastcall*)(void*, void*))
                CSF_AT(void**, object, 0)[0x14 / 4])(object, nullptr);
    }

    CSF_AT(int, entryBytes, 4) = value;
    S_00691860_InsertEntry(
        self, nullptr, reinterpret_cast<uint32_t*>(entryBytes),
        static_cast<int>(reinterpret_cast<uintptr_t>(entry)));
    return value;
}

HOOK(0x00692150, S_00691860_CreateAndStoreValue, "auto");
