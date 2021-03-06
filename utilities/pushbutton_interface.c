/************************************************************************************************
 * This code was automatically generated by Digital Twin Code Generator tool 0.6.9.
 * Changes to this file may cause incorrect behavior and will be lost if the code is regenerated.
 *
 * Generated Date: 6/26/2020
 ***********************************************************************************************/

#include "pushbutton_interface.h"

#define MAX_MESSAGE_SIZE 256

//
// Callback function declarations and DigitalTwin writable (from service side) properties for this interface
//

static void PushButtonInterface_Property_ActionCountCallback(const DIGITALTWIN_CLIENT_PROPERTY_UPDATE* dtClientPropertyUpdate, void* userInterfaceContext);

//
// Application state associated with this interface.
// It contains the DIGITALTWIN_INTERFACE_CLIENT_HANDLE used for responses in callbacks along with properties set
// and representations of the property update and command callbacks invoked on given interface
//
typedef struct PUSHBUTTON_INTERFACE_STATE_TAG
{
    DIGITALTWIN_INTERFACE_CLIENT_HANDLE interfaceClientHandle;

    int actionCount;

} PUSHBUTTON_INTERFACE_STATE;

static PUSHBUTTON_INTERFACE_STATE appState;

// PushButtonInterface_PropertyCallback is invoked when a property is updated (or failed) going to server.
// ALL property callbacks will be routed to this function and just have the userContextCallback set to the propertyName.
// Product code will potentially have context stored in this userContextCallback.
static void PushButtonInterface_PropertyCallback(DIGITALTWIN_CLIENT_RESULT digitalTwinReportedStatus, void* userContextCallback)
{
    if (digitalTwinReportedStatus == DIGITALTWIN_CLIENT_OK)
    {
        ReportProperty_Succeeded_Callback("PushButton", (const char*)userContextCallback);
        LogInfo("PUSHBUTTON_INTERFACE: Updating property=<%s> succeeded", (const char*)userContextCallback);
    }
    else
    {
        ReportProperty_Error_Callback("PushButton", (const char*)userContextCallback);
        LogError("PUSHBUTTON_INTERFACE: Updating property=<%s> failed, error=<%s>", (const char*)userContextCallback, MU_ENUM_TO_STRING(DIGITALTWIN_CLIENT_RESULT, digitalTwinReportedStatus));
    }
}

// Processes a property update, which the server initiated, for 'actionCount' property.
static void PushButtonInterface_Property_ActionCountCallback(const DIGITALTWIN_CLIENT_PROPERTY_UPDATE* dtClientPropertyUpdate, void* userInterfaceContext)
{
    PUSHBUTTON_INTERFACE_STATE* interfaceState = (PUSHBUTTON_INTERFACE_STATE*)userInterfaceContext;

    LogInfo("PUSHBUTTON_INTERFACE: actionCount property invoked...");
    LogInfo("PUSHBUTTON_INTERFACE: actionCount data=<%.*s>", (int)dtClientPropertyUpdate->propertyDesiredLen, dtClientPropertyUpdate->propertyDesired);

    DIGITALTWIN_CLIENT_PROPERTY_RESPONSE propertyResponse;

    // Version of this structure for C SDK.
    propertyResponse.version = DIGITALTWIN_CLIENT_PROPERTY_RESPONSE_VERSION_1;
    propertyResponse.responseVersion = dtClientPropertyUpdate->desiredVersion;

    // Parse string value and set updated property
    JSON_Value * jsonValue = json_parse_string((const char*)dtClientPropertyUpdate->propertyDesired);

    double newValue = json_value_get_number(jsonValue);

    bool isTypeValid = (json_value_get_type(jsonValue) == JSONNumber && newValue == (int)newValue);

    if (isTypeValid)
    {
        interfaceState->actionCount = (int)newValue;

        LogInfo("PUSHBUTTON_INTERFACE: Updating actionCount property with new value <%s> ...", dtClientPropertyUpdate->propertyDesired);
        bool updateResult = PushButton_Property_ActionCountCallback(interfaceState->actionCount);
 
        if (updateResult)
        {
            propertyResponse.statusCode = 200;
            propertyResponse.statusDescription = "actionCount property is updated successfully";
        }
        else
        {
            LogError("PUSHBUTTON_INTERFACE: failed to update writable property actionCount...");

            propertyResponse.statusCode = 500;
            propertyResponse.statusDescription = "failed to update writable property";
        }
    }
    else
    {
        LogError("PUSHBUTTON_INTERFACE: Invalid desired value for actionCount, value should be Integer");
        propertyResponse.statusCode = 500;
        propertyResponse.statusDescription = "Invalid desired value for actionCount, value should be Integer";
    }
    json_value_free(jsonValue);

    // DigitalTwin_InterfaceClient_ReportPropertyAsync takes the DIGITALTWIN_CLIENT_PROPERTY_RESPONSE and returns information back to service.
    DIGITALTWIN_CLIENT_RESULT result = DigitalTwin_InterfaceClient_ReportPropertyAsync(appState.interfaceClientHandle, PushButtonInterface_ActionCountProperty, 
        dtClientPropertyUpdate->propertyDesired, dtClientPropertyUpdate->propertyDesiredLen, &propertyResponse,
        PushButtonInterface_PropertyCallback, PushButtonInterface_ActionCountProperty);
    if (result != DIGITALTWIN_CLIENT_OK)
    {
        LogError("PUSHBUTTON_INTERFACE: DigitalTwin_InterfaceClient_ReportPropertyAsync for actionCount failed, error=<%s>", MU_ENUM_TO_STRING(DIGITALTWIN_CLIENT_RESULT, result));
    }
    else
    {
        LogInfo("PUSHBUTTON_INTERFACE: Successfully queued property update for actionCount");
    }
}

// PushButtonInterface_ProcessPropertyUpdate receives updated properties from the server.  This implementation
// acts as a simple dispatcher to the functions to perform the actual processing.
static void PushButtonInterface_ProcessPropertyUpdate(const DIGITALTWIN_CLIENT_PROPERTY_UPDATE* dtClientPropertyUpdate, void* userInterfaceContext)
{
    if (strcmp(dtClientPropertyUpdate->propertyName, PushButtonInterface_ActionCountProperty) == 0)
    {
        PushButtonInterface_Property_ActionCountCallback(dtClientPropertyUpdate, userInterfaceContext);
        return;
    }

    // If the property is not implemented by this interface, presently we only record a log message but do not have a mechanism to report back to the service
    LogError("PUSHBUTTON_INTERFACE: Property name <%s> is not associated with this interface", dtClientPropertyUpdate->propertyName);
}

// PushButtonInterface_TelemetryCallback is invoked when a DigitalTwin telemetry message is either successfully delivered to the service or else fails.
static void PushButtonInterface_TelemetryCallback(DIGITALTWIN_CLIENT_RESULT digitalTwinTelemetryStatus, void* userContextCallback)
{
    (void)userContextCallback;
    if (digitalTwinTelemetryStatus == DIGITALTWIN_CLIENT_OK)
    {
        SendTelemetry_Succeeded_Callback("PushButton");
        LogInfo("PUSHBUTTON_INTERFACE: DigitalTwin successfully delivered telemetry message.");
    }
    else
    {
        SendTelemetry_Error_Callback("PushButton");
        LogError("PUSHBUTTON_INTERFACE: DigitalTwin failed to deliver telemetry message, error=<%s> ", MU_ENUM_TO_STRING(DIGITALTWIN_CLIENT_RESULT, digitalTwinTelemetryStatus));
    }
}

DIGITALTWIN_CLIENT_RESULT PushButtonInterface_Telemetry_SendAll()
{
    if (appState.interfaceClientHandle == NULL)
    {
        LogError("PUSHBUTTON_INTERFACE: interfaceClientHandle is required to be initialized before sending telemetries");
    }

    DIGITALTWIN_CLIENT_RESULT result;

    char combinedMessage[MAX_MESSAGE_SIZE];
    char telemetryValue[MAX_MESSAGE_SIZE];

    // Send multiple telemetries in a single message
    sprintf(combinedMessage, "{");

    PushButton_SerializeSingleClickTelemetry(telemetryValue, MAX_MESSAGE_SIZE);

    sprintf(combinedMessage + strlen(combinedMessage), "\"%s\":%s,", PushButtonInterface_SingleClickTelemetry, telemetryValue);

    PushButton_SerializeDoubleClickTelemetry(telemetryValue, MAX_MESSAGE_SIZE);

    sprintf(combinedMessage + strlen(combinedMessage), "\"%s\":%s,", PushButtonInterface_DoubleClickTelemetry, telemetryValue);

    PushButton_SerializeTripleClickTelemetry(telemetryValue, MAX_MESSAGE_SIZE);

    sprintf(combinedMessage + strlen(combinedMessage), "\"%s\":%s,", PushButtonInterface_TripleClickTelemetry, telemetryValue);

    PushButton_SerializeLongPressTelemetry(telemetryValue, MAX_MESSAGE_SIZE);

    sprintf(combinedMessage + strlen(combinedMessage), "\"%s\":%s,", PushButtonInterface_LongPressTelemetry, telemetryValue);

    PushButton_SerializeSuperLongPressTelemetry(telemetryValue, MAX_MESSAGE_SIZE);

    sprintf(combinedMessage + strlen(combinedMessage), "\"%s\":%s", PushButtonInterface_SuperLongPressTelemetry, telemetryValue);

    sprintf(combinedMessage + strlen(combinedMessage), "}");

    if ((result = DigitalTwin_InterfaceClient_SendTelemetryAsync(appState.interfaceClientHandle, (unsigned char*)combinedMessage, strlen(combinedMessage),
        PushButtonInterface_TelemetryCallback, NULL)) != DIGITALTWIN_CLIENT_OK)
    {
        LogError("PUSHBUTTON_INTERFACE: DigitalTwin_InterfaceClient_SendTelemetryAsync failed for sending telemetry.");
    }

    return result;
}

// PushButtonInterface_InterfaceRegisteredCallback is invoked when this interface
// is successfully or unsuccessfully registered with the service, and also when the interface is deleted.
static void PushButtonInterface_InterfaceRegisteredCallback(DIGITALTWIN_CLIENT_RESULT dtInterfaceStatus, void* userInterfaceContext)
{
    LogInfo("PushButtonInterface_InterfaceRegisteredCallback with status=<%s>, userContext=<%p>", MU_ENUM_TO_STRING(DIGITALTWIN_CLIENT_RESULT, dtInterfaceStatus), userInterfaceContext);
    if (dtInterfaceStatus == DIGITALTWIN_CLIENT_OK)
    {
        // Once the interface is registered, send our reported properties to the service.  
        // It *IS* safe to invoke most DigitalTwin API calls from a callback thread like this, though it 
        // is NOT safe to create/destroy/register interfaces now.
        LogInfo("PUSHBUTTON_INTERFACE: Interface successfully registered.");
    }
    else if (dtInterfaceStatus == DIGITALTWIN_CLIENT_ERROR_INTERFACE_UNREGISTERING)
    {
        // Once an interface is marked as unregistered, it cannot be used for any DigitalTwin SDK calls.
        LogInfo("PUSHBUTTON_INTERFACE: Interface received unregistering callback.");
    }
    else
    {
        LogError("PUSHBUTTON_INTERFACE: Interface received failed, status=<%s>.", MU_ENUM_TO_STRING(DIGITALTWIN_CLIENT_RESULT, dtInterfaceStatus));
    }
}

//
// Create DigitalTwin interface client handle
//
DIGITALTWIN_INTERFACE_CLIENT_HANDLE PushButtonInterface_Create()
{
    DIGITALTWIN_INTERFACE_CLIENT_HANDLE interfaceHandle;
    DIGITALTWIN_CLIENT_RESULT result;

    memset(&appState, 0, sizeof(PUSHBUTTON_INTERFACE_STATE));

    if ((result = DigitalTwin_InterfaceClient_Create(PushButtonInterfaceId,  PushButtonInterfaceInstanceName, PushButtonInterface_InterfaceRegisteredCallback, (void*)&appState, &interfaceHandle)) != DIGITALTWIN_CLIENT_OK)
    {
        LogError("PUSHBUTTON_INTERFACE: Unable to allocate interface client handle for interfaceId=<%s>, interfaceInstanceName=<%s>, error=<%s>", PushButtonInterfaceId, PushButtonInterfaceInstanceName, MU_ENUM_TO_STRING(DIGITALTWIN_CLIENT_RESULT, result));
        interfaceHandle = NULL;
    }

    else if ((result = DigitalTwin_InterfaceClient_SetPropertiesUpdatedCallback(interfaceHandle, PushButtonInterface_ProcessPropertyUpdate, (void*)&appState)) != DIGITALTWIN_CLIENT_OK)
    {
        LogError("PUSHBUTTON_INTERFACE: DigitalTwin_InterfaceClient_SetPropertiesUpdatedCallback failed. error=<%s>", MU_ENUM_TO_STRING(DIGITALTWIN_CLIENT_RESULT, result));
        PushButtonInterface_Close(interfaceHandle);
        interfaceHandle = NULL;
    }

    else
    {
        LogInfo("PUSHBUTTON_INTERFACE: Created DIGITALTWIN_INTERFACE_CLIENT_HANDLE successfully for interfaceId=<%s>, interfaceInstanceName=<%s>, handle=<%p>", PushButtonInterfaceId, PushButtonInterfaceInstanceName, interfaceHandle);
        appState.interfaceClientHandle = interfaceHandle;
    }

    return interfaceHandle;
}

void PushButtonInterface_Close(DIGITALTWIN_INTERFACE_CLIENT_HANDLE digitalTwinInterfaceClientHandle)
{
    // On shutdown, in general the first call made should be to DigitalTwin_InterfaceClient_Destroy.
    // This will block if there are any active callbacks in this interface, and then
    // mark the underlying handle such that no future callbacks shall come to it.
    DigitalTwin_InterfaceClient_Destroy(digitalTwinInterfaceClientHandle);

    // After DigitalTwin_InterfaceClient_Destroy returns, it is safe to assume
    // no more callbacks shall arrive for this interface and it is OK to free
    // resources callbacks otherwise may have needed.

}
