/*
 * UAVCAN data structure definition for libcanard.
 *
 * Autogenerated, do not edit.
 *
 * Source file: /home/isthatme/Documents/Clubs/ARVP/sonar_firmware/dsdl/uavcan/equipment/safety/1100.ArmingStatus.uavcan
 */

#ifndef __UAVCAN_EQUIPMENT_SAFETY_ARMINGSTATUS
#define __UAVCAN_EQUIPMENT_SAFETY_ARMINGSTATUS

#include <stdint.h>
#include "canard.h"

#ifdef __cplusplus
extern "C"
{
#endif

/******************************* Source text **********************************
#
# This message represents the system arming status.
# Some nodes may refuse to operate unless the system is fully armed.
#

uint8 STATUS_DISARMED           = 0
uint8 STATUS_FULLY_ARMED        = 255

uint8 status
******************************************************************************/

/********************* DSDL signature source definition ***********************
uavcan.equipment.safety.ArmingStatus
saturated uint8 status
******************************************************************************/

#define UAVCAN_EQUIPMENT_SAFETY_ARMINGSTATUS_ID            1100
#define UAVCAN_EQUIPMENT_SAFETY_ARMINGSTATUS_NAME          "uavcan.equipment.safety.ArmingStatus"
#define UAVCAN_EQUIPMENT_SAFETY_ARMINGSTATUS_SIGNATURE     (0x8700F375556A8003ULL)

#define UAVCAN_EQUIPMENT_SAFETY_ARMINGSTATUS_MAX_SIZE      ((8 + 7)/8)

// Constants
#define UAVCAN_EQUIPMENT_SAFETY_ARMINGSTATUS_STATUS_DISARMED                  0 // 0
#define UAVCAN_EQUIPMENT_SAFETY_ARMINGSTATUS_STATUS_FULLY_ARMED             255 // 255

typedef struct
{
    // FieldTypes
    uint8_t    status;                        // bit len 8

} uavcan_equipment_safety_ArmingStatus;

extern
uint32_t uavcan_equipment_safety_ArmingStatus_encode(uavcan_equipment_safety_ArmingStatus* source, void* msg_buf);

extern
int32_t uavcan_equipment_safety_ArmingStatus_decode(const CanardRxTransfer* transfer, uint16_t payload_len, uavcan_equipment_safety_ArmingStatus* dest, uint8_t** dyn_arr_buf);

extern
uint32_t uavcan_equipment_safety_ArmingStatus_encode_internal(uavcan_equipment_safety_ArmingStatus* source, void* msg_buf, uint32_t offset, uint8_t root_item);

extern
int32_t uavcan_equipment_safety_ArmingStatus_decode_internal(const CanardRxTransfer* transfer, uint16_t payload_len, uavcan_equipment_safety_ArmingStatus* dest, uint8_t** dyn_arr_buf, int32_t offset);

#ifdef __cplusplus
} // extern "C"
#endif
#endif // __UAVCAN_EQUIPMENT_SAFETY_ARMINGSTATUS