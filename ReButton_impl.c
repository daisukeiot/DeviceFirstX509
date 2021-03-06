/*******************************************************************************************
 * This code was automatically generated by Digital Twin Code Generator tool 0.6.9.
 *
 * You need to add your implementation here to:
 *    - get telemetry data from device/sensor
 *    - set read-only property data
 *    - handle read-write property callback
 *    - process device command
 *
 * Generated Date: 6/26/2020
 *******************************************************************************************/

#include "ReButton_impl.h"
#include "utilities/digitaltwin_serializer.h"
#include "utilities/deviceinfo_interface.h"
#include "utilities/battery_interface.h"
#include "utilities/pushbutton_interface.h"

#define Payload_Buffer_Size 256

void SendTelemetry_Succeeded_Callback(const char* interfaceName)
{
    // If needed, put your business logic here to handle the confirmation of the delivery for device telemetry on success.

    LogInfo("DigitalTwin successfully delivered telemetry message for component %s", interfaceName);
}

void SendTelemetry_Error_Callback(const char* interfaceName)
{
    // If needed, put your business logic here to handle the confirmation of the delivery for device telemetry on failure.

    LogInfo("DigitalTwin failed to deliver telemetry message for %s", interfaceName);
}

void ReportProperty_Succeeded_Callback(const char* interfaceName, const char* propertyName)
{
    // If needed, put your business logic here to handle the result callback reporting property on success.

    LogInfo("DigitalTwin successfully report writable property for %s::%s", interfaceName, propertyName);
}

void ReportProperty_Error_Callback(const char* interfaceName, const char* propertyName)
{
    // If needed, put your business logic here to handle the result callback of reporting property on failure.

    LogInfo("DigitalTwin failed to report writable property for %s::%s", interfaceName, propertyName);
}

char* DeviceInfo_Property_GetManufacturer()
{
    // TODO: provide implementation here
    return "abc";
}

char* DeviceInfo_Property_GetModel()
{
    // TODO: provide implementation here
    return "abc";
}

char* DeviceInfo_Property_GetSwVersion()
{
    // TODO: provide implementation here
    return "abc";
}

char* DeviceInfo_Property_GetOsName()
{
    // TODO: provide implementation here
    return "abc";
}

char* DeviceInfo_Property_GetProcessorArchitecture()
{
    // TODO: provide implementation here
    return "abc";
}

char* DeviceInfo_Property_GetProcessorManufacturer()
{
    // TODO: provide implementation here
    return "abc";
}

long DeviceInfo_Property_GetTotalStorage()
{
    // TODO: provide implementation here
    return 0L;
}

long DeviceInfo_Property_GetTotalMemory()
{
    // TODO: provide implementation here
    return 0L;
}

double Battery_Telemetry_ReadBatteryVoltage()
{
    // TODO: provide implementation here
    return 0.0;
}

char* PushButton_Telemetry_ReadSingleClick()
{
    // TODO: provide implementation here
    return "abc";
}

char* PushButton_Telemetry_ReadDoubleClick()
{
    // TODO: provide implementation here
    return "abc";
}

char* PushButton_Telemetry_ReadTripleClick()
{
    // TODO: provide implementation here
    return "abc";
}

char* PushButton_Telemetry_ReadLongPress()
{
    // TODO: provide implementation here
    return "abc";
}

char* PushButton_Telemetry_ReadSuperLongPress()
{
    // TODO: provide implementation here
    return "abc";
}

bool PushButton_Property_ActionCountCallback(int actionCount)
{
    // TODO: provide implementation here
    LogInfo("ActionCount property new value: %d", actionCount);
    return true;
}

