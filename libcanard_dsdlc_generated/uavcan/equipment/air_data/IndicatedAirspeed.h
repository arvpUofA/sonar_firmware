/*
 * UAVCAN data structure definition for libcanard.
 *
 * Autogenerated, do not edit.
 *
 * Source file: /home/isthatme/Documents/Clubs/ARVP/sonar_firmware/dsdl/uavcan/equipment/air_data/1021.IndicatedAirspeed.uavcan
 */

#ifndef __UAVCAN_EQUIPMENT_AIR_DATA_INDICATEDAIRSPEED
#define __UAVCAN_EQUIPMENT_AIR_DATA_INDICATEDAIRSPEED

#include <stdint.h>
#include "canard.h"

#ifdef __cplusplus
extern "C"
{
#endif

/******************************* Source text **********************************
#
# IAS.
#

float16 indicated_airspeed              # m/s
float16 indicated_airspeed_variance     # (m/s)^2
******************************************************************************/

/********************* DSDL signature source definition ***********************
uavcan.equipment.air_data.IndicatedAirspeed
saturated float16 indicated_airspeed
saturated float16 indicated_airspeed_variance
******************************************************************************/

#define UAVCAN_EQUIPMENT_AIR_DATA_INDICATEDAIRSPEED_ID     1021
#define UAVCAN_EQUIPMENT_AIR_DATA_INDICATEDAIRSPEED_NAME   "uavcan.equipment.air_data.IndicatedAirspeed"
#define UAVCAN_EQUIPMENT_AIR_DATA_INDICATEDAIRSPEED_SIGNATURE (0xA1892D72AB8945FULL)

#define UAVCAN_EQUIPMENT_AIR_DATA_INDICATEDAIRSPEED_MAX_SIZE ((32 + 7)/8)

// Constants

typedef struct
{
    // FieldTypes
    float      indicated_airspeed;            // float16 Saturate
    float      indicated_airspeed_variance;   // float16 Saturate

} uavcan_equipment_air_data_IndicatedAirspeed;

extern
uint32_t uavcan_equipment_air_data_IndicatedAirspeed_encode(uavcan_equipment_air_data_IndicatedAirspeed* source, void* msg_buf);

extern
int32_t uavcan_equipment_air_data_IndicatedAirspeed_decode(const CanardRxTransfer* transfer, uint16_t payload_len, uavcan_equipment_air_data_IndicatedAirspeed* dest, uint8_t** dyn_arr_buf);

extern
uint32_t uavcan_equipment_air_data_IndicatedAirspeed_encode_internal(uavcan_equipment_air_data_IndicatedAirspeed* source, void* msg_buf, uint32_t offset, uint8_t root_item);

extern
int32_t uavcan_equipment_air_data_IndicatedAirspeed_decode_internal(const CanardRxTransfer* transfer, uint16_t payload_len, uavcan_equipment_air_data_IndicatedAirspeed* dest, uint8_t** dyn_arr_buf, int32_t offset);

#ifdef __cplusplus
} // extern "C"
#endif
#endif // __UAVCAN_EQUIPMENT_AIR_DATA_INDICATEDAIRSPEED