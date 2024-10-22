/*
 * UAVCAN data structure definition for libcanard.
 *
 * Autogenerated, do not edit.
 *
 * Source file: /home/isthatme/Documents/Clubs/ARVP/sonar_firmware/dsdl/uavcan/equipment/power/1092.BatteryInfo.uavcan
 */
#include "uavcan/equipment/power/BatteryInfo.h"
#include "canard.h"

#ifndef CANARD_INTERNAL_SATURATE
#define CANARD_INTERNAL_SATURATE(x, max) ( ((x) > max) ? max : ( (-(x) > max) ? (-max) : (x) ) );
#endif

#ifndef CANARD_INTERNAL_SATURATE_UNSIGNED
#define CANARD_INTERNAL_SATURATE_UNSIGNED(x, max) ( ((x) >= max) ? max : (x) );
#endif

#if defined(__GNUC__)
# define CANARD_MAYBE_UNUSED(x) x __attribute__((unused))
#else
# define CANARD_MAYBE_UNUSED(x) x
#endif

/**
  * @brief uavcan_equipment_power_BatteryInfo_encode_internal
  * @param source : pointer to source data struct
  * @param msg_buf: pointer to msg storage
  * @param offset: bit offset to msg storage
  * @param root_item: for detecting if TAO should be used
  * @retval returns offset
  */
uint32_t uavcan_equipment_power_BatteryInfo_encode_internal(uavcan_equipment_power_BatteryInfo* source,
  void* msg_buf,
  uint32_t offset,
  uint8_t CANARD_MAYBE_UNUSED(root_item))
{
    uint32_t c = 0;
#ifndef CANARD_USE_FLOAT16_CAST
    uint16_t tmp_float = 0;
#else
    CANARD_USE_FLOAT16_CAST tmp_float = 0;
#endif

    // float16 special handling
#ifndef CANARD_USE_FLOAT16_CAST
    tmp_float = canardConvertNativeFloatToFloat16(source->temperature);
#else
    tmp_float = (CANARD_USE_FLOAT16_CAST)source->temperature;
#endif
    canardEncodeScalar(msg_buf, offset, 16, (void*)&tmp_float); // 32767
    offset += 16;

    // float16 special handling
#ifndef CANARD_USE_FLOAT16_CAST
    tmp_float = canardConvertNativeFloatToFloat16(source->voltage);
#else
    tmp_float = (CANARD_USE_FLOAT16_CAST)source->voltage;
#endif
    canardEncodeScalar(msg_buf, offset, 16, (void*)&tmp_float); // 32767
    offset += 16;

    // float16 special handling
#ifndef CANARD_USE_FLOAT16_CAST
    tmp_float = canardConvertNativeFloatToFloat16(source->current);
#else
    tmp_float = (CANARD_USE_FLOAT16_CAST)source->current;
#endif
    canardEncodeScalar(msg_buf, offset, 16, (void*)&tmp_float); // 32767
    offset += 16;

    // float16 special handling
#ifndef CANARD_USE_FLOAT16_CAST
    tmp_float = canardConvertNativeFloatToFloat16(source->average_power_10sec);
#else
    tmp_float = (CANARD_USE_FLOAT16_CAST)source->average_power_10sec;
#endif
    canardEncodeScalar(msg_buf, offset, 16, (void*)&tmp_float); // 32767
    offset += 16;

    // float16 special handling
#ifndef CANARD_USE_FLOAT16_CAST
    tmp_float = canardConvertNativeFloatToFloat16(source->remaining_capacity_wh);
#else
    tmp_float = (CANARD_USE_FLOAT16_CAST)source->remaining_capacity_wh;
#endif
    canardEncodeScalar(msg_buf, offset, 16, (void*)&tmp_float); // 32767
    offset += 16;

    // float16 special handling
#ifndef CANARD_USE_FLOAT16_CAST
    tmp_float = canardConvertNativeFloatToFloat16(source->full_charge_capacity_wh);
#else
    tmp_float = (CANARD_USE_FLOAT16_CAST)source->full_charge_capacity_wh;
#endif
    canardEncodeScalar(msg_buf, offset, 16, (void*)&tmp_float); // 32767
    offset += 16;

    // float16 special handling
#ifndef CANARD_USE_FLOAT16_CAST
    tmp_float = canardConvertNativeFloatToFloat16(source->hours_to_full_charge);
#else
    tmp_float = (CANARD_USE_FLOAT16_CAST)source->hours_to_full_charge;
#endif
    canardEncodeScalar(msg_buf, offset, 16, (void*)&tmp_float); // 32767
    offset += 16;
    source->status_flags = CANARD_INTERNAL_SATURATE_UNSIGNED(source->status_flags, 2047)
    canardEncodeScalar(msg_buf, offset, 11, (void*)&source->status_flags); // 2047
    offset += 11;

    source->state_of_health_pct = CANARD_INTERNAL_SATURATE_UNSIGNED(source->state_of_health_pct, 127)
    canardEncodeScalar(msg_buf, offset, 7, (void*)&source->state_of_health_pct); // 127
    offset += 7;

    source->state_of_charge_pct = CANARD_INTERNAL_SATURATE_UNSIGNED(source->state_of_charge_pct, 127)
    canardEncodeScalar(msg_buf, offset, 7, (void*)&source->state_of_charge_pct); // 127
    offset += 7;

    source->state_of_charge_pct_stdev = CANARD_INTERNAL_SATURATE_UNSIGNED(source->state_of_charge_pct_stdev, 127)
    canardEncodeScalar(msg_buf, offset, 7, (void*)&source->state_of_charge_pct_stdev); // 127
    offset += 7;

    canardEncodeScalar(msg_buf, offset, 8, (void*)&source->battery_id); // 255
    offset += 8;

    canardEncodeScalar(msg_buf, offset, 32, (void*)&source->model_instance_id); // 4294967295
    offset += 32;

    // Dynamic Array (model_name)
    if (! root_item)
    {
        // - Add array length
        canardEncodeScalar(msg_buf, offset, 5, (void*)&source->model_name.len);
        offset += 5;
    }

    // - Add array items
    for (c = 0; c < source->model_name.len; c++)
    {
        canardEncodeScalar(msg_buf,
                           offset,
                           8,
                           (void*)(source->model_name.data + c));// 255
        offset += 8;
    }

    return offset;
}

/**
  * @brief uavcan_equipment_power_BatteryInfo_encode
  * @param source : Pointer to source data struct
  * @param msg_buf: Pointer to msg storage
  * @retval returns message length as bytes
  */
uint32_t uavcan_equipment_power_BatteryInfo_encode(uavcan_equipment_power_BatteryInfo* source, void* msg_buf)
{
    uint32_t offset = 0;

    offset = uavcan_equipment_power_BatteryInfo_encode_internal(source, msg_buf, offset, 1);

    return (offset + 7 ) / 8;
}

/**
  * @brief uavcan_equipment_power_BatteryInfo_decode_internal
  * @param transfer: Pointer to CanardRxTransfer transfer
  * @param payload_len: Payload message length
  * @param dest: Pointer to destination struct
  * @param dyn_arr_buf: NULL or Pointer to memory storage to be used for dynamic arrays
  *                     uavcan_equipment_power_BatteryInfo dyn memory will point to dyn_arr_buf memory.
  *                     NULL will ignore dynamic arrays decoding.
  * @param offset: Call with 0, bit offset to msg storage
  * @retval offset or ERROR value if < 0
  */
int32_t uavcan_equipment_power_BatteryInfo_decode_internal(
  const CanardRxTransfer* transfer,
  uint16_t CANARD_MAYBE_UNUSED(payload_len),
  uavcan_equipment_power_BatteryInfo* dest,
  uint8_t** CANARD_MAYBE_UNUSED(dyn_arr_buf),
  int32_t offset)
{
    int32_t ret = 0;
    uint32_t c = 0;
#ifndef CANARD_USE_FLOAT16_CAST
    uint16_t tmp_float = 0;
#else
    CANARD_USE_FLOAT16_CAST tmp_float = 0;
#endif

    // float16 special handling
    ret = canardDecodeScalar(transfer, offset, 16, false, (void*)&tmp_float);

    if (ret != 16)
    {
        goto uavcan_equipment_power_BatteryInfo_error_exit;
    }
#ifndef CANARD_USE_FLOAT16_CAST
    dest->temperature = canardConvertFloat16ToNativeFloat(tmp_float);
#else
    dest->temperature = (float)tmp_float;
#endif
    offset += 16;

    // float16 special handling
    ret = canardDecodeScalar(transfer, offset, 16, false, (void*)&tmp_float);

    if (ret != 16)
    {
        goto uavcan_equipment_power_BatteryInfo_error_exit;
    }
#ifndef CANARD_USE_FLOAT16_CAST
    dest->voltage = canardConvertFloat16ToNativeFloat(tmp_float);
#else
    dest->voltage = (float)tmp_float;
#endif
    offset += 16;

    // float16 special handling
    ret = canardDecodeScalar(transfer, offset, 16, false, (void*)&tmp_float);

    if (ret != 16)
    {
        goto uavcan_equipment_power_BatteryInfo_error_exit;
    }
#ifndef CANARD_USE_FLOAT16_CAST
    dest->current = canardConvertFloat16ToNativeFloat(tmp_float);
#else
    dest->current = (float)tmp_float;
#endif
    offset += 16;

    // float16 special handling
    ret = canardDecodeScalar(transfer, offset, 16, false, (void*)&tmp_float);

    if (ret != 16)
    {
        goto uavcan_equipment_power_BatteryInfo_error_exit;
    }
#ifndef CANARD_USE_FLOAT16_CAST
    dest->average_power_10sec = canardConvertFloat16ToNativeFloat(tmp_float);
#else
    dest->average_power_10sec = (float)tmp_float;
#endif
    offset += 16;

    // float16 special handling
    ret = canardDecodeScalar(transfer, offset, 16, false, (void*)&tmp_float);

    if (ret != 16)
    {
        goto uavcan_equipment_power_BatteryInfo_error_exit;
    }
#ifndef CANARD_USE_FLOAT16_CAST
    dest->remaining_capacity_wh = canardConvertFloat16ToNativeFloat(tmp_float);
#else
    dest->remaining_capacity_wh = (float)tmp_float;
#endif
    offset += 16;

    // float16 special handling
    ret = canardDecodeScalar(transfer, offset, 16, false, (void*)&tmp_float);

    if (ret != 16)
    {
        goto uavcan_equipment_power_BatteryInfo_error_exit;
    }
#ifndef CANARD_USE_FLOAT16_CAST
    dest->full_charge_capacity_wh = canardConvertFloat16ToNativeFloat(tmp_float);
#else
    dest->full_charge_capacity_wh = (float)tmp_float;
#endif
    offset += 16;

    // float16 special handling
    ret = canardDecodeScalar(transfer, offset, 16, false, (void*)&tmp_float);

    if (ret != 16)
    {
        goto uavcan_equipment_power_BatteryInfo_error_exit;
    }
#ifndef CANARD_USE_FLOAT16_CAST
    dest->hours_to_full_charge = canardConvertFloat16ToNativeFloat(tmp_float);
#else
    dest->hours_to_full_charge = (float)tmp_float;
#endif
    offset += 16;

    ret = canardDecodeScalar(transfer, offset, 11, false, (void*)&dest->status_flags);
    if (ret != 11)
    {
        goto uavcan_equipment_power_BatteryInfo_error_exit;
    }
    offset += 11;

    ret = canardDecodeScalar(transfer, offset, 7, false, (void*)&dest->state_of_health_pct);
    if (ret != 7)
    {
        goto uavcan_equipment_power_BatteryInfo_error_exit;
    }
    offset += 7;

    ret = canardDecodeScalar(transfer, offset, 7, false, (void*)&dest->state_of_charge_pct);
    if (ret != 7)
    {
        goto uavcan_equipment_power_BatteryInfo_error_exit;
    }
    offset += 7;

    ret = canardDecodeScalar(transfer, offset, 7, false, (void*)&dest->state_of_charge_pct_stdev);
    if (ret != 7)
    {
        goto uavcan_equipment_power_BatteryInfo_error_exit;
    }
    offset += 7;

    ret = canardDecodeScalar(transfer, offset, 8, false, (void*)&dest->battery_id);
    if (ret != 8)
    {
        goto uavcan_equipment_power_BatteryInfo_error_exit;
    }
    offset += 8;

    ret = canardDecodeScalar(transfer, offset, 32, false, (void*)&dest->model_instance_id);
    if (ret != 32)
    {
        goto uavcan_equipment_power_BatteryInfo_error_exit;
    }
    offset += 32;

    // Dynamic Array (model_name)
    //  - Last item in struct & Root item & (Array Size > 8 bit), tail array optimization
    if (payload_len)
    {
        //  - Calculate Array length from MSG length
        dest->model_name.len = ((payload_len * 8) - offset ) / 8; // 8 bit array item size
    }
    else
    {
        // - Array length 5 bits
        ret = canardDecodeScalar(transfer,
                                 offset,
                                 5,
                                 false,
                                 (void*)&dest->model_name.len); // 255
        if (ret != 5)
        {
            goto uavcan_equipment_power_BatteryInfo_error_exit;
        }
        offset += 5;
    }

    //  - Get Array
    if (dyn_arr_buf)
    {
        dest->model_name.data = (uint8_t*)*dyn_arr_buf;
    }

    for (c = 0; c < dest->model_name.len; c++)
    {
        if (dyn_arr_buf)
        {
            ret = canardDecodeScalar(transfer,
                                     offset,
                                     8,
                                     false,
                                     (void*)*dyn_arr_buf); // 255
            if (ret != 8)
            {
                goto uavcan_equipment_power_BatteryInfo_error_exit;
            }
            *dyn_arr_buf = (uint8_t*)(((uint8_t*)*dyn_arr_buf) + 1);
        }
        offset += 8;
    }
    return offset;

uavcan_equipment_power_BatteryInfo_error_exit:
    if (ret < 0)
    {
        return ret;
    }
    else
    {
        return -CANARD_ERROR_INTERNAL;
    }
}

/**
  * @brief uavcan_equipment_power_BatteryInfo_decode
  * @param transfer: Pointer to CanardRxTransfer transfer
  * @param payload_len: Payload message length
  * @param dest: Pointer to destination struct
  * @param dyn_arr_buf: NULL or Pointer to memory storage to be used for dynamic arrays
  *                     uavcan_equipment_power_BatteryInfo dyn memory will point to dyn_arr_buf memory.
  *                     NULL will ignore dynamic arrays decoding.
  * @retval offset or ERROR value if < 0
  */
int32_t uavcan_equipment_power_BatteryInfo_decode(const CanardRxTransfer* transfer,
  uint16_t payload_len,
  uavcan_equipment_power_BatteryInfo* dest,
  uint8_t** dyn_arr_buf)
{
    const int32_t offset = 0;
    int32_t ret = 0;

    // Clear the destination struct
    for (uint32_t c = 0; c < sizeof(uavcan_equipment_power_BatteryInfo); c++)
    {
        ((uint8_t*)dest)[c] = 0x00;
    }

    ret = uavcan_equipment_power_BatteryInfo_decode_internal(transfer, payload_len, dest, dyn_arr_buf, offset);

    return ret;
}
