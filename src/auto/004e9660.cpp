#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, CCharActionFsm_BindActionRecord, 0x00489470, (CCharActionFsm* self, void* , int a2, int a3));
CSF_FUNC(int, __fastcall, HandleTargetUnavailable, 0x0048FE90, (void* self, void* edx));

static char __fastcall S_004e9090_InitializeCharacterActions(
    S_004e9090* self, void* /*edx*/, C_00833908* character,
    int setupIndex, char activate)
{
    void* setups = reinterpret_cast<void*>(self->field_1fc8);
    void* setup =
        reinterpret_cast<void* (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, setups, 0)[8 / 4])(setups, nullptr, setupIndex);

    if (!character->field_0044)
    {
        if (CSF_AT(uint32_t, setup, 0x118))
            return static_cast<char>(HandleTargetUnavailable(character, nullptr));
        return 0;
    }

    if (!activate)
    {
        if (CSF_AT(uint32_t, setup, 0xB4))
            return 0;

        const uint32_t count = CSF_AT(uint32_t, setup, 0x118);
        uint32_t index = 0;
        while (index < count)
        {
            void* actions = CSF_AT(void*, self, 0x1FC0);
            const int handle = CSF_AT(int*, setup, 0x110)[index];
            const int actionIndex =
                reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, actions, 0)[12 / 4])(
                        actions, nullptr, handle);

            if (actionIndex != -1)
            {
                CCharActionFsm_BindActionRecord(
                    reinterpret_cast<CCharActionFsm*>(character->field_0044),
                    nullptr, actionIndex, -1);
            }
            ++index;
        }
        return static_cast<char>(index);
    }

    const uint32_t count = CSF_AT(uint32_t, setup, 0x118);
    uint32_t index = 0;
    while (index < count)
    {
        void* actions = CSF_AT(void*, self, 0x1FC0);
        const int handle = CSF_AT(int*, setup, 0x110)[index];
        const int actionIndex =
            reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, actions, 0)[12 / 4])(
                    actions, nullptr, handle);

        if (actionIndex != -1)
        {
            void* actionFsm = reinterpret_cast<void*>(character->field_0044);
            reinterpret_cast<void (__fastcall*)(void*, void*, int, int)>(
                CSF_AT(void**, actionFsm, 0)[168 / 4])(
                    actionFsm, nullptr, actionIndex, 0);

            actions = CSF_AT(void*, self, 0x1FC0);
            void* action =
                reinterpret_cast<void* (__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, actions, 0)[8 / 4])(
                        actions, nullptr, actionIndex);

            if (CSF_AT(int, action, 0x74) == 22)
            {
                const int currentHandle = CSF_AT(int*, setup, 0x110)[index];
                actionFsm = reinterpret_cast<void*>(character->field_0044);
                void** savedVtable = CSF_AT(void**, actionFsm, 0);

                const int itemIndex =
                    reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                        savedVtable[196 / 4])(
                            actionFsm, nullptr, currentHandle);
                void* entry =
                    reinterpret_cast<void* (__fastcall*)(void*, void*, int)>(
                        savedVtable[200 / 4])(
                            actionFsm, nullptr, itemIndex);
                void* item =
                    reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                        CSF_AT(void**, entry, 0)[168 / 4])(entry, nullptr);

                reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, item, 0)[52 / 4])(item, nullptr, 0);
                reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, item, 0)[60 / 4])(item, nullptr, 0);

                void* owner =
                    reinterpret_cast<void*>(character->vGetField016c());
                reinterpret_cast<void (__fastcall*)(void*, void*, void*, int, int)>(
                    CSF_AT(void**, owner, 0)[160 / 4])(
                        owner, nullptr, item, 60, 0);
            }
        }
        ++index;
    }
    return static_cast<char>(index);
}

HOOK(0x004E9660, S_004e9090_InitializeCharacterActions, "auto");
