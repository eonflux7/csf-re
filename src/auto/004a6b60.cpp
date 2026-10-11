#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(int, __fastcall, FindEntryIndex, 0x00771B80, (void* self, void* edx, int a2));

typedef int(__fastcall* VCall0)(void*, void*);
typedef void*(__fastcall* VCall1)(void*, void*, int);

static int __fastcall C_008346e8_LookupEntryValue(C_008346e8* self, void* /*edx*/, S_00771b80* a2)
{
    uint8_t* owner = reinterpret_cast<uint8_t*>(self->field_0008);
    uint8_t* ctx = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, owner, 0x34));
    void* obj = reinterpret_cast<void*>(CSF_AT(uint32_t, owner, 0x44));

    void* handle = nullptr;
    bool found = false;

    if (CSF_AT(uint8_t, ctx, 0x48) != 0) {
        int id = CSF_AT(int, ctx, 0x90);
        if (id != -1) {
            void** vtbl = CSF_AT(void**, obj, 0);
            handle = ((VCall1)vtbl[0xC8 / 4])(obj, nullptr, id);
            if (handle != nullptr)
                found = true;
        }
    }

    if (!found) {
        void** vtA = CSF_AT(void**, obj, 0);
        if (((VCall0)vtA[0xFC / 4])(obj, nullptr) == -1)
            return -1;
        void** vtB = CSF_AT(void**, obj, 0);
        int id = ((VCall0)vtB[0xFC / 4])(obj, nullptr);
        handle = ((VCall1)vtB[0xC8 / 4])(obj, nullptr, id);
        if (handle == nullptr)
            return -1;
    }

    int base = Handle_GetValuePointer(handle, nullptr);
    int idx = FindEntryIndex(reinterpret_cast<void*>(base + 0x184), nullptr, (int)a2);
    if (idx != -1) {
        int base2 = Handle_GetValuePointer(handle, nullptr);
        int arr = CSF_AT(int, reinterpret_cast<void*>(base2 + 0x184), 4);
        return CSF_AT(int, reinterpret_cast<void*>(arr), 12 * idx + 8);
    }

    void** vtC = CSF_AT(void**, obj, 0);
    if (((VCall0)vtC[0xFC / 4])(obj, nullptr) == -1)
        return -1;
    void** vtD = CSF_AT(void**, obj, 0);
    int id2 = ((VCall0)vtD[0xFC / 4])(obj, nullptr);
    void* handle2 = ((VCall1)vtD[0xC8 / 4])(obj, nullptr, id2);

    int base3 = Handle_GetValuePointer(handle2, nullptr);
    int idx2 = FindEntryIndex(reinterpret_cast<void*>(base3 + 0x184), nullptr, (int)a2);
    if (idx2 == -1)
        return -1;
    int base4 = Handle_GetValuePointer(handle2, nullptr);
    int arr2 = CSF_AT(int, reinterpret_cast<void*>(base4 + 0x184), 4);
    return CSF_AT(int, reinterpret_cast<void*>(arr2), 12 * idx2 + 8);
}

HOOK(0x004A6B60, C_008346e8_LookupEntryValue, "auto");
