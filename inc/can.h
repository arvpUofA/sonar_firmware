#ifndef CAN_H_
#define CAN_H_

#include "stm32f3xx.h"
#include "main.h"
#include "canard.h"
#include "canard_stm32.h"
#include "config.h"

#include "uavcan/protocol/NodeStatus.h"
#include "uavcan/protocol/param/GetSet.h"
#include "uavcan/protocol/RestartNode.h"
#include "uavcan/protocol/GetNodeInfo.h"
#include "arvp/sonar/RawData.h"

// Just a few error codes for us to use over CAN bus
#define UAVCAN_VS_ERROR_OK      0
#define UAVCAN_VS_ERROR_NO_MEM  1

typedef enum {
    SONAR_CHANNEL_REF = ARVP_SONAR_RAWDATA_CHANNEL_REF,
    SONAR_CHANNEL_A = ARVP_SONAR_RAWDATA_CHANNEL_A,
    SONAR_CHANNEL_B = ARVP_SONAR_RAWDATA_CHANNEL_B,
    SONAR_CHANNEL_C = ARVP_SONAR_RAWDATA_CHANNEL_C,
} sonar_channel_t;

/** @brief Initialise CAN hardware, as well as libcanard.
 * 
 */
void can_init(void);

/** @brief Encode channel data into CAN frames and push to TX queue.
 * 
 *  @param channel ID of the hydrophone channel
 *  @param p_data pointer to array of raw data
 *  @param num_samples number of raw samples
 */
void can_send_raw_data(sonar_channel_t channel,
                        uint16_t* p_data, uint16_t num_samples);


/** @brief Runs periodic CAN cleanup, etc. Should be run at about 1Hz
 * 
 */
void can_periodic(uint64_t current_time_us);


/** @brief Starts transmit chain
 * 
 * Sends the first message, and then interrupts take over to finish transfer
 */
void can_start_transmit(void);

#endif