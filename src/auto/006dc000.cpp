#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SaveGame_DestroyLocalizedString, 0x00428E90, (void* self, void*));
CSF_FUNC(int, __cdecl, RwEngineSetSubSystem, 0x00626B30, (int a1));
CSF_FUNC(int, __cdecl, RwEngineGetNumVideoModes, 0x00626B90, ());
CSF_FUNC(int, __cdecl, RwEngineGetVideoModeInfo, 0x00626BE0, (int a1, int a2));
CSF_FUNC(int, __cdecl, RwEngineSetVideoMode, 0x00626C40, (int a1));
CSF_FUNC(int, __cdecl, RwEngineOpen, 0x00627290, (int a1));
CSF_FUNC(int, __cdecl, Text_ConvertToDisplayEncoding, 0x00692580, (int text, int flag));
CSF_FUNC(void*, __cdecl, CoreParser_FormatDescriptionCount, 0x006925A0, (void* outString, const char* description, int count, int formattingArgument, int entryFlag));
CSF_FUNC(int, __cdecl, AppMain_ReportInitializationError, 0x0069CDF0, (const char* caption, char* format, ...));
CSF_FUNC(char, __cdecl, Dialog_ShowLocalizedMessage, 0x0069CE50, (const wchar_t* lpCaption, wchar_t* text, char a3));
CSF_FUNC(int, __cdecl, ParticleSystemDefs_ConvertAndShowMessage, 0x00701EC0, (int a1, int a2));

static bool __fastcall CRenderEngine_OpenAndSelectVideoMode(
    CRenderEngine* self, void* /*edx*/, int openParameters)
{
    if (!RwEngineOpen(openParameters))
    {
        ParticleSystemDefs_ConvertAndShowMessage(
            reinterpret_cast<int>(CSF_STR(0x008612F8, "g025")),
            reinterpret_cast<int>(CSF_STR(0x00861454, "g134")));
        return false;
    }

    self->field_0034 = 1;
    if (!RwEngineSetSubSystem(0))
    {
        AppMain_ReportInitializationError(
            CSF_STR(0x0083F688, "ERROR"),
            const_cast<char*>(CSF_STR(0x00861430, "RwEngineSetSubSystem() ha fallado")));
        return false;
    }

    if (!((bool(__fastcall*)(CRenderEngine*, void*, void*))
              self->vtbl[0x64 / 4])(
            self, nullptr, reinterpret_cast<uint8_t*>(self) + 4))
    {
        const uint32_t fallbackHeight = self->field_0010;
        const uint32_t fallbackWidth = self->field_000c;
        self->field_0004 = fallbackWidth;
        self->field_0008 = fallbackHeight;

        // The string header and its inline storage form one stack object.
        alignas(8) uint8_t descriptionBytes[0x1C];
        CSF_AT(uint32_t, descriptionBytes, 0x18) = 7;
        CSF_AT(uint32_t, descriptionBytes, 0x14) = 0;
        CSF_AT(uint16_t, descriptionBytes, 0x04) = 0;

        CoreParser_FormatDescriptionCount(
            descriptionBytes, CSF_STR(0x00861428, "g206"),
            static_cast<int>(fallbackWidth), static_cast<int>(fallbackHeight), 8);

        wchar_t* description =
            CSF_AT(uint32_t, descriptionBytes, 0x18) < 8
                ? reinterpret_cast<wchar_t*>(descriptionBytes + 4)
                : CSF_AT(wchar_t*, descriptionBytes, 0x04);

        const wchar_t* caption = reinterpret_cast<const wchar_t*>(
            Text_ConvertToDisplayEncoding(
                reinterpret_cast<int>(CSF_STR(0x008612F8, "g025")), 8));
        Dialog_ShowLocalizedMessage(caption, description, 0);
        SaveGame_DestroyLocalizedString(descriptionBytes, nullptr);
    }

    const int modeCount = RwEngineGetNumVideoModes();
    int selectedMode = -1;
    alignas(8) uint8_t videoModeBytes[0x18];

    for (int modeIndex = 0;
         modeIndex < modeCount && selectedMode == -1;
         ++modeIndex)
    {
        RwEngineGetVideoModeInfo(
            reinterpret_cast<int>(videoModeBytes), modeIndex);

        if (((bool(__fastcall*)(CRenderEngine*, void*, void*, void*))
                 self->vtbl[0x70 / 4])(
                self, nullptr, videoModeBytes,
                reinterpret_cast<uint8_t*>(self) + 4))
        {
            selectedMode = modeIndex;
        }
    }

    if (selectedMode == -1)
    {
        ParticleSystemDefs_ConvertAndShowMessage(
            reinterpret_cast<int>(CSF_STR(0x008612F8, "g025")),
            reinterpret_cast<int>(CSF_STR(0x00861420, "g026")));
        return false;
    }

    if (!RwEngineSetVideoMode(selectedMode))
    {
        ParticleSystemDefs_ConvertAndShowMessage(
            reinterpret_cast<int>(CSF_STR(0x008612F8, "g025")),
            reinterpret_cast<int>(CSF_STR(0x00861418, "g027")));
        return false;
    }

    return true;
}

HOOK(0x006DC000, CRenderEngine_OpenAndSelectVideoMode, "auto");
