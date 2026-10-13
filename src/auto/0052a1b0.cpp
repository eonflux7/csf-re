#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, MenuMessageBox_ResolveControl, 0x0052A040, (int source, char mode));
CSF_FUNC(int, __fastcall, ReleaseObjectReference, 0x006923C0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));

CSF_GLOBAL(const char*, g_psz_VinylD_1, 0x00899EAC);

static int __fastcall CMenuMessageBox_InitializeSlotButtons(
    CMenuMessageBox* self, void* /*edx*/, int source)
{
    void* accessor = reinterpret_cast<void*>(
        GlobalInt_ResolveAccessor(reinterpret_cast<void*>(GetGlobalInt()), nullptr));
    void* textureManager =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, accessor, 0)[0x10 / 4])(accessor, nullptr);
    reinterpret_cast<void (__fastcall*)(void*, void*, const char*)>(
        CSF_AT(void**, textureManager, 0)[0x10 / 4])(
            textureManager, nullptr, CSF_STR(0x008308FC, "Gfx/Textures/"));

    alignas(8) uint8_t dimensionsBytes[0xC];
    int result = 0;

    for (unsigned int i = 0; i < 3; ++i)
    {
        void* controls = MenuControls_ResolveObject(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr);
        void* texture =
            reinterpret_cast<void* (__fastcall*)(void*, void*, const char*)>(
                CSF_AT(void**, controls, 0)[0x38 / 4])(
                    controls, nullptr, (&g_psz_VinylD_1)[i]);
        void* dimensions =
            reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, texture, 0)[0x3C / 4])(texture, nullptr);

        CSF_AT(uint32_t, dimensionsBytes, 0) = CSF_AT(uint32_t, dimensions, 0);
        CSF_AT(uint32_t, dimensionsBytes, 4) = CSF_AT(uint32_t, dimensions, 4);

        result = MenuMessageBox_ResolveControl(source, 1);
        CSF_AT(uint32_t, self, 0x6C + i * 4) = static_cast<uint32_t>(result);

        if (result)
        {
            void* button = reinterpret_cast<void*>(result);
            reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, button, 0)[0xA0 / 4])(
                    button, nullptr, dimensionsBytes);

            button = reinterpret_cast<void*>(
                CSF_AT(uint32_t, self, 0x6C + i * 4));
            reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, button, 0)[0xD8 / 4])(button, nullptr, texture);

            ReleaseObjectReference(static_cast<uint8_t*>(texture) + 4, nullptr);

            button = reinterpret_cast<void*>(
                CSF_AT(uint32_t, self, 0x6C + i * 4));
            reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, button, 0)[0xA8 / 4])(button, nullptr, 1);

            void* container = reinterpret_cast<void*>(self->field_0894);
            button = reinterpret_cast<void*>(
                CSF_AT(uint32_t, self, 0x6C + i * 4));
            result = reinterpret_cast<int (__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, container, 0)[0xF8 / 4])(
                    container, nullptr, button);
        }
    }

    return result;
}

HOOK(0x0052A1B0, CMenuMessageBox_InitializeSlotButtons, "auto");
