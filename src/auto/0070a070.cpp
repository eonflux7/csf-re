#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, RwD3D9SetVertexDeclaration, 0x0062B720, (int a1));
CSF_FUNC(int, __cdecl, RwD3D9SetVertexShader, 0x0062B760, (int a1));
CSF_FUNC(int, __cdecl, RwD3D9SetPixelShader, 0x0062B7A0, (int a1));
CSF_FUNC(int, __cdecl, _rwD3D9SetStreams, 0x0062B840, (int a1, int a2));
CSF_FUNC(int, __cdecl, RwD3D9SetIndices, 0x0062B970, (int a1));
CSF_FUNC(int, __cdecl, RwD3D9DrawIndexedPrimitive, 0x0062B9A0, (int a1, int a2, int a3, int a4, int a5, int a6));
CSF_FUNC(int, __cdecl, RwD3D9SetRenderState, 0x0062D650, (int a1, int a2));
CSF_FUNC(int, __cdecl, RwD3D9SetTexture, 0x0062F950, (int tex, int stage));
CSF_FUNC(int, __cdecl, GfxSkin_GetDistanceOverrideOwner, 0x006B8840, (int a1));
CSF_FUNC(int, __fastcall, Water_GetTextureObject, 0x006D5FB0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, Water_UpdateShaderConstants, 0x00709BC0, (void* self, void* edx, int a2));

CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);

typedef void(__cdecl* RwEngineSetFn)(int, int);

static void CallEngineSetState(int a, int b)
{
    RwEngineSetFn fn = CSF_AT(RwEngineSetFn, g_pRwEngineInstance, 0x20);
    fn(a, b);
}

static int __cdecl Water_DrawMesh(uint32_t* meshData, int renderObj)
{
    int ownerInt = GfxSkin_GetDistanceOverrideOwner(renderObj);
    uint8_t* owner = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(ownerInt));
    uint8_t* obj = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(renderObj));

    RwD3D9SetVertexShader(CSF_AT(int, owner, 0x100));
    RwD3D9SetPixelShader(CSF_AT(int, owner, 0x104));
    Water_UpdateShaderConstants(owner, nullptr, renderObj);

    uint8_t* inner = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(int, obj, 0x18)));
    uint8_t* listPtr = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(int, inner, 0x20)));
    uint8_t* target = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(int, listPtr, 0)));

    void* texObj = reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(int, owner, 0xF8)));
    int tex = Water_GetTextureObject(texObj, nullptr);
    RwD3D9SetTexture(tex, 0);
    RwD3D9SetTexture(CSF_AT(int, owner, 0xD0), 1);
    CallEngineSetState(2, 3);

    uint8_t* flagsBase = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(int, target, 0)));
    CSF_AT(uint8_t, flagsBase, 0x51) = 0x33;

    if (CSF_AT(int, owner, 0xB4) == 0)
        CallEngineSetState(0x14, 2);
    else
        CallEngineSetState(0x14, 3);

    RwD3D9SetIndices(static_cast<int>(meshData[8]));
    _rwD3D9SetStreams(static_cast<int>(reinterpret_cast<uintptr_t>(meshData + 10)), static_cast<int>(meshData[18]));
    RwD3D9SetVertexDeclaration(static_cast<int>(meshData[19]));

    if (CSF_AT(int, owner, 0xB4) == 0)
    {
        CallEngineSetState(10, 5);
        CallEngineSetState(11, 6);
    }
    else
    {
        RwD3D9SetRenderState(27, 0);
    }

    int count = static_cast<int>(meshData[7]);
    if (count != 0)
    {
        uint32_t* p = meshData + 29;
        do
        {
            RwD3D9DrawIndexedPrimitive(static_cast<int>(meshData[9]),
                                       static_cast<int>(p[-2]),
                                       0,
                                       static_cast<int>(p[-1]),
                                       static_cast<int>(p[0]),
                                       static_cast<int>(p[1]));
            p += 9;
            --count;
        } while (count != 0);
    }

    RwD3D9SetVertexShader(0);
    RwD3D9SetPixelShader(0);
    return RwD3D9SetRenderState(27, 1);
}

HOOK(0x0070A070, Water_DrawMesh, "auto");
