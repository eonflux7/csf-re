#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(int, __fastcall, ColorAnim_ClearForControl, 0x00445580, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, CScene_RegisterObjectLink, 0x00452800, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, C_00867260_SetColor, 0x0048E790, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, SceneLoader_ProcessSceneObject, 0x004DE7C0, (void* self, void* edx, void* a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(int, __fastcall, CGuiControl_GetColorAnim, 0x004FE4B0, (CGuiControl* self, void*));

static int __fastcall CScene_AddObject(
    CScene* self, void* /*edx*/, C_00867260* object)
{
    const uint32_t count = CSF_AT(uint32_t, self, 0x18);
    uint32_t index = object->field_0050 != 0 ? 3 : 0;

    for (; index < count; ++index)
    {
        if (CSF_AT(C_00867260**, self, 0x10)[index] == nullptr)
        {
            object->field_0024 = index;
            CSF_AT(C_00867260**, self, 0x10)[index] = object;
            break;
        }
    }

    if (index >= count)
    {
        object->field_0024 = count;
        if (CSF_AT(uint32_t, self, 0x18) >=
            CSF_AT(uint32_t, self, 0x14))
        {
            // The packet also identifies this address as a vtable binding.
            (void)CSF_VTBL(0x0041E5F0);
            GrowButtonArray(
                reinterpret_cast<uint8_t*>(self) + 0x10, nullptr,
                CSF_AT(int, self, 0x1C));
        }

        const uint32_t appendIndex = CSF_AT(uint32_t, self, 0x18);
        C_00867260** slot =
            CSF_AT(C_00867260**, self, 0x10) + appendIndex;
        CSF_AT(uint32_t, self, 0x18) = appendIndex + 1;
        *slot = object;
    }

    const uint32_t objectType = object->field_0050;
    int registrationFlags = objectType == 0 ? 5 : (objectType == 13 ? 8 : 1);

    if ((CSF_AT(uint32_t, object, 0x170) & 0x2000) != 0)
    {
        // This field contains pointer bits despite its declared float type.
        uint32_t attachmentAddress;
        std::memcpy(&attachmentAddress, &object->field_0034,
                    sizeof(attachmentAddress));
        void* attachment = reinterpret_cast<void*>(attachmentAddress);
        if (attachment != nullptr &&
            reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, attachment, 0)[0x98 / 4])(
                    attachment, nullptr) == 1)
        {
            registrationFlags |= 0x20;
        }
    }

    CScene_RegisterObjectLink(
        self, nullptr, reinterpret_cast<int>(&object->field_001c),
        registrationFlags);

    if (object->field_0050 != 0)
        object->vSlot44(1);

    if (CSF_AT(uint8_t, self, 0x180) != 0 &&
        static_cast<uint8_t>(object->vGetObjectRegistry()) != 0)
    {
        float color[4] = {0.4f, 0.4f, 0.4f, 1.0f};
        C_00867260_SetColor(object, nullptr, reinterpret_cast<int>(color));
    }

    void* sceneLoader = CSF_AT(void*, self, 0x12C);
    if (sceneLoader != nullptr)
        SceneLoader_ProcessSceneObject(sceneLoader, nullptr, object);

    // The generated vSlot28 wrapper takes an argument, but this call does not.
    void* relatedObject =
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            object->vtbl[0x28 / 4])(object, nullptr);

    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, relatedObject, 0)[0x34 / 4])(
            relatedObject, nullptr, 1);

    CGuiControl* guiControl =
        reinterpret_cast<CGuiControl*>(GetGlobalValue_8B0568());
    void* colorAnimation =
        reinterpret_cast<void*>(CGuiControl_GetColorAnim(guiControl, nullptr));
    return ColorAnim_ClearForControl(
        colorAnimation, nullptr, reinterpret_cast<uint32_t*>(object));
}

HOOK(0x00453420, CScene_AddObject, "auto");
