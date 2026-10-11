#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __cdecl, SceneSubObjectTest, 0x0041E380, (void* a1));
CSF_FUNC(char, __fastcall, AppendPathResult, 0x0041F040, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(void, __cdecl, PathQueryBegin, 0x004219C0, ());
CSF_FUNC(void, __cdecl, PathQueryEnd, 0x00421A00, ());
CSF_FUNC(int*, __fastcall, TaskContinue, 0x00422A40, (void* self, void* edx, int* arg0, char a3));
CSF_FUNC(uint32_t, __fastcall, CGuiControl_GetField012C, 0x0044F630, (CGuiControl* self, void*));
CSF_FUNC(int, __fastcall, HandleTargetUnavailable, 0x0048FE90, (void* self, void* edx));
CSF_FUNC(char, __fastcall, FindPath, 0x004E06C0, (void* self, void* edx, unsigned int a2, int a3, uint32_t* a4, char a5, int* a6, int a7));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, GetCurrentItemCount, 0x00681F40, (void* self, void* edx));
CSF_FUNC(void, __fastcall, ClearMember0C, 0x0075A1C0, (void* self, void*));
CSF_FUNC(int, __fastcall, AppendThreeDwordRecord, 0x0075A3A0, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, GridLookupSlotForVector, 0x0075C7C0, (void* self, void* edx, float* a2, int a3, unsigned int* a4));
CSF_FUNC(double, __fastcall, GetFacingAngleRadians, 0x0075D0A0, (void* self, void* edx, uint32_t* a2));

CSF_GLOBAL(void*, g_CurrentSceneObj, 0x008AFC90);
CSF_GLOBAL(uint32_t, g_PathBuffer, 0x00899504);
CSF_GLOBAL(uint32_t, g_PathCount, 0x0089950C);
CSF_GLOBAL(uint32_t, g_PathParam, 0x00899514);

static int* __fastcall CharTask_0082f9f8_PreparePath(C_0082f9f8* self, void* /*edx*/, void* a1, char a3)
{
    uint8_t* owner = CSF_AT(uint8_t*, self, 0x8);
    CSF_AT(uint8_t, self, 0x7C) = 0;
    ClearMember0C(owner + 0x25C, nullptr);

    if (SceneSubObjectTest(a1)) {
        g_PathCount = 0;
        g_PathParam = 0xBF800000u;
        PathQueryBegin();

        uint8_t* actor = CSF_AT(uint8_t*, self, 0x8);
        uint32_t f12c = CGuiControl_GetField012C(reinterpret_cast<CGuiControl*>(g_CurrentSceneObj), nullptr);
        int items = GetCurrentItemCount(reinterpret_cast<void*>(f12c), nullptr);
        char found = FindPath(reinterpret_cast<void*>(items), nullptr,
                              (unsigned int)(actor + 0x218), (int)a1, &g_PathBuffer, 0, nullptr, 0);
        CSF_AT(uint8_t, self, 0x7C) = found;
        PathQueryEnd();

        if (CSF_AT(uint8_t, self, 0x7C)) {
            if (g_PathCount > 1) {
                int offset = 0x14;
                unsigned int remaining = g_PathCount - 1;
                do {
                    uint32_t* rec = reinterpret_cast<uint32_t*>(offset + g_PathBuffer + 4);
                    AppendThreeDwordRecord(CSF_AT(uint8_t*, self, 0x8) + 0x25C, nullptr, rec);
                    offset += 0x14;
                    --remaining;
                } while (remaining != 0);
            }
            if (a3) {
                void* grid = reinterpret_cast<void*>(CSF_AT(int, reinterpret_cast<void*>(GetValue8B0554()), 0x1FD8));
                uint32_t query[3];
                uint32_t result[2];
                query[2] = 0xFFFFFFFFu;
                result[0] = 0xFFFFFFFFu;
                result[1] = 0xFFFFFFFFu;
                query[1] = 0;
                query[0] = 13;
                if ((uint8_t)GridLookupSlotForVector(grid, nullptr, reinterpret_cast<float*>(a1),
                                                     (int)query, result)) {
                    float angle = (float)GetFacingAngleRadians(grid, nullptr, result);
                    CSF_AT(float, CSF_AT(uint8_t*, self, 0x8), 0x208) = angle;
                } else {
                    a3 = 0;
                }
            }
        }
        AppendPathResult(CSF_AT(uint8_t*, self, 0x8) + 0x29C, nullptr, &g_PathBuffer);
    }

    if (!CSF_AT(uint8_t, self, 0x7C)) {
        HandleTargetUnavailable(CSF_AT(void*, CSF_AT(uint8_t*, self, 0x8), 0x14), nullptr);
    }
    return TaskContinue(self, nullptr, reinterpret_cast<int*>(a1), a3);
}

HOOK(0x00422E40, CharTask_0082f9f8_PreparePath, "auto");
