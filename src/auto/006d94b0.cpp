#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(float*, __fastcall, SceneMissionSetup_GetBoundsCenter, 0x006AB660, (void* self, void* edx, float* a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __fastcall, C_008656d0_IsMaskSubset, 0x006C5780, (C_008656d0* self, void* , int mask));
CSF_FUNC(char, __fastcall, GlobalHud_Commit_6C7C30, 0x006C7C30, (C_0085e7b8* self, void*));
CSF_FUNC(int, __fastcall, C_0085e7b8_AttachWidget, 0x006C8420, (C_0085e7b8* self, void* , C_0085e7b8* widget));
CSF_FUNC(char, __fastcall, GfxParticleRender_TestSweptBoundary, 0x006D7720, (void* self, void* edx, float a2, float* a3));
CSF_FUNC(unsigned int, __fastcall, S_006d8dc0_PrepareSkinObject, 0x006D8DC0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, ParticleSystem_UpdateWithInnerFlag, 0x00701030, (void* self, void*));

CSF_GLOBAL(float, g_SkinCullDistanceSquared, 0x008611B4);

static int __fastcall S_006d8dc0_FinishSkinObject(
    S_006d8dc0* self, void* /*edx*/)
{
    if (self->field_00a4 == 0)
        return 0;

    void* globals = reinterpret_cast<void*>(GetGlobalValue_6C5760());
    void* camera = CSF_AT(void*, globals, 0xF0);

    alignas(8) uint8_t positionBytes[0xC];
    globals = reinterpret_cast<void*>(GetGlobalValue_6C5760());
    CSF_AT(uint32_t, positionBytes, 0) = CSF_AT(uint32_t, globals, 0x124);
    CSF_AT(uint32_t, positionBytes, 4) = CSF_AT(uint32_t, globals, 0x128);
    CSF_AT(uint32_t, positionBytes, 8) = CSF_AT(uint32_t, globals, 0x12C);

    alignas(8) uint8_t sweepBytes[0x10] = {};
    float* sweep = reinterpret_cast<float*>(sweepBytes);
    const double extent =
        ((double(__fastcall*)(void*, void*))
            CSF_AT(void**, camera, 0)[0x44 / 4])(camera, nullptr);
    sweep[0] = static_cast<float>(extent + extent);
    sweep[1] = 0.0f;

    S_006d8dc0_PrepareSkinObject(
        self, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(camera)));
    self->field_0110 = 0;

    globals = reinterpret_cast<void*>(GetGlobalValue_6C5760());
    C_0085e7b8_AttachWidget(
        CSF_AT(C_0085e7b8*, globals, 4), nullptr,
        reinterpret_cast<C_0085e7b8*>(
            reinterpret_cast<uint8_t*>(self) + 0x11C));

    uint32_t particleIndex = 0;
    if (self->field_0100 != 0)
    {
        do
        {
            const uint32_t particleAddress =
                reinterpret_cast<uint32_t*>(self->field_00f8)[particleIndex];
            auto* particle = reinterpret_cast<uint8_t*>(particleAddress);

            alignas(8) uint8_t centerBytes[0xC];
            float* center = SceneMissionSetup_GetBoundsCenter(
                particle + 0x10, nullptr,
                reinterpret_cast<float*>(centerBytes));

            const float dx = CSF_AT(float, positionBytes, 0) - center[0];
            const float dy = CSF_AT(float, positionBytes, 4) - center[1];
            const float dz = CSF_AT(float, positionBytes, 8) - center[2];
            const float distanceSquared = dz * dz + dy * dy + dx * dx;

            if (!(distanceSquared > g_SkinCullDistanceSquared) &&
                ((bool(__fastcall*)(void*, void*, void*, int, int))
                    CSF_AT(void**, camera, 0)[0x70 / 4])(
                        camera, nullptr, particle + 0x10, 0, 6))
            {
                // The supplied binding labels this stack word as float;
                // the original actually passes the position buffer's address.
                float positionArgument;
                float* position = reinterpret_cast<float*>(positionBytes);
                std::memcpy(&positionArgument, &position, sizeof(positionArgument));

                if (GfxParticleRender_TestSweptBoundary(
                        particle, nullptr, positionArgument, sweep) ||
                    CSF_AT(uint8_t, particle, 0x2C) != 0)
                {
                    if (self->field_0110 >= self->field_010c)
                    {
                        S_0041e5f0_GrowArrayCapacity(
                            reinterpret_cast<S_0041e5f0*>(
                                reinterpret_cast<uint8_t*>(self) + 0x108),
                            nullptr, CSF_AT(int, self, 0x114));
                    }

                    const uint32_t activeIndex = self->field_0110;
                    uint32_t* slot =
                        reinterpret_cast<uint32_t*>(
                            CSF_AT(uint32_t, self, 0x108)) + activeIndex;
                    self->field_0110 = activeIndex + 1;
                    *slot = particleAddress;
                }
                else
                {
                    ParticleSystem_UpdateWithInnerFlag(particle, nullptr);
                }
            }

            const uint32_t particleCount = self->field_0100;
            ++particleIndex;
            if (particleIndex >= particleCount)
                break;
        } while (true);
    }

    globals = reinterpret_cast<void*>(GetGlobalValue_6C5760());
    GlobalHud_Commit_6C7C30(CSF_AT(C_0085e7b8*, globals, 4), nullptr);

    const uint32_t initialObjectCount = self->field_00b0;
    self->field_0118 = 1;
    uint32_t objectIndex = 0;
    if (initialObjectCount != 0)
    {
        do
        {
            if (objectIndex != 0 ||
                static_cast<uint8_t>(C_008656d0_IsMaskSubset(
                    reinterpret_cast<C_008656d0*>(GetGlobalValue_6C5760()),
                    nullptr, 0x8000)) == 0)
            {
                auto* object = reinterpret_cast<uint8_t*>(
                    reinterpret_cast<uint32_t*>(self->field_00a8)[objectIndex]);
                void* frames = object + 0x20;
                void* frame =
                    ((void*(__fastcall*)(void*, void*, uint32_t))
                        CSF_AT(void**, frames, 0)[0x18 / 4])(
                            frames, nullptr, self->field_00e8);
                if (frame)
                {
                    ((void(__fastcall*)(void*, void*))
                        CSF_AT(void**, frame, 0)[0x7C / 4])(frame, nullptr);
                }
            }

            const uint32_t objectCount = self->field_00b0;
            ++objectIndex;
            if (objectIndex >= objectCount)
                break;
        } while (true);
    }

    self->field_0138 = self->field_0110;
    const uint32_t nextFrame = self->field_00e8 + 1;
    self->field_0118 = 0;
    self->field_00e8 = nextFrame;
    return static_cast<int>(nextFrame);
}

HOOK(0x006D94B0, S_006d8dc0_FinishSkinObject, "auto");
