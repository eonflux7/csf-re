#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwD3D9CreateVertexShader, 0x0062C060, (int a1, int a2));
CSF_FUNC(int, __cdecl, RwD3D9CreatePixelShader, 0x0062C0C0, (int a1, int a2));
CSF_FUNC(void*, __cdecl, RwD3D9GetCaps, 0x0062C110, ());
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
// __thiscall callee (ecx = this), pops 12 bytes
CSF_FUNC(int, __fastcall, Water_LoadShaderBlob, 0x00709810, (void* self, void* edx, int path, int* outBlob, int profile));

static bool __fastcall Water_CreateShaders(void* self, void* /*edx*/)
{
    int globalInt = GetGlobalInt();
    LookupObjectById(reinterpret_cast<void*>(globalInt), nullptr);

    void* caps = RwD3D9GetCaps();
    bool supported = caps != nullptr
        && CSF_AT(uint32_t, caps, 0xC4) >= 0xFFFE0200u
        && CSF_AT(uint32_t, caps, 0xCC) >= 0xFFFE0200u;

    int blob = 0;
    if (!supported)
        return false;

    if (!(Water_LoadShaderBlob(self, nullptr,
            reinterpret_cast<int>(CSF_STR(0x008634F4, "Gfx/Shaders/Water.vsh")),
            &blob,
            reinterpret_cast<int>(CSF_STR(0x008474D8, "vs_2_0"))) & 0xFF))
        return false;

    {
        void** vt = CSF_AT(void**, reinterpret_cast<void*>(blob), 0);
        int code = ((int(__stdcall*)(void*))vt[3])(reinterpret_cast<void*>(blob));
        RwD3D9CreateVertexShader(code, static_cast<int>(reinterpret_cast<intptr_t>(static_cast<uint8_t*>(self) + 0x100)));
        vt = CSF_AT(void**, reinterpret_cast<void*>(blob), 0);
        ((int(__stdcall*)(void*))vt[2])(reinterpret_cast<void*>(blob));
    }

    blob = 0;
    if (!(Water_LoadShaderBlob(self, nullptr,
            reinterpret_cast<int>(CSF_STR(0x008634DC, "Gfx/Shaders/Water.psh")),
            &blob,
            reinterpret_cast<int>(CSF_STR(0x00847570, "ps_2_0"))) & 0xFF))
        return false;

    {
        void** vt = CSF_AT(void**, reinterpret_cast<void*>(blob), 0);
        int code = ((int(__stdcall*)(void*))vt[3])(reinterpret_cast<void*>(blob));
        RwD3D9CreatePixelShader(code, static_cast<int>(reinterpret_cast<intptr_t>(static_cast<uint8_t*>(self) + 0x104)));
        vt = CSF_AT(void**, reinterpret_cast<void*>(blob), 0);
        ((int(__stdcall*)(void*))vt[2])(reinterpret_cast<void*>(blob));
    }

    return supported;
}

HOOK(0x00709990, Water_CreateShaders, "auto");
