#include "board.h"

uartPort_t *serialUSART1(uint32_t baudRate, portMode_t mode);
uartPort_t *serialUSART2(uint32_t baudRate, portMode_t mode);
uartPort_t *serialUSART3(uint32_t baudRate, portMode_t mode);

void uartPause(int n)
{
    switch (n) {
        case 3:
            USART_Cmd(USART3, DISABLE);

            break;
        default:
            break;
    }

}

void uartUnPause(int n)
{
    switch (n) {
        case 3:
            USART_Cmd(USART3, ENABLE);
            break;
        default:
            break;
    }
}

static void uartReconfigure(uartPort_t *uartPort)
{
    USART_InitTypeDef USART_InitStructure;

    USART_InitStructure.USART_BaudRate = uartPort->port.baudRate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    if (uartPort->port.mode & MODE_SBUS) {
        USART_InitStructure.USART_StopBits = USART_StopBits_2;
        USART_InitStructure.USART_Parity = USART_Parity_Even;
    } else {
        USART_InitStructure.USART_StopBits = USART_StopBits_1;
        USART_InitStructure.USART_Parity = USART_Parity_No;
    }
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = 0;
    if (uartPort->port.mode & MODE_RX)
        USART_InitStructure.USART_Mode |= USART_Mode_Rx;
    if (uartPort->port.mode & MODE_TX)
        USART_InitStructure.USART_Mode |= USART_Mode_Tx;

    USART_Init(uartPort->USARTx, &USART_InitStructure);
}

serialPort_t *uartOpen(USART_TypeDef *USARTx, serialReceiveCallbackPtr callback, uint32_t baudRate, portMode_t mode, serialInversion_e inversion)
{
    DMA_InitTypeDef DMA_InitStructure;

    uartPort_t *s = NULL;

    if (USARTx == USART1) {
        s = serialUSART1(baudRate, mode);
    } else if (USARTx == USART2) {
        s = serialUSART2(baudRate, mode);
    } else if (USARTx == USART3) {
        s = serialUSART3(baudRate, mode);
    } else {
        return (serialPort_t *)s;
    }
    s->txDMAEmpty = true;
    
    // common serial initialisation code should move to serialPort::init()
    s->port.rxBufferHead = s->port.rxBufferTail = 0;
    s->port.txBufferHead = s->port.txBufferTail = 0;
    // callback for IRQ-based RX ONLY
    s->port.callback = callback;
    s->port.mode = mode;
    s->port.baudRate = baudRate;

#if 1 // FIXME use inversion on STM32F3
    s->port.inversion = SERIAL_NOT_INVERTED;
#else
    s->port.inversion = inversion;
#endif

    uartReconfigure(s);


    // Receive DMA or IRQ
    if (mode & MODE_RX) {
//        if (s->rxDMAChannel) {
//            DMA_InitStructure.DMA_BufferSize = s->port.rxBufferSize;
//            DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
//            DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
//            DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)s->port.rxBuffer;
//            DMA_DeInit(s->rxDMAChannel);
//            DMA_Init(s->rxDMAChannel, &DMA_InitStructure);
//            DMA_Cmd(s->rxDMAChannel, ENABLE);
//            USART_DMACmd(s->USARTx, USART_DMAReq_Rx, ENABLE);
//            s->rxDMAPos = DMA_GetCurrDataCounter(s->rxDMAChannel);
//        } else {
            USART_ClearITPendingBit(s->USARTx, USART_IT_RXNE);
            USART_ITConfig(s->USARTx, USART_IT_RXNE, ENABLE);
//        }
    }


    // Transmit DMA or IRQ
    if (mode & MODE_TX) {
//        if (s->txDMAChannel) {
//            DMA_InitStructure.DMA_BufferSize = s->port.txBufferSize;
//            DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
//            DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
//            DMA_DeInit(s->txDMAChannel);
//            DMA_Init(s->txDMAChannel, &DMA_InitStructure);
//            DMA_ITConfig(s->txDMAChannel, DMA_IT_TC, ENABLE);
//            DMA_SetCurrDataCounter(s->txDMAChannel, 0);
//            s->txDMAChannel->CNDTR = 0;
//            USART_DMACmd(s->USARTx, USART_DMAReq_Tx, ENABLE);
//        } else {
            USART_ITConfig(s->USARTx, USART_IT_TXE, ENABLE);
//        }
    }

    USART_Cmd(s->USARTx, ENABLE);

    return (serialPort_t *)s;
}

void uartSetBaudRate(serialPort_t *instance, uint32_t baudRate)
{
    uartPort_t *uartPort = (uartPort_t *)instance;
    uartPort->port.baudRate = baudRate;
#ifndef STM32F303xC// FIXME this doesnt seem to work, for now re-open the port from scratch, perhaps clearing some uart flags may help?
    uartReconfigure(uartPort);
#else
    uartOpen(uartPort->USARTx, uartPort->port.callback, uartPort->port.baudRate, uartPort->port.mode, uartPort->port.inversion);
#endif
}

void uartSetMode(serialPort_t *instance, portMode_t mode)
{
    uartPort_t *uartPort = (uartPort_t *)instance;
    uartPort->port.mode = mode;
#ifndef STM32F303xC// FIXME this doesnt seem to work, for now re-open the port from scratch, perhaps clearing some uart flags may help?
    uartReconfigure(uartPort);
#else
    uartOpen(uartPort->USARTx, uartPort->port.callback, uartPort->port.baudRate, uartPort->port.mode, uartPort->port.inversion);
#endif
}


uint32_t uartTotalBytesWaiting(serialPort_t *instance)
{
    uartPort_t *s = (uartPort_t*)instance;
    // FIXME always returns 1 or 0, not the amount of bytes waiting
//    if (s->rxDMAChannel)
//        return (uint32_t)s->rxDMAChannel->CNDTR != s->rxDMAPos;
//    else
        return (uint32_t)s->port.rxBufferTail != s->port.rxBufferHead;
}

bool isUartTransmitBufferEmpty(serialPort_t *instance)
{
    uartPort_t *s = (uartPort_t *)instance;
//    if (s->txDMAChannel)
//        return s->txDMAEmpty;
//    else
        return s->port.txBufferTail == s->port.txBufferHead;
}

uint8_t uartRead(serialPort_t *instance)
{
    uint8_t ch;
    uartPort_t *s = (uartPort_t *)instance;

//    if (s->rxDMAChannel) {
//        ch = s->port.rxBuffer[s->port.rxBufferSize - s->rxDMAPos];
//        if (--s->rxDMAPos == 0)
//            s->rxDMAPos = s->port.rxBufferSize;
//    } else {
        ch = s->port.rxBuffer[s->port.rxBufferTail];
        s->port.rxBufferTail = (s->port.rxBufferTail + 1) % s->port.rxBufferSize;
//    }

    return ch;
}

void uartWrite(serialPort_t *instance, uint8_t ch)
{
    uartPort_t *s = (uartPort_t *)instance;
    s->port.txBuffer[s->port.txBufferHead] = ch;
    s->port.txBufferHead = (s->port.txBufferHead + 1) % s->port.txBufferSize;

//    if (s->txDMAChannel) {
//        if (!(s->txDMAChannel->CCR & 1))
//            uartStartTxDMA(s);
//    } else {
        USART_ITConfig(s->USARTx, USART_IT_TXE, ENABLE);
//    }
}

const struct serialPortVTable uartVTable[] = { { uartWrite, uartTotalBytesWaiting, uartRead, uartSetBaudRate, isUartTransmitBufferEmpty, uartSetMode, } };
