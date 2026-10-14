#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, Vehicle_SerializeBaseValue, 0x0040D5E0, (int* a1, int a2));
CSF_FUNC(int, __fastcall, Vehicle_SerializeEmbeddedState, 0x0040F990, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, StateNode_UpdateBase, 0x0041E140, (C_0082e520* self, void* , int a2));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));

static int __fastcall Vehicle_SerializeBaseState(C_0082de40* self, void* /*edx*/, int a2)
{
    uint8_t* ar = reinterpret_cast<uint8_t*>(a2);
    void* stream = CSF_AT(void*, ar, 4);
    void** streamVtbl = CSF_AT(void**, stream, 0);

    uint8_t errorFlag;
    if (CSF_AT(uint8_t, ar, 0) != 0) {
        errorFlag = 0;
    } else {
        char r = ((char(__fastcall*)(void*, void*, int, void*))streamVtbl[0x80 / 4])(stream, nullptr, 0, ar);
        errorFlag = (r != 0) ? 0 : 1;
    }

    int savedField1c = CSF_AT(int, ar, 0x1C);
    CSF_AT(uint8_t, ar, 0) = errorFlag;

    StateNode_UpdateBase(reinterpret_cast<C_0082e520*>(self), nullptr, a2);

    bool failed = CSF_AT(uint8_t, ar, 0) != 0;
    CSF_AT(int, ar, 0x1C) = 0;
    if (!failed) {
        CSF_AT(uint8_t, ar, 0x15) = 0;
        CSF_AT(uint8_t, ar, 0x16) = 0;
        Vehicle_SerializeEmbeddedState(reinterpret_cast<uint8_t*>(self) + 0x20, nullptr, a2);
    }

    failed = CSF_AT(uint8_t, ar, 0) != 0;
    CSF_AT(int, ar, 0x1C) = 0;
    if (!failed) {
        CSF_AT(uint8_t, ar, 0x15) = 0;
        CSF_AT(uint8_t, ar, 0x16) = 0;
        Vehicle_SerializeBaseValue(reinterpret_cast<int*>(reinterpret_cast<uint8_t*>(self) + 0x10), a2);
    }

    failed = CSF_AT(uint8_t, ar, 0) != 0;
    CSF_AT(int, ar, 0x1C) = 0;
    if (!failed) {
        CSF_AT(uint8_t, ar, 0x15) = 1;
        CSF_AT(uint8_t, ar, 0x16) = 0;
        Archive_SerializeObjectRef(reinterpret_cast<uint8_t*>(self) + 0x30, reinterpret_cast<bool*>(ar));
    }

    CSF_AT(int, ar, 0x1C) = savedField1c;

    if (CSF_AT(uint8_t, ar, 0) == 0) {
        stream = CSF_AT(void*, ar, 4);
        streamVtbl = CSF_AT(void**, stream, 0);
        int r = ((int(__fastcall*)(void*, void*, int))streamVtbl[0x7C / 4])(stream, nullptr, a2);
        if (static_cast<uint8_t>(r) != 0) {
            CSF_AT(uint8_t, ar, 0) = 0;
            return r;
        }
    }
    CSF_AT(uint8_t, ar, 0) = 1;
    return 1;
}

HOOK(0x0040FA60, Vehicle_SerializeBaseState, "auto");
