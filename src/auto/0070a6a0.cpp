#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwFrameUpdateObjects, 0x006236A0, (int a1));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(int, __fastcall, GfxMatrix_AreTransformsDifferent, 0x006ADC80, (float** self, void* edx, float** other));
CSF_FUNC(char, __fastcall, sub_6FF9F0, 0x006FF9F0, (void* self, void* edx, float a2));
CSF_FUNC(int, __cdecl, RpTeamPlayerSetActive, 0x007FEE10, (int a1, int a2, int a3));
CSF_FUNC(float*, __cdecl, sub_803770, 0x00803770, (int a1, uint16_t a2));
CSF_FUNC(int, __cdecl, sub_81BC10, 0x0081BC10, (int a1, int a2, int a3));

static char __fastcall C_00863588_UpdateTransform(C_00863588* self, void* /*edx*/, float a2)
{
    alignas(8) uint8_t matBytes[8];
    void*& matPtr = *reinterpret_cast<void**>(matBytes);
    uint8_t& ownsMatrix = matBytes[4];

    char result = self->vSlot28();
    if (!result)
        return result;

    RwFrameUpdateObjects(CSF_AT(int, reinterpret_cast<void*>(self->field_0030), 0xC));

    if (self->field_0038 || CSF_AT(uint8_t, self, 0xD)) {
        int ownerForCall = static_cast<int>(self->field_0004);
        sub_803770(ownerForCall, 0);
        // The original leaves sub_803770's pushed args on the stack, so
        // sub_81BC10 sees (field_0004, <previous field_0004>, 0).
        sub_81BC10(static_cast<int>(self->field_0004), ownerForCall, 0);
        self->field_0038 = 0;
    }

    if (self->field_0034) {
        uint8_t* owner = reinterpret_cast<uint8_t*>(self->field_0004);
        RpTeamPlayerSetActive(CSF_AT(int, owner, 0x24), static_cast<int>(self->field_0004), 0);

        void* obj30 = reinterpret_cast<void*>(self->field_0030);
        ((void(__fastcall*)(void*, void*, void*))CSF_AT(void**, obj30, 0)[0x8C / 4])(obj30, nullptr, matBytes);

        if (static_cast<uint8_t>(GfxMatrix_AreTransformsDifferent(
                reinterpret_cast<float**>(self->field_0034), nullptr,
                reinterpret_cast<float**>(matBytes)))) {
            uint8_t* owner2 = reinterpret_cast<uint8_t*>(self->field_0004);
            RpTeamPlayerSetActive(CSF_AT(int, owner2, 0x24), static_cast<int>(self->field_0004), 1);
        }

        std::memcpy(*reinterpret_cast<void**>(self->field_0034), matPtr, 0x40);

        if (ownsMatrix)
            RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(matPtr));
    }

    return sub_6FF9F0(self, nullptr, a2);
}

HOOK(0x0070A6A0, C_00863588_UpdateTransform, "auto");
