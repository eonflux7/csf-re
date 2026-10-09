#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CGuiControl_ProcessDisplayFlags_453070, 0x00453070, (CGuiControl* self, void* , void* a2, float* a3, int* a4, int a5, float a6));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneContext_RefreshFlaggedSelection, 0x004A20D0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, Scene_IsObjectRemovable, 0x007558C0, (int a1));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(uint8_t, __fastcall, GlobalFlag_IsSet, 0x007A7B50, (void* ecxArg, void* edx));

static void __fastcall CPlayerCharacter_UpdateNearbySelection(
    CPlayerCharacter* self, void* /*edx*/)
{
    if (reinterpret_cast<uint32_t>(self) !=
        GetSceneMenuContext(g_pCurrentScene, nullptr))
        return;

    // The original comparison also lets an unordered value through.
    if (self->field_0220 < g_HalfDurationScale)
        return;

    self->field_0220 = 0.0f;
    self->field_02cc = -1;
    self->field_02d0 = -1;

    void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
    if (CSF_AT(uint8_t, state, 0xD0) &&
        GlobalFlag_IsSet(reinterpret_cast<void*>(self->field_03e8), nullptr))
    {
        SceneContext_RefreshFlaggedSelection(
            reinterpret_cast<void*>(self->field_03e8), nullptr);
        return;
    }

    void* settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    void* position = reinterpret_cast<void*>(GetObjectPositionVector(self, nullptr));
    float extent = CSF_AT(float, settings, 0xB10);
    float queryPosition[4];
    queryPosition[0] = CSF_AT(float, position, 0x0);
    queryPosition[1] = CSF_AT(float, position, 0x4);
    queryPosition[2] = CSF_AT(float, position, 0x8);
    queryPosition[3] = extent;
    float radius = CSF_AT(float, self, 0x1D4) * g_HalfDurationScale;

    int results[5] = {0, 0, 0, 10, 0};
    int filter[13] = {17, 6, -1, -1, 0, 0, 0, 10, 0, 0, 0, -1, 0};
    int queryArguments[3] = {5, 0, reinterpret_cast<int>(self)};

    CGuiControl_ProcessDisplayFlags_453070(
        reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr,
        results, queryPosition, filter,
        reinterpret_cast<int>(queryArguments), radius);

    results[4] = results[2];
    queryArguments[1] = 0;
    if (results[2] > 0)
    {
        CPlayerCharacter* currentSelf = self;
        do
        {
            auto* object = reinterpret_cast<C_00833908*>(
                reinterpret_cast<uint32_t*>(results[0])[queryArguments[1]]);
            int type = static_cast<int>(object->field_0050);
            void* controller = reinterpret_cast<void*>(currentSelf->field_0044);

            if (type == 23)
            {
                int id = CSF_AT(int, object, 0x184);
                if (reinterpret_cast<bool(__fastcall*)(void*, void*, int)>(
                        CSF_AT(void**, controller, 0)[0xE0 / 4])(
                        controller, nullptr, id))
                {
                    void* owner = reinterpret_cast<void*>(object->field_0034);
                    reinterpret_cast<void(__fastcall*)(void*, void*, int, int)>(
                        CSF_AT(void**, owner, 0)[0xCC / 4])(
                        owner, nullptr, id, 1);
                }
            }
            else if (type == 22)
            {
                int id = CSF_AT(int, object, 0x184);
                if (id == -1)
                {
                    void* owner = reinterpret_cast<void*>(object->field_0034);
                    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                        CSF_AT(void**, owner, 0)[0xC4 / 4])(
                        owner, nullptr, 1);
                }
                else
                {
                    int status =
                        reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                            CSF_AT(void**, controller, 0)[0xB0 / 4])(
                            controller, nullptr, id);
                    if (status == 1)
                    {
                        void* owner = reinterpret_cast<void*>(object->field_0034);
                        reinterpret_cast<void(__fastcall*)(void*, void*, int, int)>(
                            CSF_AT(void**, owner, 0)[0xC8 / 4])(
                            owner, nullptr, id, 1);
                    }
                    else if (status == 2)
                    {
                        auto* context =
                            reinterpret_cast<CPlayerCharacter*>(queryArguments[2]);
                        context->field_02cc = object->field_0020;
                        context->field_02d0 = object->field_0024;
                    }
                }
            }
            else if (static_cast<uint8_t>(Scene_IsObjectRemovable(type)))
            {
                void* related = reinterpret_cast<void*>(object->field_004c);
                if (related &&
                    !reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                        CSF_AT(void**, related, 0)[0x4C / 4])(
                        related, nullptr))
                {
                    int id = CSF_AT(int, object, 0x194);
                    if (reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                            CSF_AT(void**, controller, 0)[0xB0 / 4])(
                            controller, nullptr, id) == 1)
                    {
                        void* attachment =
                            reinterpret_cast<void*>(object->vGetField016c());
                        if (attachment &&
                            reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                                CSF_AT(void**, attachment, 0)[0xA8 / 4])(
                                attachment, nullptr))
                        {
                            reinterpret_cast<void(__fastcall*)(void*, void*)>(
                                CSF_AT(void**, attachment, 0)[0xA4 / 4])(
                                attachment, nullptr);
                        }

                        void* owner = reinterpret_cast<void*>(object->field_0034);
                        reinterpret_cast<void(__fastcall*)(void*, void*, int, int)>(
                            CSF_AT(void**, owner, 0)[0xC8 / 4])(
                            owner, nullptr, id, 1);
                    }
                }
            }

            ++queryArguments[1];
            if (queryArguments[1] >= results[4])
                break;
            currentSelf = reinterpret_cast<CPlayerCharacter*>(queryArguments[2]);
        } while (true);
    }

    if (filter[4])
    {
        Mem_Free(reinterpret_cast<void*>(filter[4]));
        filter[4] = 0;
    }
    if (results[0])
        Mem_Free(reinterpret_cast<void*>(results[0]));
}

HOOK(0x00497150, CPlayerCharacter_UpdateNearbySelection, "auto");
