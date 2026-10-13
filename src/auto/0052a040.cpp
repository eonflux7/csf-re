#include <cstddef>
#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, ReleaseObjectReference, 0x006923C0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));
CSF_FUNC(void, __fastcall, ReplaceDurationSource, 0x00720AF0, (void* self, void* , void* source));
CSF_FUNC(CGuiStatic*, __fastcall, CGuiStatic_Construct, 0x00720E10, (CGuiStatic* self, void* , int owner, int argument));

static CGuiStatic* __cdecl MenuMessageBox_ResolveControl(int control, char loadFont)
{
    if (!control)
        return nullptr;

    void* owner = MenuControls_ResolveObject(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);

    CGuiStatic* storage =
        reinterpret_cast<CGuiStatic*>(CoreMemory_AllocUntagged(352));
    CGuiStatic* result = nullptr;

    if (storage)
    {
        void* controlObject = reinterpret_cast<void*>(control);
        void* argumentObject =
            ((void* (__fastcall*)(void*, void*))
                CSF_AT(void**, controlObject, 0)[0x54 / 4])(
                    controlObject, nullptr);

        int argument =
            ((int (__fastcall*)(void*, void*))
                CSF_AT(void**, argumentObject, 0)[0x10 / 4])(
                    argumentObject, nullptr);

        result = CGuiStatic_Construct(
            storage, nullptr, reinterpret_cast<int>(owner), argument);
    }

    if (loadFont && result && owner)
    {
        void* font =
            ((void* (__fastcall*)(void*, void*, const char*))
                CSF_AT(void**, owner, 0)[0x4C / 4])(
                    owner, nullptr,
                    CSF_STR(0x0082FFC8, "Menus/Mgame_01.fnt"));

        if (font)
        {
            void* source =
                ((void* (__fastcall*)(void*, void*, void*))
                    CSF_AT(void**, owner, 0)[0x54 / 4])(
                        owner, nullptr, font);

            ReleaseObjectReference(font, nullptr);

            if (source)
            {
                ((void (__fastcall*)(void*, void*, int))
                    CSF_AT(void**, source, 0)[0x68 / 4])(
                        source, nullptr, 1);

                ((void (__fastcall*)(void*, void*, int))
                    CSF_AT(void**, source, 0)[0x6C / 4])(
                        source, nullptr, 1);

                ReplaceDurationSource(result, nullptr, source);
            }
        }
    }

    return result;
}

HOOK(0x0052A040, MenuMessageBox_ResolveControl, "auto");
