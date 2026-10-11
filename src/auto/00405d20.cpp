#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, ApplyAngleOffsets, 0x004055E0,
         (void* self, void* edx, float a2, float a3));
CSF_FUNC(float, __cdecl, ComputePointAngle, 0x00405720,
         (int* a1, int arg4, int a3));
CSF_FUNC(int, __cdecl, NotifyAngleProperty, 0x00405AB0,
         (int a1, uint32_t* arg4, char arg8));

static void __fastcall C_0082d58c_SetAnglesFromPoint(
    C_0082d58c* self, void* /*edx*/, float* point, int applyImmediately)
{
    int* horizontalReference = nullptr;
    int* verticalReference = nullptr;

    void* owner = reinterpret_cast<void*>(self->field_0008);
    if (owner)
    {
        void* collection =
            reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, owner, 0)[0x28 / 4])(owner, nullptr);
        if (collection)
        {
            int index =
                reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, collection, 0)[0x88 / 4])(
                        collection, nullptr, 510);
            if (index != -1)
            {
                horizontalReference =
                    reinterpret_cast<int* (__fastcall*)(void*, void*, int)>(
                        CSF_AT(void**, collection, 0)[0x80 / 4])(
                            collection, nullptr, index);
            }
        }
    }

    owner = reinterpret_cast<void*>(self->field_0008);
    if (owner)
    {
        void* collection =
            reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, owner, 0)[0x28 / 4])(owner, nullptr);
        if (collection)
        {
            int index =
                reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, collection, 0)[0x88 / 4])(
                        collection, nullptr, 511);
            if (index != -1)
            {
                verticalReference =
                    reinterpret_cast<int* (__fastcall*)(void*, void*, int)>(
                        CSF_AT(void**, collection, 0)[0x80 / 4])(
                            collection, nullptr, index);
            }
        }
    }

    if (horizontalReference && verticalReference)
    {
        const int pointAddress =
            static_cast<int>(reinterpret_cast<std::uintptr_t>(point));
        const float horizontalAngle =
            ComputePointAngle(horizontalReference, pointAddress, 1);
        ApplyAngleOffsets(self, nullptr, horizontalAngle, 0.0f);

        const float verticalAngle =
            -ComputePointAngle(verticalReference, pointAddress, 0);

        if (static_cast<uint8_t>(applyImmediately))
        {
            ApplyAngleOffsets(self, nullptr, 0.0f, verticalAngle);
        }
        else
        {
            ApplyAngleOffsets(self, nullptr, -horizontalAngle, 0.0f);

            uint32_t property = 45;
            NotifyAngleProperty(
                static_cast<int>(reinterpret_cast<std::uintptr_t>(
                    reinterpret_cast<uint8_t*>(self->field_0008) + 28)),
                &property, 1);

            void* properties =
                reinterpret_cast<uint8_t*>(self->field_0008) + 28;
            std::memcpy(&self->field_0014, &horizontalAngle,
                        sizeof(horizontalAngle));
            self->field_0011 = 1;
            property = 46;
            NotifyAngleProperty(
                static_cast<int>(reinterpret_cast<std::uintptr_t>(properties)),
                &property, 1);

            self->field_0018 = verticalAngle;
            self->field_0011 = 1;
        }
    }
}

HOOK(0x00405D20, C_0082d58c_SetAnglesFromPoint, "auto");
