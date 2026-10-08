#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static void* MenuGameOptions_FindControl(void* container, const char* name)
{
    return reinterpret_cast<void* (__fastcall*)(void*, void*, const char*)>(
        CSF_AT(void**, container, 0)[0xFC / 4])(container, nullptr, name);
}

static int MenuGameOptions_GetControlKind(void* control)
{
    return reinterpret_cast<int (__fastcall*)(void*, void*)>(
        CSF_AT(void**, control, 0)[0x60 / 4])(control, nullptr);
}

static void MenuGameOptions_AddControlOption(void* control, uint32_t code)
{
    reinterpret_cast<void (__fastcall*)(void*, void*, uint32_t)>(
        CSF_AT(void**, control, 0)[0x29C / 4])(control, nullptr, code);
}

// ECX already contains self. This thunk does not introduce a call frame.
static __declspec(naked) int __fastcall CMenuGameOptions_ContinueInitialization(
    CMenuGameOptions* /*self*/, void* /*edx*/)
{
    __asm {
        mov edx, dword ptr [ecx]
        jmp dword ptr [edx + 0x64]
    }
}

static int __fastcall CMenuGameOptions_InitializeOptionControls(
    CMenuGameOptions* self, void* /*edx*/)
{
    void* muteVoice = MenuGameOptions_FindControl(
        reinterpret_cast<void*>(self->field_001c),
        CSF_STR(0x00842724, "BotonMuteVoice"));

    if (muteVoice && MenuGameOptions_GetControlKind(muteVoice) == 7) {
        self->field_003c = reinterpret_cast<uint32_t>(muteVoice);
        MenuGameOptions_AddControlOption(muteVoice, 0x47453135);
        MenuGameOptions_AddControlOption(
            reinterpret_cast<void*>(self->field_003c), 0x47453134);
    }

    void* voiceOutput = MenuGameOptions_FindControl(
        reinterpret_cast<void*>(self->field_001c),
        CSF_STR(0x00842710, "BotonVoiceOutput"));
    self->field_0040 = reinterpret_cast<uint32_t>(voiceOutput);

    if (voiceOutput && MenuGameOptions_GetControlKind(voiceOutput) == 7) {
        MenuGameOptions_AddControlOption(
            reinterpret_cast<void*>(self->field_0040), 0x47493031);
        MenuGameOptions_AddControlOption(
            reinterpret_cast<void*>(self->field_0040), 0x47493035);
        MenuGameOptions_AddControlOption(
            reinterpret_cast<void*>(self->field_0040), 0x47493034);
        MenuGameOptions_AddControlOption(
            reinterpret_cast<void*>(self->field_0040), 0x47493033);
    }

    void* voiceOutputLabel = MenuGameOptions_FindControl(
        reinterpret_cast<void*>(self->field_001c),
        CSF_STR(0x008426FC, "StaticVoiceOutput"));
    void* container = reinterpret_cast<void*>(self->field_001c);
    self->field_004c = reinterpret_cast<uint32_t>(voiceOutputLabel);

    void* liveStatus = MenuGameOptions_FindControl(
        container, CSF_STR(0x008426EC, "BotonLiveStatus"));
    self->field_0044 = reinterpret_cast<uint32_t>(liveStatus);
    if (liveStatus) {
        MenuGameOptions_AddControlOption(liveStatus, 0x47493037);
        MenuGameOptions_AddControlOption(
            reinterpret_cast<void*>(self->field_0044), 0x47493038);
    }

    void* muteVoiceLabel = MenuGameOptions_FindControl(
        reinterpret_cast<void*>(self->field_001c),
        CSF_STR(0x008426DC, "StaticMuteVoice"));
    container = reinterpret_cast<void*>(self->field_001c);
    self->field_0048 = reinterpret_cast<uint32_t>(muteVoiceLabel);

    void* subtitles = MenuGameOptions_FindControl(
        container, CSF_STR(0x008426CC, "BotonSubtitles"));
    self->field_0034 = reinterpret_cast<uint32_t>(subtitles);
    if (subtitles) {
        MenuGameOptions_AddControlOption(subtitles, 0x534F3130);
        MenuGameOptions_AddControlOption(
            reinterpret_cast<void*>(self->field_0034), 0x534F3131);
    }

    void* invertAxis = MenuGameOptions_FindControl(
        reinterpret_cast<void*>(self->field_001c),
        CSF_STR(0x008426BC, "BotonInvertAxis"));
    self->field_0038 = reinterpret_cast<uint32_t>(invertAxis);
    if (invertAxis) {
        MenuGameOptions_AddControlOption(invertAxis, 0x534F3130);
        MenuGameOptions_AddControlOption(
            reinterpret_cast<void*>(self->field_0038), 0x534F3131);
    }

    [[clang::musttail]]
    return CMenuGameOptions_ContinueInitialization(self, nullptr);
}

HOOK(0x0053B010, CMenuGameOptions_InitializeOptionControls, "auto");
