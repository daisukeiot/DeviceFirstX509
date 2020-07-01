# Build the Generated CMake Project on Linux

One of the features VS Code Digital Twin tooling provides is generating stub code based on the Device Capability Model (DCM) you specified.

Follow the steps to use the generated code with the Azure IoT Device C SDK source to compile a device app.

For more details about setting up your development environment for compiling the C Device SDK. Check the [instructions](https://github.com/Azure/azure-iot-sdk-c/blob/master/iothub_client/readme.md#compiling-the-c-device-sdk) for each platform.

## Prerequisite
1. Make sure all dependencies are installed before building the SDK. For Ubuntu, you can use apt-get to install the right packages.
    ```bash
    sudo apt-get update
    sudo apt-get install -y git cmake build-essential curl libcurl4-openssl-dev libssl-dev uuid-dev
    ```

1. Verify that **CMake** is at least version **2.8.12** and **gcc** is at least version **4.4.7**.
    ```bash
    cmake --version
    gcc --version
    ```

## Set up environment

In order to providion a device as an IoT Plug and Play using X.509 certificate, we need :

1. Azure IoT SDK C, Public Preview (As of 6/30/2020)  

1. Generate a X.509 certificate and a private key  

    In this instruction, we will use self-signed certificate using script files from Azure IoT SDK

    When the certificate is generated, registration id (or device id) is used for the common name.  
    The default name is `PostmanDevice001`.  In you prefer to use a different name, please edit line #3 in `prep.sh`  

    ```bash
    REGISTRATION_ID='PostmanDevice001'
    ```

1. Modify and build a custom HSM library based on `custom_hsm_example`

    Change `COMMON_NAME` in `custom_hsm_example.c` in `custom_hsm` folder.  

    > [!IMPORTANT]  
    > The name has to match to what is specified for the certificate above.

    ```c
    static const char* COMMON_NAME = "PostmanDevice001";
    ```

1. Certificate files  

    The custom HSM library reads certificate from `cert.pem` and the private key from `key.pem`.

    With self-signed certificate, 2 PEM files are generated and copied into `cmake` folder for the executable to read from the PEM files.

    - `certificate/certs/new-device.cert.pem` => `cmake/new-device.cert.pem`
    - `certificate/private/new-device.key.pem` => `cmake/new-device.key.pem`

    > [!TIP]  
    > For production scenarios, you may hard code certificate or read from a secure storage, or read from SIM card, for example.

## Run `prep.sh` script

This script does :

1. Clone Azure IoT SDK C
1. Copy scripts to generate certificates to `certificate` folder
1. Copy generated certificate file and private key file to `cmake` folder
1. Copy `custom_hsm_example.c` to `azure-iot-sdk-c/provisioning_client/samples/custom_hsm_example`

## Create a new device identity

### IoT Central

You can manually create a new device in your IoT Central application, or use Postman collection to create a new device identity.  

#### Instructions to create enrollment entries in IoT Central

Group Enrollment : <https://docs.microsoft.com/en-us/azure/iot-central/core/concepts-get-connected#connect-devices-using-x509-certificates>

Individual Enrollment : <https://docs.microsoft.com/en-us/azure/iot-central/core/concepts-get-connected#individual-enrollment-based-device-connectivity>

### Device Provisioning Service (DPS)

Create individual enrollment in DPS

#### Instructions to create enrollment entries in DPS

Group Enrollment : <https://docs.microsoft.com/en-us/azure/iot-dps/tutorial-group-enrollments>

Individual Enrollment : <https://docs.microsoft.com/en-us/azure/iot-dps/quick-create-simulated-device-x509#create-a-device-enrollment-entry-in-the-portal>

## Automatically associate a device template based on IoT Plug and Play Model ID

As a part of provisioning, IoT Plug and Play enabled device app communicates Model ID.  Based on this information, IoT Central can associate the appropriate device template.

> [!IMPORTANT]  
> With Public Preview, IoT central can associate the device template only if the specified device capability model is published to global repository.  If the DCM is not publised to the global repository, you need to create a device template with matching Model ID in your IoT Central application

Reference : <https://docs.microsoft.com/en-us/azure/iot-central/core/concepts-get-connected#automatically-associate-with-a-device-template>

> [!NOTE] (as of 6/30/2020)  
> It seems the device template must be added to IoT Central app for automatic association to work.  Without the device template, their first provisioning succeeds but the device is not assigned to the device template.  On the 2nd provisioning, the association is made.

## Assigning certificates to device identity

In order for a device to be authenticated using X.509, the certificate must be assigned to the device identity.  

You can manually upload the certificate (new-device.cert.pem, in our example), but this requires device template assignment.  In other words, you need to know which certificate belongs to which device and which device template the device is supposed to use.  In certain scenarios, this prevents zero touch provisioning (e.g. the end user simply turns on the device)

Without device template association, you can not browse to device's page to click `Connect` button

![IoTC-001](media/IoTC-001.png)

Once you are in device's overview page, you can click `Connect` button

![IoTC-002](media/IoTC-002.png)

This is the UI to upload `certificate`

![IoTC-003](media/IoTC-003.png)

If the scenario requires automatic device template association, please set certificate using REST API

## Device Identity and certificate using REST API

This requires following REST API calls

1. Create a device template  

    This step is required only if the DCM is not published to the public repository
    <https://docs.microsoft.com/en-us/rest/api/iotcentral/devicetemplates/set>

    Example :

    - Device Template Id : `PostmanDeviceTemplate`
    - DCM ID : urn:iotcpnptest:postmansample:1

    - Method : PUT

    - URL : https://iotpnp-with-meshsystems.azureiotcentral.com/api/preview/deviceTemplates/PostmanDeviceTemplate

    - Headers  
    Authorization : API Key from IoT Central  
    Content-Type : application/json  

    - Body :

        ```json
        {
          "types": [
            "DeviceModel"
          ],
          "displayName": "IoT Central Postman Sample",
          "capabilityModel": {
            "@id": "urn:iotcpnptest:postmansample:1",
            "@type": [
              "CapabilityModel"
            ],
            "implements": [
              {
                "@id": "urn:iotcpnptest:DeviceInfo:2",
            "@type": [

              :
              :
            ```

1. Create a new device identify

    <https://docs.microsoft.com/en-us/rest/api/iotcentral/devices/set>

    Example :

    - Device Id (or Registration ID) : `PostmanDevice001`
    - Method : PUT

    - URL : https://my-iot-central.azureiotcentral.com/api/preview/devices/PostmanDevice001

    - Headers  
    Authorization : API Key from IoT Central  
    Content-Type : application/json  

    - Body :

        ```json
        {
          "displayName": "Postman Test Device",
          "simulated": false,
          "approved": true
        }
        ```

1. Assign a certificate to the new device identity

    <https://docs.microsoft.com/en-us/rest/api/iotcentral/devices/setattestation>

    - Method : PUT

    - URL : https://my-iot-central.azureiotcentral.com/api/preview/devices/PostmanDevice001/attestation

    - Headers  
    Authorization : API Key from IoT Central  
    Content-Type : application/json  

    - Body :

        Send base64 encoded certificate value in the body.  This is the contents of `new-device.cert.pem`

        ```json
        {
          "type": "X509Attestation",
          "x509": {
            "clientCertificates": {
              "primary": {
                "certificate": "<Base64 Certificate contents in a single line>"
              }
            }
          }
        }
        ```

        > [!TIP]  
        > Make sure to format base64 encoded certificate value in a single line.  
        >  "certificate": "MIIFwzCCA6ugAwIBAgIBAjANBg........"

### Make REST API calls with Postman

You may import Postman collection and environment variables to make REST API calls to IoT Central.

To use the Postman collection, please configure environment variables

1. Import Postman Collection from these json files  
    `./Postman/DeviceFirstProvision.postman_collection.json`  
    `./Postman/IoTCentral.postman_environment.json.json`  

1. Ensure the environment `IoTCentral` is selected  

    ![Postman-001](media/Postman-001.png)

1. Add values  
    ![Postman-002](media/Postman-002.png)

| Variable             | Value                         | Notes                                                                                |
|----------------------|-------------------------------|--------------------------------------------------------------------------------------|
| IoTCDeviceTemplateId | Device Template ID            |                                                                                      |
| IoTCAppUrl           | IoT Central Application URL   | from `Administration` -> `Your application`                                          |
| IoTCDeviceId         | Device Id for the device      | This must match to the device id (or common name) used to generate X.509 certificate |
| IoTCApiKey           | API Key to access IoT Central | from `Administration` -> `API tokens`                                                |

## Run the app

Run the app with :

```bash
cd cmake
./DevicefirstX509 <Scope ID>
```
