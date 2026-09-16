/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "can.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include <stdio.h>
/* USER CODE END Includes */

/* USER CODE BEGIN PTD */
/* USER CODE END PTD */
/* USER CODE BEGIN PD */
/* USER CODE END PD */
/* USER CODE BEGIN PM */
/* USER CODE END PM */
/* USER CODE BEGIN PV */
/* USER CODE END PV */

void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 100);
    return ch;
}

CAN_TxHeaderTypeDef tx_header;
uint32_t tx_mailbox;

void CAN_Start(void)
{
    CAN_FilterTypeDef filter = {0};
    filter.FilterBank           = 0;
    filter.FilterMode           = CAN_FILTERMODE_IDMASK;
    filter.FilterScale          = CAN_FILTERSCALE_32BIT;
    filter.FilterIdHigh         = 0x0000;
    filter.FilterIdLow          = 0x0000;
    filter.FilterMaskIdHigh     = 0x0000;
    filter.FilterMaskIdLow      = 0x0000;
    filter.FilterFIFOAssignment = CAN_RX_FIFO0;
    filter.FilterActivation     = ENABLE;
    filter.SlaveStartFilterBank = 14;

    if (HAL_CAN_ConfigFilter(&hcan, &filter) != HAL_OK)
        printf("[ERR] ConfigFilter failed\r\n");

    if (HAL_CAN_Start(&hcan) != HAL_OK)
        printf("[ERR] CAN_Start failed!\r\n");

    /* ============================================================
     * 【改动1】轮询模式：不开接收中断
     * 中断和轮询会争抢同一个 FIFO：开了中断，帧一进 FIFO 就被
     * 中断取走，主循环轮询永远读到 0。
     * 等轮询版稳定后，再单独做中断版对照实验。
     * ============================================================ */
    /* HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING); */

    /* 开机横幅：一眼看出板子里跑的是哪一版 */
    printf("\r\n===== CAN_LOOPBACK | Mode=%s | Filter=%s | NART=%s | RX=%s | Built %s %s =====\r\n",
           (hcan.Init.Mode == CAN_MODE_LOOPBACK)    ? "LOOPBACK" : "NORMAL",
           (filter.FilterActivation == ENABLE)      ? "ENABLE"   : "DISABLE",
           (hcan.Init.AutoRetransmission == ENABLE) ? "ON"       : "OFF",
           "POLL-NORMAL",
           __DATE__, __TIME__);

    printf("[INIT] BTR=0x%08lX IER=0x%08lX (IER=0 表示没开任何中断)\r\n",
           hcan.Instance->BTR, hcan.Instance->IER);
}

/* 发送一帧。返回 0=成功，1=失败 */
uint8_t CAN_Send(uint32_t id, uint8_t *data, uint8_t len)
{
    if (len > 8) return 1;              /* DLC 保护 */

    tx_header.StdId = id;
    tx_header.IDE   = CAN_ID_STD;
    tx_header.RTR   = CAN_RTR_DATA;
    tx_header.DLC   = len;

    uint32_t t0 = HAL_GetTick();
    while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan) == 0) {
        if (HAL_GetTick() - t0 > 10) {  /* 超时保护，别锁死 */
            printf("[CAN] 邮箱满！查：对端上电？波特率？模式？\r\n");
            return 1;
        }
    }

    return (HAL_CAN_AddTxMessage(&hcan, &tx_header, data, &tx_mailbox) == HAL_OK) ? 0 : 1;
}

/* ============================================================
 * 【改动2】轮询接收：FIFO0 有帧就取出来打印
 * 返回 1 = 取到一帧，返回 0 = 当前没有帧（不阻塞）
 * ============================================================ */
uint8_t CAN_PollReceive(void)
{
    CAN_RxHeaderTypeDef rx_header;
    uint8_t rx_data[8];

    /* ① 先看 FIFO0 里有没有帧 —— 不依赖任何中断 */
    if (HAL_CAN_GetRxFifoFillLevel(&hcan, CAN_RX_FIFO0) == 0)
        return 0;

    /* ② 有帧就取出来 */
    if (HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &rx_header, rx_data) != HAL_OK)
        return 0;

    /* ③ 打印 */
    printf("[RX] ID=0x%03X DLC=%d Data:", rx_header.StdId, rx_header.DLC);
    for (int i = 0; i < rx_header.DLC; i++)
        printf(" %02X", rx_data[i]);
    printf("\r\n");

    return 1;
}
/* USER CODE END 0 */

int main(void)
{
  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  HAL_Init();

  /* USER CODE BEGIN Init */
  /* USER CODE END Init */

  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  MX_GPIO_Init();
  MX_CAN_Init();
  MX_USART1_UART_Init();

  /* USER CODE BEGIN 2 */
  CAN_Start();
  printf("=== CAN Loopback Test (polling RX) ===\r\n");
  /* USER CODE END 2 */

  /* USER CODE BEGIN WHILE */
  while (1)
  {
    uint8_t tx_data[8] = {0x20, 0x23, 0x09, 0x14, 0x00, 0x00, 0x00, 0x00};

    /* --- 发送 --- */
    CAN_Send(0x100, tx_data, (uint8_t)8);
    printf("[TX] ID=0x100 sent\r\n");

    /* ============================================================
     * 【改动3】发送后主动轮询取帧
     * 最多试 20 次 × 1ms = 20ms，取到就 break。
     * 正常情况第 1 次就能取到（帧约 200us 后进 FIFO）。
     * ============================================================ */
//    for (int i = 0; i < 20; i++)
//    {
//        if (CAN_PollReceive() != 0)
//            break;
//        HAL_Delay(1);
//    }

    HAL_Delay(1000);
  }
  /* USER CODE END 3 */
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) { Error_Handler(); }
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) { Error_Handler(); }
}

void Error_Handler(void) { __disable_irq(); while(1){} }
#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line) {}
#endif
