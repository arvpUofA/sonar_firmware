/*
 * UAVCAN data structure definition for libcanard.
 *
 * Autogenerated, do not edit.
 *
 * Source file: /home/isthatme/Documents/Clubs/ARVP/sonar_firmware/dsdl/uavcan/protocol/param/11.GetSet.uavcan
 */

#ifndef __UAVCAN_PROTOCOL_PARAM_GETSET
#define __UAVCAN_PROTOCOL_PARAM_GETSET

#include <stdint.h>
#include "canard.h"

#ifdef __cplusplus
extern "C"
{
#endif

#include <uavcan/protocol/param/NumericValue.h>
#include <uavcan/protocol/param/Value.h>

/******************************* Source text **********************************
#
# Get or set a parameter by name or by index.
# Note that access by index should only be used to retrieve the list of parameters; it is highly
# discouraged to use it for anything else, because persistent ordering is not guaranteed.
#

#
# Index of the parameter starting from 0; ignored if name is nonempty.
# Use index only to retrieve the list of parameters.
# Parameter ordering must be well defined (e.g. alphabetical, or any other stable ordering),
# in order for the index access to work.
#
uint13 index

#
# If set - parameter will be assigned this value, then the new value will be returned.
# If not set - current parameter value will be returned.
# Refer to the definition of Value for details.
#
Value value

#
# Name of the parameter; always preferred over index if nonempty.
#
uint8[<=92] name

---

void5

#
# Actual parameter value.
#
# For set requests, it should contain the actual parameter value after the set request was
# executed. The objective is to let the client know if the value could not be updated, e.g.
# due to its range violation, etc.
#
# Empty value (and/or empty name) indicates that there is no such parameter.
#
Value value

void5
Value default_value    # Optional

void6
NumericValue max_value # Optional, not applicable for bool/string

void6
NumericValue min_value # Optional, not applicable for bool/string

#
# Empty name (and/or empty value) in response indicates that there is no such parameter.
#
uint8[<=92] name
******************************************************************************/

/********************* DSDL signature source definition ***********************
uavcan.protocol.param.GetSet
saturated uint13 index
uavcan.protocol.param.Value value
saturated uint8[<=92] name
---
void5
uavcan.protocol.param.Value value
void5
uavcan.protocol.param.Value default_value
void6
uavcan.protocol.param.NumericValue max_value
void6
uavcan.protocol.param.NumericValue min_value
saturated uint8[<=92] name
******************************************************************************/

#define UAVCAN_PROTOCOL_PARAM_GETSET_ID                    11
#define UAVCAN_PROTOCOL_PARAM_GETSET_NAME                  "uavcan.protocol.param.GetSet"
#define UAVCAN_PROTOCOL_PARAM_GETSET_SIGNATURE             (0xA7B622F939D1A4D5ULL)

#define UAVCAN_PROTOCOL_PARAM_GETSET_REQUEST_MAX_SIZE      ((1791 + 7)/8)

// Constants

#define UAVCAN_PROTOCOL_PARAM_GETSET_REQUEST_NAME_MAX_LENGTH                             92

typedef struct
{
    // FieldTypes
    uint16_t   index;                         // bit len 13
    uavcan_protocol_param_Value value;                         //
    struct
    {
        uint8_t    len;                       // Dynamic array length
        uint8_t*   data;                      // Dynamic Array 8bit[92] max items
    } name;

} uavcan_protocol_param_GetSetRequest;

extern
uint32_t uavcan_protocol_param_GetSetRequest_encode(uavcan_protocol_param_GetSetRequest* source, void* msg_buf);

extern
int32_t uavcan_protocol_param_GetSetRequest_decode(const CanardRxTransfer* transfer, uint16_t payload_len, uavcan_protocol_param_GetSetRequest* dest, uint8_t** dyn_arr_buf);

extern
uint32_t uavcan_protocol_param_GetSetRequest_encode_internal(uavcan_protocol_param_GetSetRequest* source, void* msg_buf, uint32_t offset, uint8_t root_item);

extern
int32_t uavcan_protocol_param_GetSetRequest_decode_internal(const CanardRxTransfer* transfer, uint16_t payload_len, uavcan_protocol_param_GetSetRequest* dest, uint8_t** dyn_arr_buf, int32_t offset);

#define UAVCAN_PROTOCOL_PARAM_GETSET_RESPONSE_MAX_SIZE     ((2967 + 7)/8)

// Constants

#define UAVCAN_PROTOCOL_PARAM_GETSET_RESPONSE_NAME_MAX_LENGTH                            92

typedef struct
{
    // FieldTypes
    // void5
    uavcan_protocol_param_Value value;                         //
    // void5
    uavcan_protocol_param_Value default_value;                 //
    // void6
    uavcan_protocol_param_NumericValue max_value;                     //
    // void6
    uavcan_protocol_param_NumericValue min_value;                     //
    struct
    {
        uint8_t    len;                       // Dynamic array length
        uint8_t*   data;                      // Dynamic Array 8bit[92] max items
    } name;

} uavcan_protocol_param_GetSetResponse;

extern
uint32_t uavcan_protocol_param_GetSetResponse_encode(uavcan_protocol_param_GetSetResponse* source, void* msg_buf);

extern
int32_t uavcan_protocol_param_GetSetResponse_decode(const CanardRxTransfer* transfer, uint16_t payload_len, uavcan_protocol_param_GetSetResponse* dest, uint8_t** dyn_arr_buf);

extern
uint32_t uavcan_protocol_param_GetSetResponse_encode_internal(uavcan_protocol_param_GetSetResponse* source, void* msg_buf, uint32_t offset, uint8_t root_item);

extern
int32_t uavcan_protocol_param_GetSetResponse_decode_internal(const CanardRxTransfer* transfer, uint16_t payload_len, uavcan_protocol_param_GetSetResponse* dest, uint8_t** dyn_arr_buf, int32_t offset);

#ifdef __cplusplus
} // extern "C"
#endif
#endif // __UAVCAN_PROTOCOL_PARAM_GETSET