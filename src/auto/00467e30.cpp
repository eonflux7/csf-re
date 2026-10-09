#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, InitGlobalRequest_405A00, 0x00405A00, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, sub_405A50, 0x00405A50, (void* self, void* edx, int a2, int a3));
CSF_FUNC(bool, __fastcall, SceneObject_QueryState, 0x00444900, (void* self, void* edx, void* a1));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, sub_45A960, 0x0045A960, (void* self, void* edx));
CSF_FUNC(char, __fastcall, IsTrackedHandleActive, 0x0045BDA0, (S_0045bda0* self, void*));
CSF_FUNC(char, __fastcall, Sub467CC0, 0x00467CC0, (void* self, void* edx, unsigned int a2));
CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, sub_4F5740, 0x004F5740, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(int, __fastcall, SubmitOrderRequest, 0x005242B0, (void* self, void* edx, void* a2, int a3));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ResolveSlotIndexForState, 0x00756810, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_FloatMax, 0x00831090);
CSF_GLOBAL(const char*, g_pszMenu_P_Help, 0x00899AA4);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void __fastcall MissionScript_UpdatePendingAction(
    void* self, void* /*edx*/, float elapsedTime)
{
    if (!IsTrackedHandleActive(reinterpret_cast<S_0045bda0*>(self), nullptr))
        return;

    const uint32_t action = CSF_AT(uint32_t, self, 0x18) - 1;
    CSF_AT(float, self, 0x14) = elapsedTime + CSF_AT(float, self, 0x14);

    switch (action)
    {
    case 0:
    {
        auto* object = reinterpret_cast<C_00833b88*>(
            CHandleTable_Resolve(g_pCurrentScene, nullptr,
                reinterpret_cast<int>(static_cast<uint8_t*>(self) + 0x5C)));
        if (!object)
            break;
        if (static_cast<uint8_t>(SceneObj_IsInactive(object, nullptr)))
            break;

        auto* component = reinterpret_cast<C_008672e4*>(object->field_0034);
        auto* state = &component->field_0018;
        if (static_cast<uint8_t>(sub_45A960(state, nullptr)) &&
            (CSF_AT(uint8_t, state, 0) & 2) != 0)
            break;
        return;
    }

    case 2:
    {
        bool finished = false;
        if (CSF_AT(int, self, 0x64) >= 0)
        {
            void* object = reinterpret_cast<void*>(
                CHandleTable_Resolve(g_pCurrentScene, nullptr,
                    reinterpret_cast<int>(static_cast<uint8_t*>(self) + 0x5C)));
            finished = !object ||
                static_cast<uint8_t>(SceneObj_IsInactive(object, nullptr)) != 0;
        }
        if (!finished)
        {
            void* state = CSF_AT(void*, self, 0x70);
            finished = CSF_AT(uint32_t, state, 0x18) == 0;
        }
        if (!finished)
            return;

        CSF_AT(uint32_t, self, 0x70) = 0;
        CSF_AT(int, self, 0x60) = -1;
        CSF_AT(int, self, 0x64) = -1;
        break;
    }

    case 3:
    {
        auto* object = reinterpret_cast<C_00833b88*>(
            CHandleTable_Resolve(g_pCurrentScene, nullptr,
                reinterpret_cast<int>(static_cast<uint8_t*>(self) + 0x5C)));
        if (!object)
            break;
        if (static_cast<uint8_t>(SceneObj_IsInactive(object, nullptr)))
            break;

        void* component = reinterpret_cast<void*>(object->field_0030);
        if (reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                CSF_AT(void**, component, 0)[0xB8 / 4])(component, nullptr))
            break;
        return;
    }

    case 4:
    {
        auto* object = reinterpret_cast<C_00833b88*>(
            CHandleTable_Resolve(g_pCurrentScene, nullptr,
                reinterpret_cast<int>(static_cast<uint8_t*>(self) + 0x5C)));
        if (!object)
            break;
        if (static_cast<uint8_t>(SceneObj_IsInactive(object, nullptr)))
            break;

        auto* component = reinterpret_cast<C_008672e4*>(object->field_0034);
        if (!component)
            break;
        if (reinterpret_cast<int(__fastcall*)(void*, void*)>(
                component->vtbl[0x98 / 4])(component, nullptr) != 1)
            break;

        void* state = CSF_AT(void*,
            reinterpret_cast<C_008672e4*>(object->field_0034), 0x308);
        if (reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                CSF_AT(void**, state, 0)[0xA8 / 4])(state, nullptr))
            break;
        return;
    }

    case 5:
        if (!SceneObject_QueryState(
                CSF_AT(void*, g_pCurrentScene, 0xC0), nullptr,
                CSF_AT(void*, self, 0x74)))
            return;
        CSF_AT(int, self, 0x74) = -1;
        break;

    case 6:
    {
        void* manager = reinterpret_cast<void*>(GetGlobalValue_4FBDE0());
        void* first = reinterpret_cast<void*>(GetFirstElement(manager, nullptr));
        void* provider = GetCachedObjectProvider(first, nullptr);
        if (!sub_4F5740(provider, nullptr))
            return;
        break;
    }

    case 7:
    case 8:
    {
        void* pending = CSF_AT(void*, self, 0x7C);
        if (!pending)
            break;

        if (reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                CSF_AT(void**, pending, 0)[0x3C / 4])(pending, nullptr))
        {
            void* sceneState = CSF_AT(void*, g_pCurrentScene, 0xBC);
            if (!CSF_AT(uint8_t, sceneState, 0xAC))
                return;

            pending = CSF_AT(void*, self, 0x7C);
            const double remaining = reinterpret_cast<double(__fastcall*)(void*, void*)>(
                CSF_AT(void**, pending, 0)[0x38 / 4])(pending, nullptr);
            if (remaining == g_FloatMax)
                return;
        }

        pending = CSF_AT(void*, self, 0x7C);
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, pending, 0)[0x4C / 4])(pending, nullptr);
        CSF_AT(uint32_t, self, 0x7C) = 0;

        if (action == 8)
        {
            void* object = reinterpret_cast<void*>(
                CHandleTable_Resolve(g_pCurrentScene, nullptr,
                    reinterpret_cast<int>(static_cast<uint8_t*>(self) + 0x5C)));
            if (object)
            {
                uint32_t request[0x24 / 4] = {};
                int handle[0x14 / 4] = {};
                uint32_t argument[0x10 / 4] = {};

                InitGlobalRequest_405A00(request, nullptr);
                const int handleValue = CSF_AT(int, self, 0x60);
                const int handleGeneration = CSF_AT(int, self, 0x64);
                const int objectId = CSF_AT(int, object, 0x50);

                request[0] = 2;
                handle[1] = handleValue;
                handle[2] = handleGeneration;
                request[1] = 4;

                void* global = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
                request[2] = ResolveSlotIndexForState(
                    static_cast<uint8_t*>(global) + 0x22A4, nullptr, 1, objectId);

                uint32_t* orderArgument = sub_405A50(
                    argument, nullptr, reinterpret_cast<int>(handle), 1232348224);
                void* orderManager = reinterpret_cast<void*>(GetGlobalValue_521BB0());
                SubmitOrderRequest(orderManager, nullptr, request,
                    reinterpret_cast<int>(orderArgument));
                CSF_AT(void**, argument, 0) = CSF_VTBL(0x0082D410);
            }
        }
        break;
    }

    case 9:
        Sub467CC0(self, nullptr, CSF_AT(uint32_t, self, 0x1C));
        return;

    case 10:
    {
        void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
        void* menu = CSF_AT(void*, globalState, 0x14);
        void** menuVtable = CSF_AT(void**, menu, 0);
        const int entry = reinterpret_cast<int(__fastcall*)(void*, void*, const char*, int)>(
            menuVtable[0x5C / 4])(menu, nullptr, g_pszMenu_P_Help, 1);
        if (reinterpret_cast<bool(__fastcall*)(void*, void*, int)>(
                menuVtable[0x74 / 4])(menu, nullptr, entry))
            return;
        break;
    }

    case 11:
    {
        const float remaining = CSF_AT(float, self, 0x78) - elapsedTime;
        CSF_AT(float, self, 0x78) = remaining;
        if (!(remaining <= g_fZero))
            return;
        CSF_AT(float, self, 0x78) = 0.0f;
        break;
    }

    default:
        return;
    }

    ++CSF_AT(uint32_t, self, 0x1C);
    Sub467CC0(self, nullptr, CSF_AT(uint32_t, self, 0x1C));
}

HOOK(0x00467E30, MissionScript_UpdatePendingAction, "auto");
