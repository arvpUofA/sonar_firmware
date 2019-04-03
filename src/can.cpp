#include "can.h"

#include "arvp/sonar/RawData.h"

static CanardInstance canard;

// Max size of sonar transfer is ~4K, not sure how much memory overhead they have
// libcanard memory pool for storing TX queue.
#define CANARD_MEM_POOL_SIZE    5000
static uint8_t canard_mem_pool[CANARD_MEM_POOL_SIZE];

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
}


static void on_reception(CanardInstance* ins,
                         CanardRxTransfer* transfer) {

    // do things
}

static bool should_accept(const CanardInstance* ins,
                          uint64_t* out_data_type_signature,
                          uint16_t data_type_id,
                          CanardTransferType transfer_type,
                          uint8_t source_node_id) {
    // do things
    return true;
}