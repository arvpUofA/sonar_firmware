/*
 * UAVCAN data structure definition for libcanard.
 *
 * Autogenerated, do not edit.
 *
 * Source file: /home/isthatme/Documents/Clubs/ARVP/sonar_firmware/dsdl/uavcan/equipment/gnss/1060.Fix.uavcan
 */

#ifndef __UAVCAN_EQUIPMENT_GNSS_FIX
#define __UAVCAN_EQUIPMENT_GNSS_FIX

#include <stdint.h>
#include "canard.h"

#ifdef __cplusplus
extern "C"
{
#endif

#include <uavcan/Timestamp.h>

/******************************* Source text **********************************
#
# GNSS navigation solution with uncertainty.
# This message is deprecated. Use the newer 1063.Fix2.uavcan message.
#

uavcan.Timestamp timestamp         # Global network-synchronized time, if available, otherwise zero

#
# Time solution.
# Time standard (GPS, UTC, TAI, etc) is defined in the field below.
#
uavcan.Timestamp gnss_timestamp

#
# Time standard used in the GNSS timestamp field.
#
uint3 GNSS_TIME_STANDARD_NONE = 0  # Time is unknown
uint3 GNSS_TIME_STANDARD_TAI  = 1
uint3 GNSS_TIME_STANDARD_UTC  = 2
uint3 GNSS_TIME_STANDARD_GPS  = 3
uint3 gnss_time_standard

void5   # Reserved space

#
# If known, the number of leap seconds allows to perform conversions between some time standards.
#
uint8 NUM_LEAP_SECONDS_UNKNOWN = 0
uint8 num_leap_seconds

#
# Position and velocity solution
#
int37 longitude_deg_1e8            # Longitude degrees multiplied by 1e8 (approx. 1 mm per LSB)
int37 latitude_deg_1e8             # Latitude degrees multiplied by 1e8 (approx. 1 mm per LSB on equator)
int27 height_ellipsoid_mm          # Height above ellipsoid in millimeters
int27 height_msl_mm                # Height above mean sea level in millimeters

float16[3] ned_velocity            # NED frame (north-east-down) in meters per second

#
# Fix status
#
uint6 sats_used

uint2 STATUS_NO_FIX    = 0
uint2 STATUS_TIME_ONLY = 1
uint2 STATUS_2D_FIX    = 2
uint2 STATUS_3D_FIX    = 3
uint2 status

#
# Precision
#
float16 pdop

void4
float16[<=9] position_covariance   # m^2
float16[<=9] velocity_covariance   # (m/s)^2
******************************************************************************/

/********************* DSDL signature source definition ***********************
uavcan.equipment.gnss.Fix
uavcan.Timestamp timestamp
uavcan.Timestamp gnss_timestamp
saturated uint3 gnss_time_standard
void5
saturated uint8 num_leap_seconds
saturated int37 longitude_deg_1e8
saturated int37 latitude_deg_1e8
saturated int27 height_ellipsoid_mm
saturated int27 height_msl_mm
saturated float16[3] ned_velocity
saturated uint6 sats_used
saturated uint2 status
saturated float16 pdop
void4
saturated float16[<=9] position_covariance
saturated float16[<=9] velocity_covariance
******************************************************************************/

#define UAVCAN_EQUIPMENT_GNSS_FIX_ID                       1060
#define UAVCAN_EQUIPMENT_GNSS_FIX_NAME                     "uavcan.equipment.gnss.Fix"
#define UAVCAN_EQUIPMENT_GNSS_FIX_SIGNATURE                (0x54C1572B9E07F297ULL)

#define UAVCAN_EQUIPMENT_GNSS_FIX_MAX_SIZE                 ((628 + 7)/8)

// Constants
#define UAVCAN_EQUIPMENT_GNSS_FIX_GNSS_TIME_STANDARD_NONE                     0 // 0
#define UAVCAN_EQUIPMENT_GNSS_FIX_GNSS_TIME_STANDARD_TAI                      1 // 1
#define UAVCAN_EQUIPMENT_GNSS_FIX_GNSS_TIME_STANDARD_UTC                      2 // 2
#define UAVCAN_EQUIPMENT_GNSS_FIX_GNSS_TIME_STANDARD_GPS                      3 // 3
#define UAVCAN_EQUIPMENT_GNSS_FIX_NUM_LEAP_SECONDS_UNKNOWN                    0 // 0
#define UAVCAN_EQUIPMENT_GNSS_FIX_STATUS_NO_FIX                               0 // 0
#define UAVCAN_EQUIPMENT_GNSS_FIX_STATUS_TIME_ONLY                            1 // 1
#define UAVCAN_EQUIPMENT_GNSS_FIX_STATUS_2D_FIX                               2 // 2
#define UAVCAN_EQUIPMENT_GNSS_FIX_STATUS_3D_FIX                               3 // 3

#define UAVCAN_EQUIPMENT_GNSS_FIX_NED_VELOCITY_LENGTH                                    3
#define UAVCAN_EQUIPMENT_GNSS_FIX_POSITION_COVARIANCE_MAX_LENGTH                         9
#define UAVCAN_EQUIPMENT_GNSS_FIX_VELOCITY_COVARIANCE_MAX_LENGTH                         9

typedef struct
{
    // FieldTypes
    uavcan_Timestamp timestamp;                     //
    uavcan_Timestamp gnss_timestamp;                //
    uint8_t    gnss_time_standard;            // bit len 3
    // void5
    uint8_t    num_leap_seconds;              // bit len 8
    int64_t    longitude_deg_1e8;             // bit len 37
    int64_t    latitude_deg_1e8;              // bit len 37
    int32_t    height_ellipsoid_mm;           // bit len 27
    int32_t    height_msl_mm;                 // bit len 27
    float      ned_velocity[3];               // Static Array 16bit[3] max items
    uint8_t    sats_used;                     // bit len 6
    uint8_t    status;                        // bit len 2
    float      pdop;                          // float16 Saturate
    // void4
    struct
    {
        uint8_t    len;                       // Dynamic array length
        float*     data;                      // Dynamic Array 16bit[9] max items
    } position_covariance;
    struct
    {
        uint8_t    len;                       // Dynamic array length
        float*     data;                      // Dynamic Array 16bit[9] max items
    } velocity_covariance;

} uavcan_equipment_gnss_Fix;

extern
uint32_t uavcan_equipment_gnss_Fix_encode(uavcan_equipment_gnss_Fix* source, void* msg_buf);

extern
int32_t uavcan_equipment_gnss_Fix_decode(const CanardRxTransfer* transfer, uint16_t payload_len, uavcan_equipment_gnss_Fix* dest, uint8_t** dyn_arr_buf);

extern
uint32_t uavcan_equipment_gnss_Fix_encode_internal(uavcan_equipment_gnss_Fix* source, void* msg_buf, uint32_t offset, uint8_t root_item);

extern
int32_t uavcan_equipment_gnss_Fix_decode_internal(const CanardRxTransfer* transfer, uint16_t payload_len, uavcan_equipment_gnss_Fix* dest, uint8_t** dyn_arr_buf, int32_t offset);

#ifdef __cplusplus
} // extern "C"
#endif
#endif // __UAVCAN_EQUIPMENT_GNSS_FIX