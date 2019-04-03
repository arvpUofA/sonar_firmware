#include "can.h"


static CanardInstance canard;
static uavcan_protocol_NodeStatus node_status;
static uint8_t inout_transfer_id = 0;

// Max size of sonar transfer is ~4K, not sure how much memory overhead they have
// libcanard memory pool for storing TX queue.
#define CANARD_MEM_POOL_SIZE    5000
static uint8_t canard_mem_pool[CANARD_MEM_POOL_SIZE];
static uint8_t canard_message_buffer[ARVP_SONAR_RAWDATA_MAX_SIZE];

static CanardSTM32CANTimings canard_timings;

static void can_gpio_init() {
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef gpio;

    // TODO match these to hardware
    gpio.Pin = GPIO_PIN_12 | GPIO_PIN_13;
    gpio.Alternate = GPIO_AF7_CAN;
    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;

    HAL_GPIO_Init(GPIOA, &gpio);
}

static void on_reception(CanardInstance* ins,
                         CanardRxTransfer* transfer);

static bool should_accept(const CanardInstance* ins,
                          uint64_t* out_data_type_signature,
                          uint16_t data_type_id,
                          CanardTransferType transfer_type,
                          uint8_t source_node_id);

void can_init(void) {
    __HAL_RCC_CAN1_CLK_ENABLE();

    can_gpio_init();

    // Compute timings and initialize CAN peripheral
    canardSTM32ComputeCANTimings(72000000, 1000000, &canard_timings);
    canardSTM32Init(&canard_timings, CanardSTM32IfaceModeNormal);
    
    // Initialise libcanard instance, and set node ID
    canardInit(&canard,
        (void*) &canard_mem_pool,
        CANARD_MEM_POOL_SIZE,
        on_reception,
        should_accept,
        NULL);
    canardSetLocalNodeID(&canard, UAVCAN_NODE_ID);
    
    // Set initial node status
    node_status.health = UAVCAN_PROTOCOL_NODESTATUS_HEALTH_OK;
    node_status.mode = UAVCAN_PROTOCOL_NODESTATUS_MODE_INITIALIZATION;
    node_status.vendor_specific_status_code = UAVCAN_VS_ERROR_OK;
}

static void can_panic(uint8_t vs_error);

void can_send_raw_data(sonar_channel_t channel,
                        uint16_t* p_data, uint16_t num_samples) {

    arvp_sonar_RawData in_data;
    in_data.hydrophone_id = channel;
    // TODO get gain as well
    in_data.data.len = num_samples;
    in_data.data.data = p_data;

    // Encode message
    uint32_t msg_len = arvp_sonar_RawData_encode(&in_data, canard_message_buffer);


    int16_t err = canardBroadcast(&canard,
        ARVP_SONAR_RAWDATA_SIGNATURE,
        UAVCAN_SONAR_RAWDATA_DATATYPE_ID,
        &inout_transfer_id,
        0,  // These messages are very low priority.
        (void*) canard_message_buffer,
        msg_len);

    // Want to be able to relatively easily catch if we run out of memory
    if (err == -3) {
        can_panic(UAVCAN_VS_ERROR_NO_MEM);
    }
}

static void can_panic(uint8_t vs_error) {
    // Publish error node status urgently
    node_status.health = UAVCAN_PROTOCOL_NODESTATUS_HEALTH_WARNING;
    node_status.mode = UAVCAN_PROTOCOL_NODESTATUS_MODE_OFFLINE;
    node_status.vendor_specific_status_code = vs_error;

    // Pop all frames off queue
    while (canardPeekTxQueue(&canard) != NULL) {
        canardPopTxQueue(&canard);
    }

    // Encode message
    uint32_t msg_len = uavcan_protocol_NodeStatus_encode(&node_status, canard_message_buffer);

    canardBroadcast(&canard,
        UAVCAN_PROTOCOL_NODESTATUS_SIGNATURE,
        UAVCAN_PROTOCOL_NODESTATUS_ID,
        &inout_transfer_id,
        CANARD_TRANSFER_PRIORITY_HIGHEST,
        &canard_message_buffer,
        msg_len);

    const CanardCANFrame* p_frame = canardPeekTxQueue(&canard);
    canardSTM32Transmit(p_frame);
    
    while(1);
}


static void can_publish_node_status() {
    uint32_t msg_len = uavcan_protocol_NodeStatus_encode(&node_status, canard_message_buffer);

    canardBroadcast(&canard,
        UAVCAN_PROTOCOL_NODESTATUS_SIGNATURE,
        UAVCAN_PROTOCOL_NODESTATUS_ID,
        &inout_transfer_id,
        CANARD_TRANSFER_PRIORITY_HIGHEST,
        &canard_message_buffer,
        msg_len);

}

void can_periodic(uint64_t current_time_usec) {
    canardCleanupStaleTransfers(&canard, current_time_usec);
    can_publish_node_status();
}


static void on_reception(CanardInstance* ins,
                         CanardRxTransfer* transfer) {

    // Not receiving anything yet
}

static bool should_accept(const CanardInstance* ins,
                          uint64_t* out_data_type_signature,
                          uint16_t data_type_id,
                          CanardTransferType transfer_type,
                          uint8_t source_node_id) {
    // Not receiving anything yet
    return true;
}