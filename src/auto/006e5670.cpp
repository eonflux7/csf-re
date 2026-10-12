#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, RpAtomicGetWorldBoundingSphere, 0x0058EC60, (int a1));
CSF_FUNC(char, __cdecl, GfxSkin_ApplyDistanceFade, 0x006C5430, (void* sceneObject, void* viewObject));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static int __fastcall GfxObjectInstances_IsVisible(void* self, void* /*edx*/)
{
    int visibilityMode = 8;
    void* model = CSF_AT(void*, self, 0x10);
    const std::uint32_t* flags =
        reinterpret_cast<const std::uint32_t* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, model, 0)[0x20 / 4])(model, nullptr);

    if (((*flags >> 8) & 0x80u) != 0)
        visibilityMode = 10;

    model = CSF_AT(void*, self, 0x10);
    char distanceVisible = 1;
    if (CSF_AT(std::uint8_t, model, 0xBC) &&
        (CSF_AT(float, model, 0xC0) > g_fZero ||
         CSF_AT(float, model, 0xC4) > g_fZero))
    {
        void* context = reinterpret_cast<void*>(GetGlobalValue_6C5760());
        void* view = CSF_AT(void*, context, 0xF0);
        void* atomic = CSF_AT(void*, self, 0x0C);
        distanceVisible = GfxSkin_ApplyDistanceFade(atomic, view);
    }

    if (CSF_AT(std::uint8_t, self, 0x19) && distanceVisible)
    {
        void* context = reinterpret_cast<void*>(GetGlobalValue_6C5760());
        int atomic = CSF_AT(int, self, 0x0C);
        void* view = CSF_AT(void*, context, 0xF0);
        int sphere = RpAtomicGetWorldBoundingSphere(atomic);

        if (reinterpret_cast<unsigned char (__fastcall*)(void*, void*, int, void*, int)>(
                CSF_AT(void**, view, 0)[0x6C / 4])(
                view, nullptr, sphere,
                reinterpret_cast<std::uint8_t*>(self) + 0x44,
                visibilityMode))
        {
            return 1;
        }
    }

    return 0;
}

HOOK(0x006E5670, GfxObjectInstances_IsVisible, "auto");
