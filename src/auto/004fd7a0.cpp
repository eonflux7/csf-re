#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));
CSF_FUNC(unsigned int, __stdcall, Scene_SerializeNameList, 0x004E8060, (S_0041e5f0* a1, int a2));
CSF_FUNC(void, __fastcall, GlobalValue4FBDE0_SetToggleMode, 0x004FC210, (void* self, void* edx, int a3));
CSF_FUNC(int, __fastcall, GlobalValue_SetSelection, 0x004FC3B0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetSceneRegionCount, 0x004FC8B0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AddSceneRegion, 0x004FC970, (void* self, void* edx, int a2, int a3, int a4, int a5, int a6));
CSF_FUNC(int, __fastcall, C_0083f448_FinishLoading, 0x004FD0A0, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, C_0083f448_PrepareForSaving, 0x004FD600, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CommandoAnims_SerializeAuxiliaryState, 0x004FD720, (void* self, void* edx, int a2));

static int __fastcall C_0083f448_Serialize(
    C_0083f448* self, void* /*edx*/, unsigned int archiveAddress)
{
    auto* archive = reinterpret_cast<uint8_t*>(archiveAddress);
    auto& failed = CSF_AT(uint8_t, archive, 0);
    auto& saving = CSF_AT(uint8_t, archive, 0x14);
    auto& objectMode = CSF_AT(uint8_t, archive, 0x15);
    auto& secondaryMode = CSF_AT(uint8_t, archive, 0x16);
    auto& context = CSF_AT(uint32_t, archive, 0x1C);

    unsigned int firstCount = archiveAddress;

    alignas(8) uint8_t secondCountBytes[0x4];
    auto& secondCount = *reinterpret_cast<int32_t*>(secondCountBytes);
    secondCount = 0;

    alignas(8) uint8_t toggleBytes[0x8];
    auto& toggleMode = *reinterpret_cast<uint32_t*>(toggleBytes);
    auto& index = *reinterpret_cast<uint32_t*>(toggleBytes + 0x4);

    alignas(8) uint8_t selectionBytes[0x4];
    auto& selection = *reinterpret_cast<int32_t*>(selectionBytes);

    bool initialFailure = false;
    if (!failed)
    {
        void* stream = CSF_AT(void*, archive, 0x4);
        initialFailure =
            !((bool(__fastcall*)(void*, void*, int, unsigned int))
                  CSF_AT(void**, stream, 0)[0x80 / 4])(
                      stream, nullptr, 0, archiveAddress);
    }

    const uint32_t savedContext = context;
    const bool isSaving = saving != 0;
    failed = initialFailure;
    if (isSaving)
    {
        firstCount = self->field_000c;
        secondCount = static_cast<int32_t>(self->field_001c);
    }

    context = 0;
    if (!initialFailure)
    {
        void* stream = CSF_AT(void*, archive, 0x4);
        objectMode = 0;
        secondaryMode = 0;
        failed =
            ((int(__fastcall*)(void*, void*, unsigned int*, unsigned int))
                 CSF_AT(void**, stream, 0)[0x48 / 4])(
                     stream, nullptr, &firstCount, archiveAddress) == -1;
    }

    bool ready = failed == 0;
    context = 0;
    if (ready)
    {
        void* stream = CSF_AT(void*, archive, 0x4);
        objectMode = 0;
        secondaryMode = 0;
        failed =
            ((int(__fastcall*)(void*, void*, int32_t*, unsigned int))
                 CSF_AT(void**, stream, 0)[0x48 / 4])(
                     stream, nullptr, &secondCount, archiveAddress) == -1;
    }

    if (!saving && !failed)
    {
        for (index = self->field_000c; index < firstCount; ++index)
            GetSceneRegionCount(self, nullptr);

        for (index = self->field_001c;
             index < static_cast<uint32_t>(secondCount);
             ++index)
        {
            AddSceneRegion(self, nullptr, -1, 0, 0, 0, 0);
        }
    }

    ready = failed == 0;
    context = 0;
    if (ready)
    {
        objectMode = 1;
        secondaryMode = 0;
        Scene_SerializeNameList(
            reinterpret_cast<S_0041e5f0*>(&self->field_0004),
            static_cast<int>(archiveAddress));
    }

    ready = failed == 0;
    context = 0;
    if (ready)
    {
        objectMode = 1;
        secondaryMode = 0;
        Scene_SerializeNameList(
            reinterpret_cast<S_0041e5f0*>(&self->field_0014),
            static_cast<int>(archiveAddress));
    }

    ready = failed == 0;
    context = 0;
    if (ready)
    {
        objectMode = 1;
        secondaryMode = 0;
        Archive_SerializeObjectRef(
            &self->field_0078, reinterpret_cast<bool*>(archive));
    }

    ready = failed == 0;
    toggleMode = self->field_007c;
    context = 0;
    if (ready)
    {
        void* stream = CSF_AT(void*, archive, 0x4);
        objectMode = 0;
        secondaryMode = 0;
        failed =
            ((int(__fastcall*)(void*, void*, uint32_t*, unsigned int))
                 CSF_AT(void**, stream, 0)[0x58 / 4])(
                     stream, nullptr, &toggleMode, archiveAddress) == -1;
    }

    if (!saving && !failed)
    {
        const int mode = static_cast<int>(toggleMode);
        self->field_007c = 0;
        GlobalValue4FBDE0_SetToggleMode(self, nullptr, mode);
    }

    ready = failed == 0;
    context = 0;
    if (ready)
    {
        void* stream = CSF_AT(void*, archive, 0x4);
        objectMode = 0;
        secondaryMode = 0;
        failed =
            ((int(__fastcall*)(void*, void*, uint32_t*, unsigned int))
                 CSF_AT(void**, stream, 0)[0x54 / 4])(
                     stream, nullptr, &self->field_0080, archiveAddress) == -1;
    }

    ready = failed == 0;
    context = 0;
    if (ready)
    {
        void* stream = CSF_AT(void*, archive, 0x4);
        objectMode = 0;
        secondaryMode = 0;
        failed =
            ((int(__fastcall*)(void*, void*, uint32_t*, unsigned int))
                 CSF_AT(void**, stream, 0)[0x54 / 4])(
                     stream, nullptr, &self->field_0084, archiveAddress) == -1;
    }

    ready = failed == 0;
    context = 0;
    if (ready)
    {
        void* stream = CSF_AT(void*, archive, 0x4);
        objectMode = 0;
        secondaryMode = 0;
        failed =
            ((int(__fastcall*)(void*, void*, uint32_t*, unsigned int))
                 CSF_AT(void**, stream, 0)[0x54 / 4])(
                     stream, nullptr, &self->field_0088, archiveAddress) == -1;
    }

    ready = failed == 0;
    selection = static_cast<int32_t>(self->field_008c);
    context = 0;
    if (ready)
    {
        void* stream = CSF_AT(void*, archive, 0x4);
        objectMode = 0;
        secondaryMode = 0;
        failed =
            ((int(__fastcall*)(void*, void*, int32_t*, unsigned int))
                 CSF_AT(void**, stream, 0)[0x58 / 4])(
                     stream, nullptr, &selection, archiveAddress) == -1;
    }

    if (saving)
    {
        C_0083f448_PrepareForSaving(self, nullptr);
    }
    else
    {
        if (!failed)
        {
            const int selectedValue = selection;
            self->field_008c = static_cast<uint32_t>(-1);
            GlobalValue_SetSelection(self, nullptr, selectedValue);
        }
        if (saving)
            C_0083f448_PrepareForSaving(self, nullptr);
    }

    ready = failed == 0;
    context = 0;
    if (ready)
    {
        objectMode = 0;
        secondaryMode = 0;
        CommandoAnims_SerializeAuxiliaryState(
            reinterpret_cast<uint8_t*>(self) + 0xC0,
            nullptr, static_cast<int>(archiveAddress));
    }

    if (!saving && !failed)
        C_0083f448_FinishLoading(self, nullptr);

    ready = failed == 0;
    context = savedContext;
    if (ready)
    {
        void* stream = CSF_AT(void*, archive, 0x4);
        const int result =
            ((int(__fastcall*)(void*, void*, unsigned int))
                 CSF_AT(void**, stream, 0)[0x7C / 4])(
                     stream, nullptr, archiveAddress);
        if (static_cast<uint8_t>(result))
        {
            failed = 0;
            return result;
        }
    }

    failed = 1;
    return 1;
}

HOOK(0x004FD7A0, C_0083f448_Serialize, "auto");
