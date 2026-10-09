#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResetAttachedObject, 0x00425640, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, SceneLoader_ConfigureFocusEntry, 0x0048E460, (void* self, void* edx, int* a2));
CSF_FUNC(int, __fastcall, DispatchSceneObjectWithPayload, 0x0048E4D0, (void* self, void* edx, int payload));
CSF_FUNC(double, __fastcall, ComputeScalarForResolvedObject, 0x0048F220, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CharActionFsm_SetCharacterItem, 0x00494E90, (void* self, void* edx, int a4));
CSF_FUNC(int, __fastcall, GetFocusEntryDescriptor, 0x0049ED80, (void* self, void* edx, int a2, uint32_t* a3));
CSF_FUNC(unsigned int*, __fastcall, CopyFocusEntryPosition, 0x0049EE00, (void* self, void* edx, int arg0, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, GetMatrixEulerAngles, 0x006AE3A0, (void* self, void* edx, float* a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_RadiansToDegrees, 0x00833D9C);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

static std::uint32_t __fastcall CPlayerCharacter_InitializeFromFocusEntry(
    CPlayerCharacter* self, void* /*edx*/)
{
    auto* source = reinterpret_cast<C_008342e8*>(
        CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            reinterpret_cast<int>(reinterpret_cast<std::uint8_t*>(self) + 0x1B4)));

    std::uint32_t descriptor[6]{};
    GetFocusEntryDescriptor(source, nullptr, self->field_01c0, descriptor);

    void* entries = reinterpret_cast<void*>(source->vGetField016c());
    const int focusIndex =
        ((int(__fastcall*)(void*, void*, int))
            CSF_AT(void**, entries, 0)[0x98 / 4])(
                entries, nullptr, descriptor[1]);

    entries = reinterpret_cast<void*>(source->vGetField016c());
    const int baseIndex =
        ((int(__fastcall*)(void*, void*, int))
            CSF_AT(void**, entries, 0)[0x98 / 4])(
                entries, nullptr, 3000);

    if (focusIndex == -1 || baseIndex == -1)
        return static_cast<std::uint32_t>(baseIndex);

    entries = reinterpret_cast<void*>(source->vGetField016c());
    void* focusEntry =
        ((void*(__fastcall*)(void*, void*, int))
            CSF_AT(void**, entries, 0)[0x8C / 4])(
                entries, nullptr, focusIndex);

    entries = reinterpret_cast<void*>(source->vGetField016c());
    void* baseEntry =
        ((void*(__fastcall*)(void*, void*, int))
            CSF_AT(void**, entries, 0)[0x8C / 4])(
                entries, nullptr, baseIndex);

    if (source->field_0050 == 7)
    {
        self->field_01ac = 0;
        self->field_01b0 = 0;
        if (source->field_0030)
            ResetAttachedObject(
                reinterpret_cast<void*>(source->field_0030), nullptr);
    }
    else
    {
        CharActionFsm_SetCharacterItem(self, nullptr, 0);
    }

    CopyFocusEntryPosition(
        source, nullptr, self->field_01c0,
        reinterpret_cast<int>(&self->field_001c));

    void* focusObject =
        ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, focusEntry, 0)[0x18 / 4])(
                focusEntry, nullptr);
    int* focusData =
        ((int*(__fastcall*)(void*, void*))
            CSF_AT(void**, focusObject, 0)[0x10 / 4])(
                focusObject, nullptr);
    SceneLoader_ConfigureFocusEntry(self, nullptr, focusData);

    void* baseObject =
        ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, baseEntry, 0)[0x18 / 4])(
                baseEntry, nullptr);
    const float baseScalar = static_cast<float>(
        ((double(__fastcall*)(void*, void*))
            CSF_AT(void**, baseObject, 0)[0x28 / 4])(
                baseObject, nullptr));
    const float scalar = static_cast<float>(
        ComputeScalarForResolvedObject(source, nullptr) + baseScalar);
    int payload;
    std::memcpy(&payload, &scalar, sizeof(payload));
    DispatchSceneObjectWithPayload(self, nullptr, payload);

    entries = reinterpret_cast<void*>(source->vGetField016c());
    const int orientationIndex =
        ((int(__fastcall*)(void*, void*, int))
            CSF_AT(void**, entries, 0)[0x88 / 4])(
                entries, nullptr, 203);

    if (orientationIndex != -1)
    {
        entries = reinterpret_cast<void*>(source->vGetField016c());
        void* orientationEntry =
            ((void*(__fastcall*)(void*, void*, int))
                CSF_AT(void**, entries, 0)[0x80 / 4])(
                    entries, nullptr, orientationIndex);

        std::uint32_t orientationMatrix[3]{};
        float angles[3]{};
        void* matrixReference =
            ((void*(__fastcall*)(void*, void*, std::uint32_t*))
                CSF_AT(void**, orientationEntry, 0)[0x40 / 4])(
                    orientationEntry, nullptr, orientationMatrix);
        float* euler = GetMatrixEulerAngles(matrixReference, nullptr, angles);

        const float firstAngle = euler[0] * g_RadiansToDegrees;
        const float secondAngle = euler[1] * g_RadiansToDegrees;
        if (static_cast<std::uint8_t>(orientationMatrix[1]))
            RwMatrixDestroy(
                reinterpret_cast<std::uint32_t*>(orientationMatrix[0]));

        std::memcpy(&self->field_01ac, &secondAngle, sizeof(secondAngle));
        std::memcpy(&self->field_01b0, &firstAngle, sizeof(firstAngle));
    }

    const int globalValue = GetGlobalValue_4FBDE0();
    void* firstElement = reinterpret_cast<void*>(
        GetFirstElement(reinterpret_cast<void*>(globalValue), nullptr));
    void* provider = GetCachedObjectProvider(firstElement, nullptr);
    void* referencedObject = GetReferencedObject(provider, nullptr);
    void* destination =
        ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, referencedObject, 0)[0x10 / 4])(
                referencedObject, nullptr);

    self->vSlot6C(0);
    self->field_03a5 = 1;

    baseObject =
        ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, baseEntry, 0)[0x18 / 4])(
                baseEntry, nullptr);
    void** destinationVtable = CSF_AT(void**, destination, 0);
    void* baseData =
        ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, baseObject, 0)[0x10 / 4])(
                baseObject, nullptr);
    ((void(__fastcall*)(void*, void*, void*))
        destinationVtable[0x14 / 4])(destination, nullptr, baseData);

    baseObject =
        ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, baseEntry, 0)[0x18 / 4])(
                baseEntry, nullptr);

    std::uint32_t transformMatrix[2]{};
    void* transformReference =
        ((void*(__fastcall*)(void*, void*, std::uint32_t*))
            CSF_AT(void**, baseObject, 0)[0x8C / 4])(
                baseObject, nullptr, transformMatrix);
    void* translation = CSF_AT(std::uint8_t*, transformReference, 0) + 32;
    ((void(__fastcall*)(void*, void*, void*))
        CSF_AT(void**, destination, 0)[0x58 / 4])(
            destination, nullptr, translation);

    if (static_cast<std::uint8_t>(transformMatrix[1]))
        RwMatrixDestroy(
            reinterpret_cast<std::uint32_t*>(transformMatrix[0]));

    transformReference =
        ((void*(__fastcall*)(void*, void*, std::uint32_t*))
            CSF_AT(void**, destination, 0)[0x8C / 4])(
                destination, nullptr, transformMatrix);
    auto* position = reinterpret_cast<std::uint32_t*>(
        CSF_AT(std::uint8_t*, transformReference, 0) + 32);
    self->field_03a8 = position[0];
    self->field_03ac = position[1];
    self->field_03b0 = position[2];

    if (static_cast<std::uint8_t>(transformMatrix[1]))
        RwMatrixDestroy(
            reinterpret_cast<std::uint32_t*>(transformMatrix[0]));

    CSF_AT(std::uint32_t, self, 0x348) = 0;
    CSF_AT(std::uint32_t, self, 0x34C) = 0;
    self->field_02e0 |= 0x2000u;
    self->field_0244 = -1;
    self->field_0248 = -1;
    self->field_0250 = -1;
    self->field_0254 = -1;
    self->field_0258 = 0;
    std::memcpy(&self->field_025c, &g_ZeroVectorX, sizeof(float));
    self->field_0260 = g_ZeroVectorY;
    std::memcpy(&self->field_0264, &g_ZeroVectorZ, sizeof(float));
    self->field_0268 = 0;
    self->field_026c = 0;
    self->field_0274 = -1;
    self->field_0278 = -1;
    self->field_027c = -1;
    std::memcpy(&self->field_0280, &g_ZeroVectorX, sizeof(float));
    std::memcpy(&self->field_0284, &g_ZeroVectorY, sizeof(float));
    std::memcpy(&self->field_0288, &g_ZeroVectorZ, sizeof(float));
    self->field_0290 = -1;
    self->field_0294 = -1;
    std::memcpy(&self->field_01c8, &g_ZeroVectorX, sizeof(float));
    std::memcpy(&self->field_01cc, &g_ZeroVectorY, sizeof(float));
    std::memcpy(&self->field_01d0, &g_ZeroVectorZ, sizeof(float));
    self->field_03c4 = source->field_0020;
    const std::uint32_t result = source->field_0024;
    self->field_03c8 = result;
    return result;
}

HOOK(0x00499590, CPlayerCharacter_InitializeFromFocusEntry, "auto");
