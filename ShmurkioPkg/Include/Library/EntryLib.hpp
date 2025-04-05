#pragma once
#pragma warning(default: 4100)

extern "C"
{
    #include <Base.h>
    #include <Uefi.h>
    #include <Library/UefiLib.h>
    #include <Library/UefiBootServicesTableLib.h>
    #include <Library/MemoryAllocationLib.h>

    #include <Protocol/LoadedImage.h>

    #include <sal.h>
}

#include <cstdint>

int
EFIAPI
Main(
    [[maybe_unused]] _In_ int ArgC,
    [[maybe_unused]] _In_ char* ArgV[]
);