#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, sub_407F40, 0x00407F40, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_4084C0, 0x004084C0, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __cdecl, sub_40A430, 0x0040A430, (int a1));
CSF_FUNC(char, __fastcall, sub_418B40, 0x00418B40, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, HandleTargetUnavailable, 0x0048FE90, (void* self, void* edx));

static char __fastcall C_0082e2e8_InitWheelRecords(C_0082e2e8* self, void* /*edx*/, int a2, int a3)
{
    int param = a2;
    self->field_0004 = a2;

    uint32_t* handle = sub_40A430(9);
    self->field_0030 = reinterpret_cast<uint32_t>(handle);

    typedef int(__fastcall * VFn2)(void*, void*, int);
    char ok = (char)(((VFn2)CSF_AT(void**, handle, 0)[2])(handle, nullptr, a2) & 1);

    static const int slots[4] = {0, 2, 1, 3};
    for (int i = 0; i < 4; i++) {
        char r;
        if (sub_407F40(reinterpret_cast<void*>(self->field_0030), nullptr, slots[i])) {
            r = 1;
        } else {
            HandleTargetUnavailable(reinterpret_cast<void*>(self->field_0004), nullptr);
            r = 0;
        }
        ok = (char)(ok & r);
    }

    if (ok) {
        uint32_t index = 0;
        if (self->field_004c != 0) {
            uint32_t off = 0;
            do {
                void* table = reinterpret_cast<void*>(self->field_0030);
                int entryA = sub_4084C0(table,
                    nullptr,
                    (int)CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0044), off));
                table = reinterpret_cast<void*>(self->field_0030);
                int entryB = sub_4084C0(table,
                    nullptr,
                    (int)CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0034), off));

                typedef int(__fastcall * VFn6)(void*, void*);
                int resA = ((VFn6)CSF_AT(void**, entryA, 0)[6])(reinterpret_cast<void*>(entryA), nullptr);
                CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0044), off + 4) =
                    CSF_AT(uint32_t, resA, 0xC);

                int resB = ((VFn6)CSF_AT(void**, entryB, 0)[6])(reinterpret_cast<void*>(entryB), nullptr);
                CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0034), off + 4) =
                    CSF_AT(uint32_t, resB, 0xC);

                off += 32;
                ++index;
            } while (index < self->field_004c);
            param = a2;
        }
    }

    return (char)(sub_418B40(self, nullptr, param, a3) & ok);
}

HOOK(0x0041BBF0, C_0082e2e8_InitWheelRecords, "auto");
