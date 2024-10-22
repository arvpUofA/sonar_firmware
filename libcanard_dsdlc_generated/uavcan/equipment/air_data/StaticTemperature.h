/*
 * UAVCAN data structure definition for libcanard.
 *
 * Autogenerated, do not edit.
 *
 * Source file: /home/isthatme/Documents/Clubs/ARVP/sonar_firmware/dsdl/uavcan/equipment/air_data/1029.StaticTemperature.uavcan
 */

#ifndef __UAVCAN_EQUIPMENT_AIR_DATA_STATICTEMPERATURE
#define __UAVCAN_EQUIPMENT_AIR_DATA_STATICTEMPERATURE

#include <stdint.h>
#include "canard.h"

#ifdef __cplusplus
extern "C"
{
#endif

/******************************* Source text **********************************
#
# Static temperature.
#

float16 static_temperature              # Kelvin
float16 static_temperature_variance     # Kelvin^2
******************************************************************************/

/********************* DSDL signature source definition ***********************
uavcan.equipment.air_data.StaticTemperature
saturated float16 static_temperature
saturated float16 static_temperature_variance
******************************************************************************/

#define UAVCAN_EQUIPMENT_AIR_DATA_STATICTEMPERATURE_ID     1029
#define UAVCAN_EQUIPMENT_AIR_DATA_STATICTEMPERATURE_NAME   "uavcan.equipment.air_data.StaticTemperature"
#define UAVCAN_EQUIPMENT_AIR_DATA_STATICTEMPERATURE_SIGNATURE (0x49272A6477D96271ULL)

#define UAVCAN_EQUIPMENT_AIR_DATA_STATICTEMPERATURE_MAX_SIZE ((32 + 7)/8)

// Constants

typedef struct
{
    // FieldTypes
    float      static_temperature;            // float16 Saturate
    float      static_temperature_variance;   // float16 Saturate

} uavcan_equipment_air_data_StaticTemperature;

extern
uint32_t uavcan_equipment_air_data_StaticTemperature_encode(uavcan_equipment_air_data_StaticTemperature* source, void* msg_buf);

extern
int32_t uavcan_equipment_air_data_StaticTemperature_decode(const CanardRxTransfer* transfer, uint16_t payload_len, uavcan_equipment_air_data_StaticTemperature* dest, uint8_t** dyn_arr_buf);

extern
uint32_t uavcan_equipment_air_data_StaticTemperature_encode_internal(uavcan_equipment_air_data_StaticTemperature* source, void* msg_buf, uint32_t offset, uint8_t root_item);

extern
int32_t uavcan_equipment_air_data_StaticTemperature_decode_internal(const CanardRxTransfer* transfer, uint16_t payload_len, uavcan_equipment_air_data_StaticTemperature* dest, uint8_t** dyn_arr_buf, int32_t offset);

#ifdef __cplusplus
} // extern "C"
#endif
#endif // __UAVCAN_EQUIPMENT_AIR_DATA_STATICTEMPERATURE