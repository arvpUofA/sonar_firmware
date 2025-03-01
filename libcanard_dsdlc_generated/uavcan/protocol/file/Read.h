/*
 * UAVCAN data structure definition for libcanard.
 *
 * Autogenerated, do not edit.
 *
 * Source file: /home/isthatme/Documents/Clubs/ARVP/sonar_firmware/dsdl/uavcan/protocol/file/48.Read.uavcan
 */

#ifndef __UAVCAN_PROTOCOL_FILE_READ
#define __UAVCAN_PROTOCOL_FILE_READ

#include <stdint.h>
#include "canard.h"

#ifdef __cplusplus
extern "C"
{
#endif

#include <uavcan/protocol/file/Error.h>
#include <uavcan/protocol/file/Path.h>

/******************************* Source text **********************************
#
# Read file from a remote node.
#
# There are two possible outcomes of a successful service call:
#  1. Data array size equals its capacity. This means that the end of the file is not reached yet.
#  2. Data array size is less than its capacity, possibly zero. This means that the end of file is reached.
#
# Thus, if the client needs to fetch the entire file, it should repeatedly call this service while increasing the
# offset, until incomplete data is returned.
#
# If the object pointed by 'path' cannot be read (e.g. it is a directory or it does not exist), appropriate error code
# will be returned, and data array will be empty.
#

uint40 offset

Path path

---

Error error

uint8[<=256] data
******************************************************************************/

/********************* DSDL signature source definition ***********************
uavcan.protocol.file.Read
saturated uint40 offset
uavcan.protocol.file.Path path
---
uavcan.protocol.file.Error error
saturated uint8[<=256] data
******************************************************************************/

#define UAVCAN_PROTOCOL_FILE_READ_ID                       48
#define UAVCAN_PROTOCOL_FILE_READ_NAME                     "uavcan.protocol.file.Read"
#define UAVCAN_PROTOCOL_FILE_READ_SIGNATURE                (0x8DCDCA939F33F678ULL)

#define UAVCAN_PROTOCOL_FILE_READ_REQUEST_MAX_SIZE         ((1648 + 7)/8)

// Constants

typedef struct
{
    // FieldTypes
    uint64_t   offset;                        // bit len 40
    uavcan_protocol_file_Path path;                          //

} uavcan_protocol_file_ReadRequest;

extern
uint32_t uavcan_protocol_file_ReadRequest_encode(uavcan_protocol_file_ReadRequest* source, void* msg_buf);

extern
int32_t uavcan_protocol_file_ReadRequest_decode(const CanardRxTransfer* transfer, uint16_t payload_len, uavcan_protocol_file_ReadRequest* dest, uint8_t** dyn_arr_buf);

extern
uint32_t uavcan_protocol_file_ReadRequest_encode_internal(uavcan_protocol_file_ReadRequest* source, void* msg_buf, uint32_t offset, uint8_t root_item);

extern
int32_t uavcan_protocol_file_ReadRequest_decode_internal(const CanardRxTransfer* transfer, uint16_t payload_len, uavcan_protocol_file_ReadRequest* dest, uint8_t** dyn_arr_buf, int32_t offset);

#define UAVCAN_PROTOCOL_FILE_READ_RESPONSE_MAX_SIZE        ((2073 + 7)/8)

// Constants

#define UAVCAN_PROTOCOL_FILE_READ_RESPONSE_DATA_MAX_LENGTH                               256

typedef struct
{
    // FieldTypes
    uavcan_protocol_file_Error error;                         //
    struct
    {
        uint16_t    len;                       // Dynamic array length
        uint8_t*   data;                      // Dynamic Array 8bit[256] max items
    } data;

} uavcan_protocol_file_ReadResponse;

extern
uint32_t uavcan_protocol_file_ReadResponse_encode(uavcan_protocol_file_ReadResponse* source, void* msg_buf);

extern
int32_t uavcan_protocol_file_ReadResponse_decode(const CanardRxTransfer* transfer, uint16_t payload_len, uavcan_protocol_file_ReadResponse* dest, uint8_t** dyn_arr_buf);

extern
uint32_t uavcan_protocol_file_ReadResponse_encode_internal(uavcan_protocol_file_ReadResponse* source, void* msg_buf, uint32_t offset, uint8_t root_item);

extern
int32_t uavcan_protocol_file_ReadResponse_decode_internal(const CanardRxTransfer* transfer, uint16_t payload_len, uavcan_protocol_file_ReadResponse* dest, uint8_t** dyn_arr_buf, int32_t offset);

#ifdef __cplusplus
} // extern "C"
#endif
#endif // __UAVCAN_PROTOCOL_FILE_READ