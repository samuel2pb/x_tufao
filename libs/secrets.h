#include <pgmspace.h>

#define SECRET
#define THINGNAME "dht22" // change this

const char WIFI_SSID[] = "***********";                                                // change this
const char WIFI_PASSWORD[] = "**********";                                        // change this
const char AWS_IOT_ENDPOINT[] = "a3e0jhoa9ehxhx-ats.iot.us-east-1.amazonaws.com"; // change this

// Amazon Root CA 1
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
 -----END CERTIFICATE-----
)EOF";

// Device Certificate                                               //change this
static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWTCCAkGgAwIBAgIUIJIKPjOdO3Sq9pAESKos7Md4acgwDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTIyMDgyNTE2NTA1
MFoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAKZpY9yYef9XiVIR0ov6
mLI1jftSXIRKts/Ri+9qcatawJwNtDQditowwu/kfrDtl4NoeVNMXGwtrCzaOJWY
ooIfSklXWVhjsdeNdj8UVFE385LRBpiBVDBK/W+6xsETbth7pfMLwMcyweQVGAU6
VY5VwWWL3v9O2Kbi9RZKM3yI+f2WIlgQ+jxWUhJgZxGG4dkNSsK6+NonF/xUEQO/
sRF15hpyLKTuNtxs+sjhoGldRrHP3W3AM6C9ks00/3eezYxZlWQQu8ccyJHMMEWg
9oyTv6ez6ODSN7B97HSAzXi0b/dBXYeZd29TyRs72C7dmdJgkANP+8MvlwDcfCO3
InMCAwEAAaNgMF4wHwYDVR0jBBgwFoAUC1Afnz0SiI3fJL1MOVnl5dQfkjowHQYD
VR0OBBYEFG2Pqmhsmo/nSeBp9A46vx90xLtMMAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQDh+NpXasEM8f4OCCtFpZmnGRPN
9Urb+cC1WrmHSSKTTtwBH+Hk5HiaKmg5chdAGIBnlskJ7nMvBTGJIKnS01ThaX62
CTn2KjfoZ0R40GG83OtVhb3d/iVkwzoX8TvuhdF9cRwPgx1Jkb2mEehqnNneEYVG
IOuZG1dhvouFcAEcY5s/wuzRuq3gD5FkjG0IaNM90h0v2fAqsYeo1uxB+uRAPNgn
5fl/zb7srb4/nANqzmgXRbIhO5iH5vfP6gxjIpvnS3eQD08kG1EzncaEVPi5VVIm
CUXi0N80aYlRX/iXlItvPre1tRTWqzuXp6JaRYrBej06kXfR+JvozCC6DRTQ
 -----END CERTIFICATE-----
)KEY";

// Device Private Key                                               //change this
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----

-----END RSA PRIVATE KEY----- 
)KEY";
