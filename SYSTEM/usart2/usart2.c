#include "usart2.h"
#include "string.h"

// 接收缓冲区
uint8_t USART2_RX_BUF[USART2_RX_BUF_SIZE];

// 接收状态
// bit15: 接收完成 (收到 \r\n)
// bit14: 收到 \r
// bit13~0: 数据长度
volatile uint16_t USART2_RX_STA = 0;

/**
 * @brief  初始化 USART2 (PA2=TX, PA3=RX)
 * @param  bound: 波特率
 */
void USART2_Init(uint32_t bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // 使能时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    // GPIO 复用配置
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

    // PA2=TX, PA3=RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // USART2 参数配置
    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);

    // 使能 USART2
    USART_Cmd(USART2, ENABLE);

    // 开启接收中断
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

    // NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
 * @brief  USART2 中断服务函数
 */
void USART2_IRQHandler(void)
{
    uint8_t res;

    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
        res = USART_ReceiveData(USART2);

        // 如果接收未完成
        if ((USART2_RX_STA & 0x8000) == 0) {
            // 如果上次收到了 \r
            if (USART2_RX_STA & 0x4000) {
                if (res != 0x0A) {
                    // 不是 \n，重新开始
                    USART2_RX_STA = 0;
                } else {
                    // 收到 \r\n，接收完成
                    USART2_RX_STA |= 0x8000;
                }
            } else {
                // 还没收到 \r
                if (res == 0x0D) {
                    // 标记收到 \r
                    USART2_RX_STA |= 0x4000;
                } else {
                    // 存储数据
                    USART2_RX_BUF[USART2_RX_STA & 0x3FFF] = res;
                    USART2_RX_STA++;

                    // 防止溢出
                    if (USART2_RX_STA > (USART2_RX_BUF_SIZE - 1)) {
                        USART2_RX_STA = 0;
                    }
                }
            }
        }
    }
}

/**
 * @brief  检查接收是否完成
 * @return 1=完成, 0=未完成
 */
uint8_t USART2_RxComplete(void)
{
    return (USART2_RX_STA & 0x8000) ? 1 : 0;
}

/**
 * @brief  重置接收状态，准备下一次接收
 */
void USART2_RxReset(void)
{
    USART2_RX_STA = 0;
    memset(USART2_RX_BUF, 0, USART2_RX_BUF_SIZE);
}

/**
 * @brief  通过 USART2 发送一个字节
 */
void USART2_SendByte(uint8_t data)
{
    USART_SendData(USART2, data);
    while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
}

/**
 * @brief  通过 USART2 发送字符串
 */
void USART2_SendString(const char *str)
{
    while (*str) {
        USART2_SendByte(*str++);
    }
}
