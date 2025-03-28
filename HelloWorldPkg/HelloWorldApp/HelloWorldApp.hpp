#pragma once

extern "C"
{
    #include <Uefi.h>
    #include <Library/UefiLib.h>
    #include <Library/UefiBootServicesTableLib.h>
    #include <Library/UefiApplicationEntryPoint.h>
    #include <Library/UefiRuntimeServicesTableLib.h>
    #include <Library/SerialPortLib.h>
    #include <Library/DebugLib.h>
    #include <Library/IoLib.h>
    #include <Library/PrintLib.h>
    #include <Library/MemoryAllocationLib.h>
    #include <Library/BaseMemoryLib.h>

    #include <sal.h>
}

#include <Library/SerialLib.hpp>
#include <Library/EntryLib.hpp>

#include <cstdint>