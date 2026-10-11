#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int*, __fastcall, C_0082db60_Initialize, 0x0040CEB0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_0082dcf0_Initialize, 0x0040DE10, (void* self, void* edx, char a2));
CSF_FUNC(uint32_t*, __fastcall, C_0082dd78_Initialize, 0x0040E4A0, (void* self, void* edx, char a2));
CSF_FUNC(uint32_t*, __fastcall, C_0082deb8_Initialize, 0x00411500, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0082dfd0_Initialize, 0x00415AE0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_0082e090_Initialize, 0x00416530, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0082e128_Initialize, 0x00417F00, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_0082e280_Initialize, 0x0041B9B0, (void* self, void* edx, char a2));
CSF_FUNC(uint32_t*, __fastcall, C_0082e2e8_Initialize, 0x0041BEA0, (void* self, void* edx, char a2));
CSF_FUNC(uint32_t*, __fastcall, C_0082e350_Initialize, 0x0041C270, (void* self, void* edx, char a2));
CSF_FUNC(int*, __fastcall, C_0082e3b8_Initialize, 0x0041CCE0, (void* self, void* edx, char a2));
CSF_FUNC(int*, __fastcall, C_0082e428_Initialize, 0x0041D710, (void* self, void* edx, char a2));
CSF_FUNC(uint32_t*, __fastcall, C_0082e520_Initialize, 0x0041E0F0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

static uint32_t* __cdecl Vehicle_CreateObjectByType(int type)
{
    switch (type)
    {
    case 0:
        if (CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0))
        {
            auto* object = static_cast<C_0082deb8*>(Mem_Alloc(132));
            if (!object)
                return nullptr;

            C_0082deb8_Initialize(object, nullptr);
            object->vtbl = CSF_VTBL(0x0082E4C0);
            return reinterpret_cast<uint32_t*>(object);
        }
        else
        {
            void* object = Mem_Alloc(132);
            if (!object)
                return nullptr;
            return C_0082deb8_Initialize(object, nullptr);
        }

    case 1:
    {
        void* object = Mem_Alloc(360);
        if (!object)
            return nullptr;
        return reinterpret_cast<uint32_t*>(C_0082dfd0_Initialize(object, nullptr));
    }

    case 2:
    {
        void* object = Mem_Alloc(188);
        if (!object)
            return nullptr;
        return C_0082e090_Initialize(object, nullptr);
    }

    case 3:
    {
        void* object = Mem_Alloc(532);
        if (!object)
            return nullptr;
        return reinterpret_cast<uint32_t*>(C_0082e128_Initialize(object, nullptr));
    }

    case 6:
    {
        void* object = Mem_Alloc(176);
        if (!object)
            return nullptr;
        return reinterpret_cast<uint32_t*>(C_0082e3b8_Initialize(object, nullptr, 0));
    }

    case 7:
    {
        void* object = Mem_Alloc(164);
        if (!object)
            return nullptr;
        return reinterpret_cast<uint32_t*>(C_0082e428_Initialize(object, nullptr, 0));
    }

    case 8:
    {
        void* object = Mem_Alloc(144);
        if (!object)
            return nullptr;
        return C_0082e280_Initialize(object, nullptr, 0);
    }

    case 9:
    {
        void* object = Mem_Alloc(144);
        if (!object)
            return nullptr;
        return C_0082e2e8_Initialize(object, nullptr, 0);
    }

    case 10:
    {
        void* object = Mem_Alloc(144);
        if (!object)
            return nullptr;
        return C_0082e350_Initialize(object, nullptr, 0);
    }

    case 11:
    {
        auto* object = static_cast<C_0082e520*>(Mem_Alloc(20));
        if (!object)
            return nullptr;

        C_0082e520_Initialize(object, nullptr);
        object->vtbl = CSF_VTBL(0x0082DC10);
        CSF_AT(uint32_t, object, 0x10) = 0;
        return reinterpret_cast<uint32_t*>(object);
    }

    case 12:
    {
        void* object = Mem_Alloc(168);
        if (!object)
            return nullptr;
        return reinterpret_cast<uint32_t*>(C_0082db60_Initialize(object, nullptr));
    }

    case 13:
    {
        auto* object = static_cast<C_0082e520*>(Mem_Alloc(16));
        if (!object)
            return nullptr;

        C_0082e520_Initialize(object, nullptr);
        object->vtbl = CSF_VTBL(0x0082DF28);
        return reinterpret_cast<uint32_t*>(object);
    }

    case 14:
    {
        auto* object = static_cast<C_0082e520*>(Mem_Alloc(20));
        if (!object)
            return nullptr;

        C_0082e520_Initialize(object, nullptr);
        object->vtbl = CSF_VTBL(0x0082E188);
        CSF_AT(uint32_t, object, 0x10) = 0;
        return reinterpret_cast<uint32_t*>(object);
    }

    case 15:
    {
        auto* object = static_cast<C_0082e520*>(Mem_Alloc(100));
        if (!object)
            return nullptr;

        C_0082e520_Initialize(object, nullptr);
        object->vtbl = CSF_VTBL(0x0082DDF8);
        CSF_AT(uint32_t, object, 0x14) = 0;

        CSF_AT(float, object, 0x2C) = g_ZeroVectorX;
        CSF_AT(float, object, 0x30) = g_ZeroVectorY;
        CSF_AT(float, object, 0x34) = g_ZeroVectorZ;
        CSF_AT(float, object, 0x38) = g_ZeroVectorX;
        CSF_AT(float, object, 0x3C) = g_ZeroVectorY;
        CSF_AT(float, object, 0x40) = g_ZeroVectorZ;

        CSF_AT(uint32_t, object, 0x44) = 0;
        CSF_AT(uint32_t, object, 0x48) = 0;
        CSF_AT(uint32_t, object, 0x4C) = 0;
        CSF_AT(uint32_t, object, 0x58) = 0;
        CSF_AT(uint32_t, object, 0x5C) = 0;
        CSF_AT(uint32_t, object, 0x60) = 0;
        CSF_AT(uint32_t, object, 0x14) = 0;
        CSF_AT(uint32_t, object, 0x1C) = 0;
        CSF_AT(uint32_t, object, 0x20) = 0;
        CSF_AT(uint32_t, object, 0x24) = 0;
        CSF_AT(uint8_t, object, 0x18) = 0;
        CSF_AT(uint8_t, object, 0x19) = 0;
        CSF_AT(uint8_t, object, 0x1A) = 0;
        CSF_AT(uint32_t, object, 0x10) = 0;
        CSF_AT(uint32_t, object, 0x28) = 0;
        return reinterpret_cast<uint32_t*>(object);
    }

    case 16:
    {
        auto* object = static_cast<C_0082e520*>(Mem_Alloc(20));
        if (!object)
            return nullptr;

        C_0082e520_Initialize(object, nullptr);
        object->vtbl = CSF_VTBL(0x0082DB10);
        CSF_AT(uint32_t, object, 0x10) = 0;
        return reinterpret_cast<uint32_t*>(object);
    }

    case 17:
    {
        auto* object = static_cast<C_0082e520*>(Mem_Alloc(20));
        if (!object)
            return nullptr;

        C_0082e520_Initialize(object, nullptr);
        object->vtbl = CSF_VTBL(0x0082DF70);
        CSF_AT(uint32_t, object, 0x10) = 0;
        return reinterpret_cast<uint32_t*>(object);
    }

    case 19:
    {
        void* object = Mem_Alloc(120);
        if (!object)
            return nullptr;
        return C_0082dd78_Initialize(object, nullptr, 0);
    }

    case 20:
    {
        void* object = Mem_Alloc(108);
        if (!object)
            return nullptr;
        return C_0082dcf0_Initialize(object, nullptr, 0);
    }

    default:
        return nullptr;
    }
}

HOOK(0x0041DDC0, Vehicle_CreateObjectByType, "auto");
