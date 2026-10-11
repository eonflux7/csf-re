#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));
CSF_FUNC(int, __fastcall, Archive_SerializeBaseFields, 0x004DB4F0, (void* self, void* edx, int a2));

static int __fastcall S_0041f0a0_Serialize(S_0041f0a0* self, void* /*edx*/, S_0041f0a0* stream)
{
    uint8_t* selfBytes = reinterpret_cast<uint8_t*>(self);

    uint8_t newFlag = 0;
    if (CSF_AT(uint8_t, stream, 0) == 0) {
        void* target = CSF_AT(void*, stream, 4);
        uint8_t ok = (uint8_t)((int(__fastcall*)(void*, void*, int, void*))CSF_AT(void**, target, 0)[0x80 / 4])(target, nullptr, 0, stream);
        newFlag = (ok == 0) ? 1 : 0;
    }
    uint32_t savedField = CSF_AT(uint32_t, stream, 0x1C);
    CSF_AT(uint8_t, stream, 0) = newFlag;

    Archive_SerializeBaseFields(self, nullptr, (int)(intptr_t)stream);

    bool isZero = CSF_AT(uint8_t, stream, 0) == 0;
    CSF_AT(uint32_t, stream, 0x1C) = 0;
    if (isZero) {
        CSF_AT(uint8_t, stream, 0x15) = 0;
        CSF_AT(uint8_t, stream, 0x16) = 0;
        Archive_SerializeObjectRef(selfBytes + 0x10, reinterpret_cast<bool*>(stream));
    }

    isZero = CSF_AT(uint8_t, stream, 0) == 0;
    CSF_AT(uint32_t, stream, 0x1C) = 0;
    if (isZero) {
        void* target = CSF_AT(void*, stream, 4);
        CSF_AT(uint8_t, stream, 0x15) = 0;
        CSF_AT(uint8_t, stream, 0x16) = 0;
        int r = ((int(__fastcall*)(void*, void*, void*, void*))CSF_AT(void**, target, 0)[0x20 / 4])(target, nullptr, selfBytes + 0x14, stream);
        CSF_AT(uint8_t, stream, 0) = (r == -1) ? 1 : 0;
    }

    isZero = CSF_AT(uint8_t, stream, 0) == 0;
    CSF_AT(uint32_t, stream, 0x1C) = 0;
    if (isZero) {
        void* target = CSF_AT(void*, stream, 4);
        CSF_AT(uint8_t, stream, 0x15) = 0;
        CSF_AT(uint8_t, stream, 0x16) = 0;
        int r = ((int(__fastcall*)(void*, void*, void*, void*))CSF_AT(void**, target, 0)[0x58 / 4])(target, nullptr, selfBytes + 0x18, stream);
        CSF_AT(uint8_t, stream, 0) = (r == -1) ? 1 : 0;
    }

    isZero = CSF_AT(uint8_t, stream, 0) == 0;
    CSF_AT(uint32_t, stream, 0x1C) = savedField;
    if (isZero) {
        void* target = CSF_AT(void*, stream, 4);
        int result = ((int(__fastcall*)(void*, void*, void*))CSF_AT(void**, target, 0)[0x7C / 4])(target, nullptr, stream);
        if ((uint8_t)result != 0) {
            CSF_AT(uint8_t, stream, 0) = 0;
            return result;
        }
    }
    CSF_AT(uint8_t, stream, 0) = 1;
    return 1;
}

HOOK(0x0041F0A0, S_0041f0a0_Serialize, "auto");
