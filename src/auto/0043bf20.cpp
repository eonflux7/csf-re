#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Minimap_ResolveTarget, 0x0043B9A0, (void* self, void* edx, int a2, uint8_t* a3, uint8_t* a4));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, Minimap_ComputeOffset, 0x0043B8B0, (void* self, void* edx, float* a2, float* a3, float* a4, float a5, char a6, uint8_t* a7));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(float*, __fastcall, NormalizeVector2, 0x006AEE00, (void* self, void* edx));
CSF_FUNC(double, __fastcall, C_008342e8_ComputeResolvedObjectScalar, 0x0048F220, (C_008342e8* self, void*));
CSF_FUNC(uint32_t*, __fastcall, Minimap_BuildMarker, 0x0043AB60, (void* self, void* edx, int a2, int a3, uint32_t* a4, char a5, char a6));
CSF_FUNC(char, __fastcall, Minimap_ApplyMarker, 0x0043AF30, (void* self, void* edx, float* a2));

static char __fastcall Minimap_UpdateTargetMarker(C_00830e5c* self, void* /*edx*/, C_008342e8* arg0)
{
    alignas(8) uint8_t markerBytes[0x18];
    float vec[2];
    uint32_t flagB = 0;
    uint8_t flagA = 0;
    uint8_t outFlag = 0;

    uint8_t* ctx = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetSceneMenuContext(g_pCurrentScene, nullptr)));
    uint8_t* inner;
    if (ctx) {
        inner = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, ctx, 0x3C)));
    } else {
        inner = nullptr;
    }

    if (arg0 && ctx && inner && reinterpret_cast<uint8_t*>(arg0) != ctx) {
        flagA = 0;
        flagB = 1;
        int target = Minimap_ResolveTarget(self, nullptr, (int)(intptr_t)arg0, &flagA, reinterpret_cast<uint8_t*>(&flagB));
        if (target) {
            vec[1] = CSF_AT(float, inner, 0x34);
            outFlag = 0;
            float* posArg = GetObjectPositionVector(arg0, nullptr);
            float* posCtx = GetObjectPositionVector(ctx, nullptr);
            Minimap_ComputeOffset(self, nullptr, &vec[0], posCtx, posArg, vec[1], (char)flagA, &outFlag);
            if (outFlag) {
                GetValue8B0554();
                double lenSq = (double)(vec[1] * vec[1] + vec[0] * vec[0]);
                double len = __builtin_elementwise_sqrt(lenSq);
                if (len > (double)CSF_AT(float, inner, 0x34)) {
                    NormalizeVector2(vec, nullptr);
                    float s = CSF_AT(float, inner, 0x34);
                    vec[0] = vec[0] * s;
                    vec[1] = vec[1] * s;
                }
                float scalar = (float)C_008342e8_ComputeResolvedObjectScalar(arg0, nullptr);
                int scalarBits;
                std::memcpy(&scalarBits, &scalar, sizeof(scalarBits));
                Minimap_BuildMarker(markerBytes, nullptr, target, scalarBits,
                                    reinterpret_cast<uint32_t*>(vec), (char)flagB, 1);
                return Minimap_ApplyMarker(self, nullptr, reinterpret_cast<float*>(markerBytes));
            }
        }
    }
    return 0;
}

HOOK(0x0043BF20, Minimap_UpdateTargetMarker, "auto");
