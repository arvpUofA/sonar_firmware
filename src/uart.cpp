#include "uart.h"
#include "pinout.h"

static UART_HandleTypeDef huart;

bool receive_finished_flag;
uint8_t incoming_message_len;
char incoming_message[50];

void uart_init(void) {
    // Get flag pointer
    incoming_message_len = 0;

    // Enable clocks
    __HAL_RCC_USART1_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    // Configure GPIO
    GPIO_InitTypeDef gpio;
    gpio.Pin = UART_TX_PIN;
    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Alternate = GPIO_AF7_USART1; // TODO match this up
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(UART_PORT, &gpio);

    gpio.Pin = UART_RX_PIN;
    gpio.Mode = GPIO_MODE_INPUT; // not sure if this is correct
    HAL_GPIO_Init(UART_PORT, &gpio);

    // Configure UART
    huart.Instance = USART1;

    huart.Init.BaudRate = 115200;
    huart.Init.WordLength = UART_WORDLENGTH_8B;
    huart.Init.StopBits = UART_STOPBITS_1;
    huart.Init.Parity = UART_PARITY_NONE;
    huart.Init.Mode = UART_MODE_TX_RX;
    huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart.Init.OverSampling = UART_OVERSAMPLING_8;
    huart.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_ENABLE;
    HAL_UART_Init(&huart);

    // Configure Interrupt vectors
    NVIC_SetPriority(USART1_IRQn, 2);
    NVIC_EnableIRQ(USART1_IRQn);

    // Configure USART interrupts
    __HAL_UART_CLEAR_IT(&huart, UART_IT_RXNE);
    __HAL_UART_ENABLE_IT(&huart, UART_IT_RXNE);
}

void uart_send(uint8_t* data, uint16_t len) {
    HAL_UART_Transmit(&huart, data, len, 200);
}

void USART1_IRQHandler(void) {
    if (__HAL_UART_GET_IT(&huart, UART_IT_RXNE)) {
        // Here we can handle incoming messages
        __HAL_UART_CLEAR_IT(&huart, UART_IT_RXNE); // Clear interrupt

        if (receive_finished_flag) {
            // do nothing, we are still processing the message so we shouldn't be receiving more
        } else {
            // Read in data
            incoming_message[incoming_message_len] = USART1->RDR;
            if (incoming_message[incoming_message_len] == '\n') {
                // Message is over, raise flag
                receive_finished_flag = true;
            }
            
            // Increment length
            incoming_message_len++;
        }
    }

    return;
}