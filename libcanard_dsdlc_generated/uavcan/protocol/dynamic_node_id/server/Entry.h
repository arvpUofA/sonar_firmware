/*
 * UAVCAN data structure definition for libcanard.
 *
 * Autogenerated, do not edit.
 *
 * Source file: /home/isthatme/Documents/Clubs/ARVP/sonar_firmware/dsdl/uavcan/protocol/dynamic_node_id/server/Entry.uavcan
 */

#ifndef __UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_SERVER_ENTRY
#define __UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_SERVER_ENTRY

#include <stdint.h>
#include "canard.h"

#ifdef __cplusplus
extern "C"
{
#endif

/******************************* Source text **********************************
#
# THIS DEFINITION IS SUBJECT TO CHANGE.
#
# One dynamic node ID allocation entry.
# This type is a part of the Raft consensus algorithm.
# Please refer to the specification for details.
#

uint32 term             # Refer to the Raft paper for explanation.

uint8[16] unique_id     # Unique ID of this allocation.

void1
uint7 node_id           # Node ID of this allocation.
******************************************************************************/

/********************* DSDL signature source definition ***********************
uavcan.protocol.dynamic_node_id.server.Entry
saturated uint32 term
saturated uint8[16] unique_id
void1
saturated uint7 node_id
******************************************************************************/

#define UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_SERVER_ENTRY_NAME  "uavcan.protocol.dynamic_node_id.server.Entry"
#define UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_SERVER_ENTRY_SIGNATURE (0x7FAA779D64FA75C2ULL)

#define UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_SERVER_ENTRY_MAX_SIZE ((168 + 7)/8)

// Constants

#define UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_SERVER_ENTRY_UNIQUE_ID_LENGTH                    16

typedef struct
{
    // FieldTypes
    uint32_t   term;                          // bit len 32
    uint8_t    unique_id[16];                 // Static Array 8bit[16] max items
    // void1
    uint8_t    node_id;                       // bit len 7

} uavcan_protocol_dynamic_node_id_server_Entry;

extern
uint32_t uavcan_protocol_dynamic_node_id_server_Entry_encode(uavcan_protocol_dynamic_node_id_server_Entry* source, void* msg_buf);

extern
int32_t uavcan_protocol_dynamic_node_id_server_Entry_decode(const CanardRxTransfer* transfer, uint16_t payload_len, uavcan_protocol_dynamic_node_id_server_Entry* dest, uint8_t** dyn_arr_buf);

extern
uint32_t uavcan_protocol_dynamic_node_id_server_Entry_encode_internal(uavcan_protocol_dynamic_node_id_server_Entry* source, void* msg_buf, uint32_t offset, uint8_t root_item);

extern
int32_t uavcan_protocol_dynamic_node_id_server_Entry_decode_internal(const CanardRxTransfer* transfer, uint16_t payload_len, uavcan_protocol_dynamic_node_id_server_Entry* dest, uint8_t** dyn_arr_buf, int32_t offset);

#ifdef __cplusplus
} // extern "C"
#endif
#endif // __UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_SERVER_ENTRY