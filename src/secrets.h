/*
####################################################################
## Simple Mqtt Client Code for connecting to Eugenio.io IoT platform
####################################################################
##
##################################################
## Código feito por Artur Sarlo(artur.sarlo@la.logicalis.com) codigo 
## modificado por Luiz Henrique (luizhfmonteiro@gmail.com) para 
## Eugenio Challenge Hub
##################################################
*/

#ifndef SECRETS
#define SECRETS

/*
 #########################################################################################
 # CHANGE HERE
 #########################################################################################
*/
#define WIFI_SSID "Luiz-2.4G-"
#define WIFI_PASSWORD "ruanicolaucoelho"

#define DEVICE_ID "529a226c-f80f-4c26-8175-46f8e3c37ff8"

// Copy contents from CA certificate here ▼
static const char cacert[] PROGMEM = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDdzCCAl+gAwIBAgIEAgAAuTANBgkqhkiG9w0BAQUFADBaMQswCQYDVQQGEwJJ\n" \
"RTESMBAGA1UEChMJQmFsdGltb3JlMRMwEQYDVQQLEwpDeWJlclRydXN0MSIwIAYD\n" \
"VQQDExlCYWx0aW1vcmUgQ3liZXJUcnVzdCBSb290MB4XDTAwMDUxMjE4NDYwMFoX\n" \
"DTI1MDUxMjIzNTkwMFowWjELMAkGA1UEBhMCSUUxEjAQBgNVBAoTCUJhbHRpbW9y\n" \
"ZTETMBEGA1UECxMKQ3liZXJUcnVzdDEiMCAGA1UEAxMZQmFsdGltb3JlIEN5YmVy\n" \
"VHJ1c3QgUm9vdDCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAKMEuyKr\n" \
"mD1X6CZymrV51Cni4eiVgLGw41uOKymaZN+hXe2wCQVt2yguzmKiYv60iNoS6zjr\n" \
"IZ3AQSsBUnuId9Mcj8e6uYi1agnnc+gRQKfRzMpijS3ljwumUNKoUMMo6vWrJYeK\n" \
"mpYcqWe4PwzV9/lSEy/CG9VwcPCPwBLKBsua4dnKM3p31vjsufFoREJIE9LAwqSu\n" \
"XmD+tqYF/LTdB1kC1FkYmGP1pWPgkAx9XbIGevOF6uvUA65ehD5f/xXtabz5OTZy\n" \
"dc93Uk3zyZAsuT3lySNTPx8kmCFcB5kpvcY67Oduhjprl3RjM71oGDHweI12v/ye\n" \
"jl0qhqdNkNwnGjkCAwEAAaNFMEMwHQYDVR0OBBYEFOWdWTCCR1jMrPoIVDaGezq1\n" \
"BE3wMBIGA1UdEwEB/wQIMAYBAf8CAQMwDgYDVR0PAQH/BAQDAgEGMA0GCSqGSIb3\n" \
"DQEBBQUAA4IBAQCFDF2O5G9RaEIFoN27TyclhAO992T9Ldcw46QQF+vaKSm2eT92\n" \
"9hkTI7gQCvlYpNRhcL0EYWoSihfVCr3FvDB81ukMJY2GQE/szKN+OMY3EU/t3Wgx\n" \
"jkzSswF07r51XgdIGn9w/xZchMB5hbgF/X++ZRGjD8ACtPhSNzkE1akxehi/oCr0\n" \
"Epn3o0WC4zxe9Z2etciefC7IpJ5OCBRLbf1wbWsaY71k5h+3zvDyny67G7fyUIhz\n" \
"ksLi4xaNmjICq44Y3ekQEe5+NauQrz4wlHrQMz2nZQ/1/I6eYs9HRCwBXbsdtTLS\n" \
"R9I4LtD+gdwyah617jzV/OeBHRnDJELqYzmp\n" \
"-----END CERTIFICATE-----\n";

// Copy contents from cert.pem here ▼
static const char client_cert[] PROGMEM = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDCTCCAfGgAwIBAgIUSPrPkEIJnBBlpElq2l6qLD3wGpMwDQYJKoZIhvcNAQEL\n" \
"BQAwFDESMBAGA1UEAwwJbG9jYWxob3N0MB4XDTIwMTAwNjAwNTcwM1oXDTIxMTAw\n" \
"NjAwNTcwM1owFDESMBAGA1UEAwwJbG9jYWxob3N0MIIBIjANBgkqhkiG9w0BAQEF\n" \
"AAOCAQ8AMIIBCgKCAQEAzLOcje0+OjQ3kGQEO3b8WxdNOG4a9g8tPgVVkI8Adaz/\n" \
"SEXEbsL2cOFZJGyej/Pg+Fwm2quetD2w7zKOiMLBoK7KFAOV94lqAue3QzlNYUzW\n" \
"rPx/xst84nI5klEGYbSWnHwfNXGgngvB8AGtTm81XLA0XgnMPRgEU2/V8oHAHrwQ\n" \
"B+qnImJIFJSj8H/xzI+b1gu73/L4ObMJRv3wRqNilZctUfjpllgznalnsx3cZczW\n" \
"ExnPKufe2DXVuklNZPihzpOFPxoxsxfTe9HcTQuLsIE55+yh8PHBSPLF4+bNtbex\n" \
"+Koq0f1vQENK5yBtIqfIZfH6N9gckQD+byUnZKXm3wIDAQABo1MwUTAdBgNVHQ4E\n" \
"FgQUwo3YzVPV05smXee0HOhYcADaujMwHwYDVR0jBBgwFoAUwo3YzVPV05smXee0\n" \
"HOhYcADaujMwDwYDVR0TAQH/BAUwAwEB/zANBgkqhkiG9w0BAQsFAAOCAQEACTCz\n" \
"BkpVU6pDQAVA14sKe+SCh/mfhU0f7N1VlLtoV+snc866EQIlafvjpkg+YY0M3h/p\n" \
"u+cS1jHFwt6x0/mAMKLJp9H8+i6OKZm54kiOJtyVsaHG0AYozX6QyTtIAotDp5/m\n" \
"MnBSTDByB6CGKdwnA9LVOSmUdQJyDVRrJtCwre5MnP8EFU9tjDVvgh17rZh1VxRO\n" \
"nzLnOCAJ8u6nc6sakntQ0VSyoXoQXg+5e/oiq3Twb/25MA++ky1+Jlsp8qH4f56G\n" \
"euJOuuU4ex6N1xdpvPVIFY3cIBDDiv+7QHIfrQOQMh/tuoT46GNgKCgmcJc36dxx\n" \
"czALeK2LpoZd6gocFQ==\n" \
"-----END CERTIFICATE-----\n";

// Copy contents from  key.pem here ▼
static const char privkey[] PROGMEM = \
"-----BEGIN PRIVATE KEY-----\n" \
"MIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQDMs5yN7T46NDeQ\n" \
"ZAQ7dvxbF004bhr2Dy0+BVWQjwB1rP9IRcRuwvZw4VkkbJ6P8+D4XCbaq560PbDv\n" \
"Mo6IwsGgrsoUA5X3iWoC57dDOU1hTNas/H/Gy3zicjmSUQZhtJacfB81caCeC8Hw\n" \
"Aa1ObzVcsDReCcw9GARTb9XygcAevBAH6qciYkgUlKPwf/HMj5vWC7vf8vg5swlG\n" \
"/fBGo2KVly1R+OmWWDOdqWezHdxlzNYTGc8q597YNdW6SU1k+KHOk4U/GjGzF9N7\n" \
"0dxNC4uwgTnn7KHw8cFI8sXj5s21t7H4qirR/W9AQ0rnIG0ip8hl8fo32ByRAP5v\n" \
"JSdkpebfAgMBAAECggEAQg4Ditjvp6CJ/ZIIu5edv1c5l+gP0y76jnTyM3+xykdU\n" \
"6WF6DbPkCnDtbw+N6y3rfrld4FpZEBiIK+fIjuEp0kwU8XhmwEYba2ySeqLXEXrW\n" \
"G2QvZ1rqpbfuVATJZmX74+s5tuk/x0BMCMLBxfmk4pAJlf7bZSuG2dXQ2Dx+XmwF\n" \
"kq2ktybrAkZko7ZsE0yFoGRi7AHkFQ8CejB26htnWUsfwV9nE5oChcB4zUM0tUgo\n" \
"qHdfXHeCI0Af6R1745zETcx4G4PTnvWxMkyismgsnZDKkUGK54SLFFIeReKhau5Q\n" \
"2+iHvUMWTQ91pc5KluohB9Mx6p3QGOQ0YKBMH68uoQKBgQDk9pGBFbjVOSsK2fTs\n" \
"8b1KuY9nu4AtFD22rny7qq36hvavw4Zpk29ptgCa85/gbfItVrQYeOsz+afogJwX\n" \
"Y4SKARs6j6ZSUr23r+SY2ywyEAgM5+FDpN3WWYRMTbJLuG4iEBavWfO3fOiRD19Z\n" \
"JDUdlz2fMvcGjbqgwCH7rYKVXQKBgQDk36E0thcYZJI8bxE+W5CZS599iYp4XBZ4\n" \
"4cco9QZJo/mF+cHz8UVkRdmHSrOP4ysDkMUau/muqUOo1x+qTH9gQd0g/tCby7us\n" \
"n3RYU2GQqPp1EuDzLYfzWZqBn+DYZsAx/nIHJArWN8jmCRh4/B1ykUGHcoHC4+UG\n" \
"a2izJe7NawKBgQCIJb4b3gjMcXQQvc4QXybKGlEVw77SD2krCdKN3U4a3Movdbql\n" \
"aNAlMd5mRVvaHkaVgysQKkbh3yBi9qbamYikb0f7qJOj/CNn6KWr5xf+pN3EntWq\n" \
"XYyz+wLaRJEyyXZjtuFmX7y2bQX1zAhMf2U+NUKlOa7+LKS/FcCfYWt22QKBgQCQ\n" \
"u+rRzqTkRBVGKmaZLIZQqsX9OVFbs8BySTUM7YZAY3WUB9FjVpwvQQzCxbavs1Ss\n" \
"OIysz1qh3WBwETpxE1cpo/0WtI1vOHNCQsPsZy43sORRzNJ8dbqXZX6Tomsp7jB0\n" \
"bRfm9B/2P+jHr9DOEYD9xmXOJddxS3S1jo7zW3WcaQKBgEJEbgRWbpWo53Dow9Gp\n" \
"ay3CIQ9eIJyzgZMIIJ10x/Ptu9TKpAZVI1nhsdddFSuaHDnEix0LuC1oLb2LUZWh\n" \
"swQKwBe0oYjO1MIhtc0EaC45/I7ARQRiOq5EF2TFph1yuyvwfdcukIpBCE+m7gu6\n" \
"59P0/Dd9Uoi2QdU4TN9umqHR\n" \
"-----END PRIVATE KEY-----\n";

/*
#########################################################################################
#########################################################################################
*/

// NTP related
#define UTC_TIME_ZONE_BRAZIL_HOURS -3
#define UTC_TIME_ZONE_BRAZIL_SECONDS UTC_TIME_ZONE_BRAZIL_HOURS * 3600
//#define USE_SUMMER_TIME_DST  //uncomment to use DST
#define DST_HOURS 0
#define DST_SECONDS DST_HOURS * 3600
#define CURRENT_TIME_APROXIMATION_AS_EPOCH_SECONDS 1510592825
#define NTP_SERVER_0 "pool.ntp.org"
#define NTP_SERVER_1 "time.nist.gov"

// MQTT related
#define MQTT_BROKER "logicalis-eugeniostg-iothub.azure-devices.net"
#define MQTT_PORT 8883
#define MQTT_USER "logicalis-eugeniostg-iothub.azure-devices.net/" DEVICE_ID "/api-version=2019-03-30"

#define MQTT_SUB_CLOUD_TO_DEVICE_TOPIC "devices/529a226c-f80f-4c26-8175-46f8e3c37ff8/messages/devicebound/#"
#define MQTT_SUB_INVOKE_BASE_TOPIC "$iothub/methods/POST/"
#define MQTT_SUB_INVOKE_TOPIC "$iothub/methods/POST/#"

#define MQTT_PUB_INVOKE_REPLY_TOPIC "$iothub/methods/res/{status_code}/?$rid={rid}"
#define MQTT_PUB_DEVICE_TO_CLOUD_TOPIC "devices/529a226c-f80f-4c26-8175-46f8e3c37ff8/messages/events/"

// #define USE_ARDUINO_JSON_LIB

#endif