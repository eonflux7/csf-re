#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int archiveAddress, S_00691860* string));
CSF_FUNC(int, __fastcall, AiSearchOrders_SerializeBaseState, 0x004A9D00, (void* self, void* edx, int a1));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));
CSF_FUNC(int, __fastcall, S_004af750_EvaluateGuardWaypoint, 0x004AF750, (S_004af750* self, void* , float* position));
CSF_FUNC(int, __fastcall, S_00523cf0_ProcessCoverAttackState, 0x004C9BC0, (S_00523cf0* self, void* , S_00691860* context));
CSF_FUNC(int, __fastcall, AiAutonomous_SerializeByteAndPair, 0x004D18F0, (void* self, void* , void* context));
CSF_FUNC(int, __fastcall, AiStateFactory_HasObjectReference, 0x004DB5F0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, WaypointGraph_VisitNodePair, 0x0075D2C0, (S_004a9e10* self, void* , uint8_t* ctx));

CSF_GLOBAL(void*, g_ObjectReferenceManager, 0x008B0550);

// The packet also identifies this address as a vtable reference.
static const auto g_GuardWaypointVtable = CSF_VTBL(0x004AF750);

static bool AiSearchOrders_BeginArchiveValue(uint8_t* context, uint8_t referenceFlag)
{
    const bool ready = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(uint8_t, context, 0x15) = referenceFlag;
        CSF_AT(uint8_t, context, 0x16) = 0;
    }
    return ready;
}

// Slot is a compile-time vtable index, never a runtime dispatch parameter.
template<std::size_t Slot>
static void AiSearchOrders_SerializeValue(uint8_t* context, void* value)
{
    const bool ready = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (ready)
    {
        void* stream = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, uint8_t*)>(
                CSF_AT(void**, stream, 0x00)[Slot])(
                    stream, nullptr, value, context) == -1;
    }
}

static void AiSearchOrders_SerializePresentReference(
    uint8_t* context, void* reference, uint8_t* present, uint8_t* referenceBytes)
{
    AiSearchOrders_SerializeValue<68 / 4>(context, present);
    if (*present != 0)
    {
        CSF_AT(void*, referenceBytes, 0x00) = reference;
        if (AiSearchOrders_BeginArchiveValue(context, 1))
            Archive_SerializeObjectRef(referenceBytes, reinterpret_cast<bool*>(context));
    }
}

template<std::size_t ReferenceOffset, std::size_t CountOffset>
static void AiSearchOrders_SerializeCountedReference(
    uint8_t* self, uint8_t* context, uint8_t* present, uint8_t* referenceBytes)
{
    *present = 0;
    if (CSF_AT(uint8_t, context, 0x14) != 0)
        *present = CSF_AT(uint32_t, self, CountOffset) != 0;
    AiSearchOrders_SerializePresentReference(
        context, self + ReferenceOffset, present, referenceBytes);
}

static int __fastcall AiSearchOrders_SerializeAutonomousState(
    void* selfAddress, void* /*edx*/, uint8_t* context)
{
    auto* self = static_cast<uint8_t*>(selfAddress);

    bool failed = false;
    if (CSF_AT(uint8_t, context, 0x00) == 0)
    {
        void* stream = CSF_AT(void*, context, 0x04);
        // The original tests the whole AL byte, not a canonical C++ bool.
        failed = !reinterpret_cast<uint8_t(__fastcall*)(void*, void*, int, uint8_t*)>(
            CSF_AT(void**, stream, 0x00)[128 / 4])(
                stream, nullptr, 0, context);
    }

    const uint32_t savedArchiveValue = CSF_AT(uint32_t, context, 0x1C);
    CSF_AT(uint8_t, context, 0x00) = failed;
    AiSearchOrders_SerializeBaseState(
        self, nullptr, reinterpret_cast<int>(context));

    alignas(8) uint8_t referenceBytes[0x7];
    alignas(8) uint8_t presenceBytes[0x1];
    uint8_t& present = presenceBytes[0];

    static const std::size_t referenceOffsets[] = {
        44, 48, 52, 56, 76, 128, 264, 408, 412, 416, 424, 428, 452,
        536, 568, 980, 1024, 1044, 1336, 1372, 1388, 1392,
        1436, 1472, 1084
    };

    for (std::size_t offset : referenceOffsets)
    {
        present = 0;
        if (CSF_AT(uint8_t, context, 0x14) != 0)
        {
            present = static_cast<uint8_t>(AiStateFactory_HasObjectReference(
                g_ObjectReferenceManager, nullptr,
                reinterpret_cast<int>(self + offset)));
        }
        AiSearchOrders_SerializePresentReference(
            context, self + offset, &present, referenceBytes);
    }

    AiSearchOrders_SerializeCountedReference<596, 600>(
        self, context, &present, referenceBytes);
    AiSearchOrders_SerializeCountedReference<672, 676>(
        self, context, &present, referenceBytes);
    AiSearchOrders_SerializeCountedReference<720, 724>(
        self, context, &present, referenceBytes);
    AiSearchOrders_SerializeCountedReference<752, 756>(
        self, context, &present, referenceBytes);
    AiSearchOrders_SerializeCountedReference<780, 784>(
        self, context, &present, referenceBytes);
    AiSearchOrders_SerializeCountedReference<808, 812>(
        self, context, &present, referenceBytes);
    AiSearchOrders_SerializeCountedReference<864, 868>(
        self, context, &present, referenceBytes);
    AiSearchOrders_SerializeCountedReference<900, 904>(
        self, context, &present, referenceBytes);
    AiSearchOrders_SerializeCountedReference<944, 948>(
        self, context, &present, referenceBytes);

    uint8_t hasCoverAttackState = 0;
    if (CSF_AT(uint8_t, context, 0x14) != 0)
    {
        if (CSF_AT(int32_t, self, 1540) != -1 ||
            CSF_AT(int32_t, self, 1544) != -1)
            hasCoverAttackState = 1;
    }
    AiSearchOrders_SerializeValue<68 / 4>(context, &hasCoverAttackState);

    if (CSF_AT(int32_t, context, 0x18) > 75 || hasCoverAttackState != 0)
    {
        if (AiSearchOrders_BeginArchiveValue(context, 0))
        {
            S_00523cf0_ProcessCoverAttackState(
                reinterpret_cast<S_00523cf0*>(self + 1496), nullptr,
                reinterpret_cast<S_00691860*>(context));
        }
    }

    AiSearchOrders_SerializeValue<88 / 4>(context, self + 1668);
    AiSearchOrders_SerializeValue<72 / 4>(context, self + 1672);

    if (AiSearchOrders_BeginArchiveValue(context, 0))
        AiAutonomous_SerializeByteAndPair(self + 1676, nullptr, context);

    AiSearchOrders_SerializeValue<84 / 4>(context, self + 1688);
    AiSearchOrders_SerializeValue<72 / 4>(context, self + 1700);
    AiSearchOrders_SerializeValue<72 / 4>(context, self + 1704);
    AiSearchOrders_SerializeValue<68 / 4>(context, self + 1708);
    AiSearchOrders_SerializeValue<56 / 4>(context, self + 1712);

    if (AiSearchOrders_BeginArchiveValue(context, 0))
    {
        WaypointGraph_VisitNodePair(
            reinterpret_cast<S_004a9e10*>(self + 1724), nullptr, context);
    }

    AiSearchOrders_SerializeValue<68 / 4>(context, self + 1736);

    if (AiSearchOrders_BeginArchiveValue(context, 0))
    {
        Archive_SerializeString(
            reinterpret_cast<int>(self + 1740),
            reinterpret_cast<S_00691860*>(context));
    }

    AiSearchOrders_SerializeValue<88 / 4>(context, self + 1752);
    AiSearchOrders_SerializeValue<68 / 4>(context, self + 1692);

    if (CSF_AT(uint8_t, context, 0x14) == 0 &&
        CSF_AT(uint8_t, context, 0x00) == 0 &&
        CSF_AT(int32_t, self, 1724) != -1 &&
        CSF_AT(int32_t, self, 1728) != -1)
    {
        CSF_AT(int32_t, self, 1732) = S_004af750_EvaluateGuardWaypoint(
            reinterpret_cast<S_004af750*>(self - 4), nullptr,
            reinterpret_cast<float*>(self + 1712));
    }

    const bool ready = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = savedArchiveValue;

    if (ready)
    {
        void* stream = CSF_AT(void*, context, 0x04);
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint8_t*)>(
                CSF_AT(void**, stream, 0x00)[124 / 4])(
                    stream, nullptr, context);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, context, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, context, 0x00) = 1;
    return 1;
}

HOOK(0x004CBC30, AiSearchOrders_SerializeAutonomousState, "auto");
