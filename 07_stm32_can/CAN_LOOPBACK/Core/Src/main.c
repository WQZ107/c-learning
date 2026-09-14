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

    HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
    hcan.Instance->IER |= CAN_IER_FMPIE0;

    printf("[INIT] BTR=0x%08lX MSG_PENDING=0x%08lX\r\n",
           hcan.Instance->BTR, (uint32_t)CAN_IT_RX_FIFO0_MSG_PENDING);
    
        /* ===== 三个关键探针 ===== */
    printf("[A] RF1R=0x%08lX (FMP1=%lu)\r\n",
           hcan.Instance->RF1R, hcan.Instance->RF1R & 3U);

    printf("[B] FFA1R=0x%08lX (bit0=%lu -> 0:FIFO0  1:FIFO1)\r\n",
           *(volatile uint32_t *)0x40006614U,
           *(volatile uint32_t *)0x40006614U & 1U);

    printf("[C] MCR=0x%08lX MSR=0x%08lX (INAK=%lu SLAK=%lu)\r\n",
           hcan.Instance->MCR, hcan.Instance->MSR,
           hcan.Instance->MSR & 1U, (hcan.Instance->MSR >> 1) & 1U);
}

void CAN_Send(uint32_t id, uint8_t *data, uint8_t len)
{
    tx_header.StdId = id;
    tx_header.IDE   = CAN_ID_STD;
    tx_header.RTR   = CAN_RTR_DATA;
    tx_header.DLC   = len;

    /* 等空邮箱（带超时，别死等） */
    uint32_t t0 = HAL_GetTick();
    while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan) == 0) {
        if (HAL_GetTick() - t0 > 10) {
            printf("[CAN] 邮箱满! 查: 对端上电? 波特率? 模式?\r\n");
            return;
        }
    }

    hcan.Instance->TSR = 0;                                    /* 清历史标志 */
    if (HAL_CAN_AddTxMessage(&hcan, &tx_header, data, &tx_mailbox) != HAL_OK) {
        printf("[CAN] AddTx 失败\r\n");
        return;
    }

    /* 等本帧握手结束，再判 TXOK —— 只发一次，但等出结果 */
    t0 = HAL_GetTick();
    while ((hcan.Instance->TSR & 1U) == 0) {                    /* 等 RQCP0 */
        if (HAL_GetTick() - t0 > 10) break;
    }

    printf("[D] TSR=0x%08lX TXOK0=%lu TERR0=%lu\r\n",
           hcan.Instance->TSR,
           (hcan.Instance->TSR >> 1) & 1U, (hcan.Instance->TSR >> 2) & 1U);
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *can_h)
{
    CAN_RxHeaderTypeDef rx_header;
    uint8_t rx_data[8];
    HAL_CAN_GetRxMessage(can_h, CAN_RX_FIFO0, &rx_header, rx_data);
    printf("[RX] ID=0x%03X DLC=%d Data:", rx_header.StdId, rx_header.DLC);
    for (int i = 0; i < rx_header.DLC; i++) printf(" %02X", rx_data[i]);
    printf("\r\n");
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
  printf("=== CAN Loopback Test ===\r\n");
  /* USER CODE END 2 */

  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */
    uint8_t tx_data[8] = {0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};
    CAN_Send(0x100, tx_data, 8);
    printf("[TX] ID=0x100 sent\r\n");

    printf("[DBG] IER=0x%08lX RF0R=0x%08lX BTR=0x%08lX\r\n",
       hcan.Instance->IER, hcan.Instance->RF0R, hcan.Instance->BTR);

    printf("[FLT] FA1R=0x%08lX FM1R=0x%08lX FS1R=0x%08lX F0R1=0x%08lX F0R2=0x%08lX\r\n",
       *(volatile uint32_t *)0x4000661C,
       *(volatile uint32_t *)0x40006604,
       *(volatile uint32_t *)0x4000660C,
       *(volatile uint32_t *)0x40006640,
       *(volatile uint32_t *)0x40006644);

    printf("[FMR] FMR=0x%08lX CAN2SB=%ld\r\n",
       hcan.Instance->FMR, (hcan.Instance->FMR >> 8) & 0xFF);

    if (HAL_CAN_GetRxFifoFillLevel(&hcan, CAN_RX_FIFO0) > 0)
        printf("[POLL] FIFO0 you zhen!\r\n");

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
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) { Error_Handler(); }
}

void Error_Handler(void) { __disable_irq(); while(1){} }
#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line) {}
#endif
