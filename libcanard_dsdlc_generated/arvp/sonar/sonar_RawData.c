/*
 * UAVCAN data structure definition for libcanard.
 *
 * Autogenerated, do not edit.
 *
 * Source file: /home/isthatme/Documents/Clubs/ARVP/sonar_firmware/custom_dsdl/arvp/sonar/RawData.uavcan
 */
#include "arvp/sonar/RawData.h"
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
  * @brief arvp_sonar_RawData_encode_internal
  * @param source : pointer to source data struct
  * @param msg_buf: pointer to msg storage
  * @param offset: bit offset to msg storage
  * @param root_item: for detecting if TAO should be used
  * @retval returns offset
  */
uint32_t arvp_sonar_RawData_encode_internal(arvp_sonar_RawData* source,
  void* msg_buf,
  uint32_t offset,
  uint8_t CANARD_MAYBE_UNUSED(root_item))
{
    uint32_t c = 0;

    canardEncodeScalar(msg_buf, offset, 8, (void*)&source->hydrophone_id); // 255
    offset += 8;

    canardEncodeScalar(msg_buf, offset, 8, (void*)&source->gain); // 255
    offset += 8;

    // Dynamic Array (data)
    if (! root_item)
    {
        // - Add array length
        canardEncodeScalar(msg_buf, offset, 12, (void*)&source->data.len);
        offset += 12;
    }

    // - Add array items
    for (c = 0; c < source->data.len; c++)
    {
        canardEncodeScalar(msg_buf,
                           offset,
                           16,
                           (void*)(source->data.data + c));// 65535
        offset += 16;
    }

    return offset;
}

/**
  * @brief arvp_sonar_RawData_encode
  * @param source : Pointer to source data struct
  * @param msg_buf: Pointer to msg storage
  * @retval returns message length as bytes
  */
uint32_t arvp_sonar_RawData_encode(arvp_sonar_RawData* source, void* msg_buf)
{
    uint32_t offset = 0;

    offset = arvp_sonar_RawData_encode_internal(source, msg_buf, offset, 1);

    return (offset + 7 ) / 8;
}

/**
  * @brief arvp_sonar_RawData_decode_internal
  * @param transfer: Pointer to CanardRxTransfer transfer
  * @param payload_len: Payload message length
  * @param dest: Pointer to destination struct
  * @param dyn_arr_buf: NULL or Pointer to memory storage to be used for dynamic arrays
  *                     arvp_sonar_RawData dyn memory will point to dyn_arr_buf memory.
  *                     NULL will ignore dynamic arrays decoding.
  * @param offset: Call with 0, bit offset to msg storage
  * @retval offset or ERROR value if < 0
  */
int32_t arvp_sonar_RawData_decode_internal(
  const CanardRxTransfer* transfer,
  uint16_t CANARD_MAYBE_UNUSED(payload_len),
  arvp_sonar_RawData* dest,
  uint8_t** CANARD_MAYBE_UNUSED(dyn_arr_buf),
  int32_t offset)
{
    int32_t ret = 0;
    uint32_t c = 0;

    ret = canardDecodeScalar(transfer, offset, 8, false, (void*)&dest->hydrophone_id);
    if (ret != 8)
    {
        goto arvp_sonar_RawData_error_exit;
    }
    offset += 8;

    ret = canardDecodeScalar(transfer, offset, 8, false, (void*)&dest->gain);
    if (ret != 8)
    {
        goto arvp_sonar_RawData_error_exit;
    }
    offset += 8;

    // Dynamic Array (data)
    //  - Last item in struct & Root item & (Array Size > 8 bit), tail array optimization
    if (payload_len)
    {
        //  - Calculate Array length from MSG length
        dest->data.len = ((payload_len * 8) - offset ) / 16; // 16 bit array item size
    }
    else
    {
        // - Array length 12 bits
        ret = canardDecodeScalar(transfer,
                                 offset,
                                 12,
                                 false,
                                 (void*)&dest->data.len); // 65535
        if (ret != 12)
        {
            goto arvp_sonar_RawData_error_exit;
        }
        offset += 12;
    }

    //  - Get Array
    if (dyn_arr_buf)
    {
        dest->data.data = (uint16_t*)*dyn_arr_buf;
    }

    for (c = 0; c < dest->data.len; c++)
    {
        if (dyn_arr_buf)
        {
            ret = canardDecodeScalar(transfer,
                                     offset,
                                     16,
                                     false,
                                     (void*)*dyn_arr_buf); // 65535
            if (ret != 16)
            {
                goto arvp_sonar_RawData_error_exit;
            }
            *dyn_arr_buf = (uint8_t*)(((uint16_t*)*dyn_arr_buf) + 1);
        }
        offset += 16;
    }
    return offset;

arvp_sonar_RawData_error_exit:
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
  * @brief arvp_sonar_RawData_decode
  * @param transfer: Pointer to CanardRxTransfer transfer
  * @param payload_len: Payload message length
  * @param dest: Pointer to destination struct
  * @param dyn_arr_buf: NULL or Pointer to memory storage to be used for dynamic arrays
  *                     arvp_sonar_RawData dyn memory will point to dyn_arr_buf memory.
  *                     NULL will ignore dynamic arrays decoding.
  * @retval offset or ERROR value if < 0
  */
int32_t arvp_sonar_RawData_decode(const CanardRxTransfer* transfer,
  uint16_t payload_len,
  arvp_sonar_RawData* dest,
  uint8_t** dyn_arr_buf)
{
    const int32_t offset = 0;
    int32_t ret = 0;

    // Clear the destination struct
    for (uint32_t c = 0; c < sizeof(arvp_sonar_RawData); c++)
    {
        ((uint8_t*)dest)[c] = 0x00;
    }

    ret = arvp_sonar_RawData_decode_internal(transfer, payload_len, dest, dyn_arr_buf, offset);

    return ret;
}