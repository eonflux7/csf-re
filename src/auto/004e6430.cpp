#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int archiveAddress, S_00691860* string));

static int __fastcall Archive_SerializeThreeFields(void* self, void* /*edx*/, S_00691860* a2)
{
    uint8_t* thisBytes = reinterpret_cast<uint8_t*>(self);

    // Error flag becomes 1 only if it was 0 and the slot 0x80 check returns false.
    {
        bool failed = false;
        if (a2->pad_0000[0] == 0)
        {
            void* obj = reinterpret_cast<void*>(a2->field_0004);
            typedef bool(__fastcall * Fn)(void*, void*, int, S_00691860*);
            Fn fn = reinterpret_cast<Fn>(CSF_AT(void**, obj, 0)[0x80 / 4]);
            failed = !fn(obj, nullptr, 0, a2);
        }
        a2->pad_0000[0] = failed ? 1 : 0;
    }

    uint32_t savedField = CSF_AT(uint32_t, a2, 0x1C);
    CSF_AT(uint32_t, a2, 0x1C) = 0;

    typedef int(__fastcall * ReadFn)(void*, void*, void*, S_00691860*);

    if (a2->pad_0000[0] == 0)
    {
        void* obj = reinterpret_cast<void*>(a2->field_0004);
        CSF_AT(uint8_t, a2, 0x15) = 0;
        CSF_AT(uint8_t, a2, 0x16) = 0;
        ReadFn fn = reinterpret_cast<ReadFn>(CSF_AT(void**, obj, 0)[0x58 / 4]);
        a2->pad_0000[0] = (fn(obj, nullptr, self, a2) == -1) ? 1 : 0;
    }

    CSF_AT(uint32_t, a2, 0x1C) = 0;

    if (a2->pad_0000[0] == 0)
    {
        void* obj = reinterpret_cast<void*>(a2->field_0004);
        CSF_AT(uint8_t, a2, 0x15) = 0;
        CSF_AT(uint8_t, a2, 0x16) = 0;
        ReadFn fn = reinterpret_cast<ReadFn>(CSF_AT(void**, obj, 0)[0x58 / 4]);
        a2->pad_0000[0] = (fn(obj, nullptr, thisBytes + 4, a2) == -1) ? 1 : 0;
    }

    CSF_AT(uint32_t, a2, 0x1C) = 0;

    if (a2->pad_0000[0] == 0)
    {
        CSF_AT(uint8_t, a2, 0x15) = 0;
        CSF_AT(uint8_t, a2, 0x16) = 0;
        Archive_SerializeString(static_cast<int>(reinterpret_cast<uintptr_t>(thisBytes + 8)), a2);
    }

    CSF_AT(uint32_t, a2, 0x1C) = savedField;

    if (a2->pad_0000[0] == 0)
    {
        void* obj = reinterpret_cast<void*>(a2->field_0004);
        typedef int(__fastcall * FinishFn)(void*, void*, S_00691860*);
        FinishFn fn = reinterpret_cast<FinishFn>(CSF_AT(void**, obj, 0)[0x7C / 4]);
        int result = fn(obj, nullptr, a2);
        if ((result & 0xFF) != 0)
        {
            a2->pad_0000[0] = 0;
            return result;
        }
    }

    a2->pad_0000[0] = 1;
    return 1;
}

HOOK(0x004E6430, Archive_SerializeThreeFields, "auto");
