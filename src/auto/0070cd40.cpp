#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwTextureDestroy, 0x006317E0, (int texture));
CSF_FUNC(int, __cdecl, RwTextureRead, 0x006319C0, (int a1, int a2));
CSF_FUNC(char, __cdecl, NormalizeResourceName, 0x006949D0, (char* source, char* destination, unsigned int capacity));
CSF_FUNC(unsigned int, __cdecl, ResourceName_Transform, 0x00694B20, (char* a1, char* a2, char* a3, unsigned int a4));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));

typedef void* (__fastcall* VCallNoArg)(void* self, void* edx);
typedef void* (__fastcall* VCallOneArg)(void* self, void* edx, char* arg);

static bool __fastcall WaterShader_SetTexture(S_0070cd40* self, void* /*edx*/, char* source)
{
    char destination[256];
    char transformed[256];

    if (self->field_00d0) {
        RwTextureDestroy(static_cast<int>(self->field_00d0));
        self->field_00d0 = 0;
    }

    if (!*source)
        return true;

    // First chain: global -> accessor -> virtual slot 4 (result unused)
    {
        void* acc = reinterpret_cast<void*>(GlobalInt_ResolveAccessor(reinterpret_cast<void*>(GetGlobalInt()), nullptr));
        ((VCallNoArg)CSF_AT(void**, acc, 0)[4])(acc, nullptr);
    }

    ResourceName_Transform(source, (char*)CSF_ADDR_DATA(0x0082E65C), transformed, 256);

    {
        void* acc = reinterpret_cast<void*>(GlobalInt_ResolveAccessor(reinterpret_cast<void*>(GetGlobalInt()), nullptr));
        void* obj = ((VCallNoArg)CSF_AT(void**, acc, 0)[4])(acc, nullptr);
        ((VCallOneArg)CSF_AT(void**, obj, 0)[4])(obj, nullptr, transformed);
    }

    NormalizeResourceName(source, destination, 256);
    int tex = RwTextureRead(reinterpret_cast<int>(destination), 0);
    bool result = tex != 0;
    self->field_00d0 = static_cast<uint32_t>(tex);
    if (tex)
        CSF_AT(uint8_t, reinterpret_cast<void*>(tex), 0x50) = 4;
    return result;
}

HOOK(0x0070CD40, WaterShader_SetTexture, "auto");
