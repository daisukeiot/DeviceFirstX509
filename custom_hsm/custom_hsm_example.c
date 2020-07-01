// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hsm_client_data.h"

static const char* COMMON_NAME = "PostmanDevice001";

typedef struct CUSTOM_HSM_SAMPLE_INFO_TAG
{
    const char* certificate;
    const char* common_name;
    const char* key;
} CUSTOM_HSM_SAMPLE_INFO;

static char *CERTIFICATE = NULL; 
static char *PRIVATE_KEY = NULL; 

int hsm_client_x509_init()
{
    FILE * fp;
    int fileSize; 

    (void)printf("hsm_client_x509_init()\r\n");

    if (CERTIFICATE == NULL)
    {
        fp = fopen("./new-device.cert.pem", "r");

        if (fp ==NULL)
        {
            (void)printf("./new-device.cert.pem not found\r\n");
            return -1;
        }

        fseek(fp, 0L, SEEK_END);
        fileSize = ftell(fp);
        (void)printf("File size %d\r\n", fileSize);
        fseek(fp, 0, SEEK_SET);

        CERTIFICATE = malloc(fileSize + 1);

        if (CERTIFICATE == NULL)
        {
            (void)printf("failed to allocate buffer for ./new-device.cert.pem\r\n");
            return -1;
        }

        fread(CERTIFICATE, fileSize, 1, fp);
        (void)printf("Certificate :\r\n%s\r\n", CERTIFICATE);

        fclose(fp);
    }

    if (PRIVATE_KEY == NULL)
    {
        fp = fopen("./new-device.key.pem", "r");

        if (fp ==NULL)
        {
            (void)printf("./new-device.key.pem not found\r\n");
            return -1;
        }

        fseek(fp, 0L, SEEK_END);
        fileSize = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        PRIVATE_KEY = malloc(fileSize + 1);

        if (PRIVATE_KEY == NULL)
        {
            (void)printf("failed to allocate buffer for ./new-device.key.pem.pem\r\n");
            return -1;
        }

        fread(PRIVATE_KEY, fileSize, 1, fp);
        (void)printf("Private Key : \r\n%s\r\n", PRIVATE_KEY);

        fclose(fp);
    }

    return 0;
}

void hsm_client_x509_deinit()
{
    (void)printf("hsm_client_x509_deinit()\r\n");

    if (CERTIFICATE != NULL)
    {
        free(CERTIFICATE);
        CERTIFICATE = NULL;
    }

    if (PRIVATE_KEY != NULL)
    {
        free(PRIVATE_KEY);
        PRIVATE_KEY = NULL;
    }
}


HSM_CLIENT_HANDLE custom_hsm_create()
{
    HSM_CLIENT_HANDLE result;
    CUSTOM_HSM_SAMPLE_INFO* hsm_info = malloc(sizeof(CUSTOM_HSM_SAMPLE_INFO));
    
    (void)printf("custom_hsm_create()\r\n");

    if (hsm_info == NULL)
    {
        (void)printf("Failued allocating hsm info\r\n");
        result = NULL;
    }
    else
    {
        hsm_info->certificate = CERTIFICATE;
        hsm_info->key = PRIVATE_KEY;
        hsm_info->common_name = COMMON_NAME;
        result = hsm_info;
    }
    return result;
}

void custom_hsm_destroy(HSM_CLIENT_HANDLE handle)
{
    (void)printf("custom_hsm_destroy()\r\n");

    if (handle != NULL)
    {
        CUSTOM_HSM_SAMPLE_INFO* hsm_info = (CUSTOM_HSM_SAMPLE_INFO*)handle;
        // Free anything that has been allocated in this module
        free(hsm_info);
    }
}

char* custom_hsm_get_certificate(HSM_CLIENT_HANDLE handle)
{
    char* result;

    (void)printf("custom_hsm_get_certificate()\r\n");
    if (handle == NULL)
    {
        (void)printf("Invalid handle value specified\r\n");
        result = NULL;
    }
    else
    {
        // TODO: Malloc the certificate for the iothub sdk to free
        // this value will be sent unmodified to the tlsio
        // layer to be processed
        CUSTOM_HSM_SAMPLE_INFO* hsm_info = (CUSTOM_HSM_SAMPLE_INFO*)handle;
        size_t len = strlen(hsm_info->certificate);
        if ((result = (char*)malloc(len + 1)) == NULL)
        {
            (void)printf("Failure allocating certificate\r\n");
            result = NULL;
        }
        else
        {
            strcpy(result, hsm_info->certificate);
        }
    }
    return result;
}

char* custom_hsm_get_key(HSM_CLIENT_HANDLE handle)
{
    char* result;

    (void)printf("custom_hsm_get_key()\r\n");
    if (handle == NULL)
    {
        (void)printf("Invalid handle value specified\r\n");
        result = NULL;
    }
    else
    {
        // TODO: Malloc the private key for the iothub sdk to free
        // this value will be sent unmodified to the tlsio
        // layer to be processed
        CUSTOM_HSM_SAMPLE_INFO* hsm_info = (CUSTOM_HSM_SAMPLE_INFO*)handle;
        size_t len = strlen(hsm_info->key);
        if ((result = (char*)malloc(len + 1)) == NULL)
        {
            (void)printf("Failure allocating certificate\r\n");
            result = NULL;
        }
        else
        {
            strcpy(result, hsm_info->key);
        }
    }
    return result;
}

char* custom_hsm_get_common_name(HSM_CLIENT_HANDLE handle)
{
    char* result;
   if (handle == NULL)
    {
        (void)printf("Invalid handle value specified\r\n");
        result = NULL;
    }
    else
    {
        // TODO: Malloc the common name for the iothub sdk to free
        // this value will be sent to dps
        CUSTOM_HSM_SAMPLE_INFO* hsm_info = (CUSTOM_HSM_SAMPLE_INFO*)handle;
        size_t len = strlen(hsm_info->common_name);
        if ((result = (char*)malloc(len + 1)) == NULL)
        {
            (void)printf("Failure allocating certificate\r\n");
            result = NULL;
        }
        else
        {
            strcpy(result, hsm_info->common_name);
        }
    }

    (void)printf("custom_hsm_get_common_name() : %s\r\n", result);

    return result;
}


// Defining the v-table for the x509 hsm calls
static const HSM_CLIENT_X509_INTERFACE x509_interface =
{
    custom_hsm_create,
    custom_hsm_destroy,
    custom_hsm_get_certificate,
    custom_hsm_get_key,
    custom_hsm_get_common_name
};

const HSM_CLIENT_X509_INTERFACE* hsm_client_x509_interface()
{
    // x509 interface pointer
    printf("hsm_client_x509_interface()\r\n");
    return &x509_interface;
}

const HSM_CLIENT_TPM_INTERFACE* hsm_client_tpm_interface()
{
    return NULL;
}

int hsm_client_tpm_init()
{
    return 0;
}

void hsm_client_tpm_deinit()
{
    return;
}

const HSM_CLIENT_KEY_INTERFACE* hsm_client_key_interface()
{
    return NULL;
}
