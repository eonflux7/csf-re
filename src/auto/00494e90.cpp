#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(C_00833908*, __fastcall, CCharActionFsm_SetActionMode, 0x00484420, (CCharActionFsm* self, void* , char a2));
CSF_FUNC(C_00833908*, __fastcall, CCharActionFsm_ProcessCurrentItem, 0x004844C0, (CCharActionFsm* self, void*));
CSF_FUNC(int, __fastcall, CCharActionFsm_GetSlot28ItemIfUsable, 0x00487260, (CCharActionFsm* self, void*));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(bool, __fastcall, WeaponFirearms_ForwardToAssociatedObject, 0x004762C0, (void* self, void*));

static int __fastcall CharActionFsm_SetCharacterItem(
    void* self, void* /*edx*/, int itemState)
{
    if (CSF_AT(int, self, 0x398) == itemState)
        return itemState;

    if (CSF_AT(std::uint8_t, self, 0x228) &&
        CSF_AT(void*, self, 0x39C))
    {
        auto* actionFsm = CSF_AT(CCharActionFsm*, self, 0x44);
        CCharActionFsm_SetActionMode(actionFsm, nullptr, 1);

        alignas(8) std::uint8_t matrixBytes[0x8];

        if (!CSF_AT(int, self, 0x398))
        {
            void* item = CSF_AT(void*, self, 0x39C);
            void* destination =
                ((void* (__fastcall*)(void*, void*))
                 CSF_AT(void**, item, 0)[0x18 / 4])(item, nullptr);

            CSF_AT(std::uint32_t*, matrixBytes, 0) = RwMatrixCreate();
            CSF_AT(std::uint8_t, matrixBytes, 4) = 1;
            ((void (__fastcall*)(void*, void*, void*))
             CSF_AT(void**, destination, 0)[0x94 / 4])(
                destination, nullptr, matrixBytes);
            if (CSF_AT(std::uint8_t, matrixBytes, 4))
                RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixBytes, 0));

            item = CSF_AT(void*, self, 0x22C);
            void* source =
                ((void* (__fastcall*)(void*, void*))
                 CSF_AT(void**, item, 0)[0x18 / 4])(item, nullptr);
            item = CSF_AT(void*, self, 0x39C);
            destination =
                ((void* (__fastcall*)(void*, void*))
                 CSF_AT(void**, item, 0)[0x18 / 4])(item, nullptr);
            void** sourceVtable = CSF_AT(void**, source, 0);
            void** destinationVtable = CSF_AT(void**, destination, 0);
            int value =
                ((int (__fastcall*)(void*, void*))
                 sourceVtable[0x10 / 4])(source, nullptr);
            ((void (__fastcall*)(void*, void*, int))
             destinationVtable[0x14 / 4])(destination, nullptr, value);

            item = CSF_AT(void*, self, 0x22C);
            source =
                ((void* (__fastcall*)(void*, void*))
                 CSF_AT(void**, item, 0)[0x18 / 4])(item, nullptr);
            item = CSF_AT(void*, self, 0x39C);
            destination =
                ((void* (__fastcall*)(void*, void*))
                 CSF_AT(void**, item, 0)[0x18 / 4])(item, nullptr);
            sourceVtable = CSF_AT(void**, source, 0);
            destinationVtable = CSF_AT(void**, destination, 0);
            float scalar =
                ((float (__fastcall*)(void*, void*))
                 sourceVtable[0x28 / 4])(source, nullptr);
            ((void (__fastcall*)(void*, void*, float))
             destinationVtable[0x34 / 4])(destination, nullptr, scalar);

            item = CSF_AT(void*, self, 0x39C);
            ((void (__fastcall*)(void*, void*, int))
             CSF_AT(void**, item, 0)[0x34 / 4])(item, nullptr, 1);
            item = CSF_AT(void*, self, 0x22C);
            ((void (__fastcall*)(void*, void*, int))
             CSF_AT(void**, item, 0)[0x34 / 4])(item, nullptr, 0);
        }
        else
        {
            void* item = CSF_AT(void*, self, 0x22C);
            void* destination =
                ((void* (__fastcall*)(void*, void*))
                 CSF_AT(void**, item, 0)[0x18 / 4])(item, nullptr);

            CSF_AT(std::uint32_t*, matrixBytes, 0) = RwMatrixCreate();
            CSF_AT(std::uint8_t, matrixBytes, 4) = 1;
            ((void (__fastcall*)(void*, void*, void*))
             CSF_AT(void**, destination, 0)[0x94 / 4])(
                destination, nullptr, matrixBytes);
            if (CSF_AT(std::uint8_t, matrixBytes, 4))
                RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixBytes, 0));

            item = CSF_AT(void*, self, 0x39C);
            void* source =
                ((void* (__fastcall*)(void*, void*))
                 CSF_AT(void**, item, 0)[0x18 / 4])(item, nullptr);
            item = CSF_AT(void*, self, 0x22C);
            destination =
                ((void* (__fastcall*)(void*, void*))
                 CSF_AT(void**, item, 0)[0x18 / 4])(item, nullptr);
            void** sourceVtable = CSF_AT(void**, source, 0);
            void** destinationVtable = CSF_AT(void**, destination, 0);
            int value =
                ((int (__fastcall*)(void*, void*))
                 sourceVtable[0x10 / 4])(source, nullptr);
            ((void (__fastcall*)(void*, void*, int))
             destinationVtable[0x14 / 4])(destination, nullptr, value);

            item = CSF_AT(void*, self, 0x39C);
            source =
                ((void* (__fastcall*)(void*, void*))
                 CSF_AT(void**, item, 0)[0x18 / 4])(item, nullptr);
            item = CSF_AT(void*, self, 0x22C);
            destination =
                ((void* (__fastcall*)(void*, void*))
                 CSF_AT(void**, item, 0)[0x18 / 4])(item, nullptr);
            sourceVtable = CSF_AT(void**, source, 0);
            destinationVtable = CSF_AT(void**, destination, 0);
            float scalar =
                ((float (__fastcall*)(void*, void*))
                 sourceVtable[0x28 / 4])(source, nullptr);
            ((void (__fastcall*)(void*, void*, float))
             destinationVtable[0x34 / 4])(destination, nullptr, scalar);

            item = CSF_AT(void*, self, 0x22C);
            ((void (__fastcall*)(void*, void*, int))
             CSF_AT(void**, item, 0)[0x34 / 4])(item, nullptr, 1);
            item = CSF_AT(void*, self, 0x39C);
            ((void (__fastcall*)(void*, void*, int))
             CSF_AT(void**, item, 0)[0x34 / 4])(item, nullptr, 0);
        }

        CSF_AT(int, self, 0x398) = itemState;
        CCharActionFsm_ProcessCurrentItem(actionFsm, nullptr);
    }

    void** characterVtable = CSF_AT(void**, self, 0);
    CSF_AT(int, self, 0x398) = itemState;

    // SETE replaces only AL; the other bytes of the argument remain intact.
    int notification =
        (itemState & ~0xFF) | static_cast<int>(itemState == 0);
    ((void (__fastcall*)(void*, void*, int))
     characterVtable[0x6C / 4])(self, nullptr, notification);

    int usableItem = CCharActionFsm_GetSlot28ItemIfUsable(
        CSF_AT(CCharActionFsm*, self, 0x44), nullptr);
    int result = CSF_AT(int, self, 0x398);
    if (!result && usableItem)
    {
        void* item = reinterpret_cast<void*>(usableItem);
        bool associatedObjectReady =
            WeaponFirearms_ForwardToAssociatedObject(item, nullptr);
        if (associatedObjectReady)
            return ((int (__fastcall*)(void*, void*))
                    CSF_AT(void**, item, 0)[0x3C / 4])(item, nullptr);
        return 0;
    }
    return result;
}

HOOK(0x00494E90, CharActionFsm_SetCharacterItem, "auto");
