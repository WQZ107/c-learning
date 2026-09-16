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
#include <stdint.h>
#include <string.h>
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
// ==================== 声明工具函数 ====================
void ShowFloatBytes(const char *name, float v);

uint8_t PackFloatLE(float v, uint8_t *out);
uint8_t PackInt16LE(int16_t v, uint8_t *out);
uint8_t PackInt16BE(int16_t v, uint8_t *out);

float UnpackFloatLE(const uint8_t *in);
float UnpackFloatLE_WRONG(const uint8_t *in);
int16_t UnpackInt16LE(const uint8_t *in);
int16_t UnpackInt16BE(const uint8_t *in);

void TestPackUnpack(void);

// 声明协议发送函数
uint8_t CAN_SendLatitude(float lat);
uint8_t CAN_SendLongitude(float lon);
uint8_t CAN_SendSpeed(int16_t speed);
uint8_t CAN_SendTemperature(float temp);

// 声明接收解析函数
void CAN_ProcessRx(CAN_RxHeaderTypeDef *rx_header, uint8_t *rx_data);
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
// ==================== 重定向 printf 到 UART1 ====================
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

    printf("\r\n===== CAN_LOOPBACK | Mode=%s | Filter=%s | Built %s %s =====\r\n",
           (hcan.Init.Mode == CAN_MODE_LOOPBACK) ? "LOOPBACK" : "NORMAL",
           (filter.FilterActivation == ENABLE) ? "ENABLE" : "DISABLE",
           __DATE__, __TIME__);
}

uint8_t CAN_Send(uint32_t id, uint8_t *data, uint8_t len)
{
    if (len > 8) return 1;

    tx_header.StdId = id;
    tx_header.IDE   = CAN_ID_STD;
    tx_header.RTR   = CAN_RTR_DATA;
    tx_header.DLC   = len;

    uint32_t t0 = HAL_GetTick();
    while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan) == 0) {
        if (HAL_GetTick() - t0 > 10) {
            printf("[CAN] Mailbox full!\r\n");
            return 1;
        }
    }

    return (HAL_CAN_AddTxMessage(&hcan, &tx_header, data, &tx_mailbox) == HAL_OK) ? 0 : 1;
}

uint8_t CAN_PollReceive(void)
{
    CAN_RxHeaderTypeDef rx_header;
    uint8_t rx_data[8];

    if (HAL_CAN_GetRxFifoFillLevel(&hcan, CAN_RX_FIFO0) == 0)
        return 0;

    if (HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &rx_header, rx_data) != HAL_OK)
        return 0;

    // 收到数据后调用解析函数处理
    CAN_ProcessRx(&rx_header, rx_data);
    return 1;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
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

  // 执行打包解包测试
  TestPackUnpack();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    // 发送纬度 23.123456（4字节float 小端）
    float lat = 23.123456f;
    printf("[TX] 0x100 lat=%.6f\r\n", lat);
    CAN_SendLatitude(lat);

    // 轮询接收
    CAN_PollReceive();

    HAL_Delay(1000);
  }
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
}
/* USER CODE END 3 */

/**
  * @brief System Clock Configuration
  * @retval None
  */
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

/* USER CODE BEGIN 4 */
// ==================== 4个协议发送函数 ====================

// 0x100 纬度 (4B float 小端)
uint8_t CAN_SendLatitude(float lat)
{
    uint8_t data[8] = {0};
    PackFloatLE(lat, data);
    return CAN_Send(0x100, data, 4);
}

// 0x101 经度 (4B float 小端)
uint8_t CAN_SendLongitude(float lon)
{
    uint8_t data[8] = {0};
    PackFloatLE(lon, data);
    return CAN_Send(0x101, data, 4);
}

// 0x102 车速 (2B int16 大端)
uint8_t CAN_SendSpeed(int16_t speed)
{
    uint8_t data[8] = {0};
    PackInt16BE(speed, data);
    return CAN_Send(0x102, data, 2);
}

// 0x103 温度 (2B int16 大端, 需乘以10)
uint8_t CAN_SendTemperature(float temp)
{
    uint8_t data[8] = {0};
    PackInt16BE((int16_t)(temp * 10.0f), data);
    return CAN_Send(0x103, data, 2);
}


// ==================== 接收解析函数 ====================
void CAN_ProcessRx(CAN_RxHeaderTypeDef *rx_header, uint8_t *rx_data)
{
    printf("[RX] ID=0x%03X DLC=%d Data: ", rx_header->StdId, rx_header->DLC);
    for (int i = 0; i < rx_header->DLC; i++) {
        printf("%02X ", rx_data[i]);
    }
    printf(" -> ");

    switch (rx_header->StdId)
    {
        case 0x100:
            printf("lat = %.6f\r\n", UnpackFloatLE(rx_data));
            break;
        case 0x101:
            printf("lon = %.6f\r\n", UnpackFloatLE(rx_data));
            break;
        case 0x102:
            printf("speed = %d\r\n", UnpackInt16BE(rx_data));
            break;
        case 0x103:
            printf("temp = %.1f\r\n", UnpackInt16BE(rx_data) / 10.0f);
            break;
        default:
            printf("unknown ID\r\n");
            break;
    }
}


// ==================== 工具函数实现区 ====================

void ShowFloatBytes(const char *name, float v)
{
    union { float f; uint8_t b[4]; } u;
    u.f = v;
    printf("%s -> %02X %02X %02X %02X\r\n", name, u.b[0], u.b[1], u.b[2], u.b[3]);
}

uint8_t PackFloatLE(float v, uint8_t *out)
{
    union { float f; uint8_t b[4]; } u;
    u.f = v;
    out[0] = u.b[0]; out[1] = u.b[1];
    out[2] = u.b[2]; out[3] = u.b[3];
    return 4;
}

// 小端打包 int16
uint8_t PackInt16LE(int16_t v, uint8_t *out)
{
    out[0] = (uint8_t)(v & 0xFF);
    out[1] = (uint8_t)((v >> 8) & 0xFF);
    return 2;
}

// 大端打包 int16
uint8_t PackInt16BE(int16_t v, uint8_t *out)
{
    out[0] = (uint8_t)((v >> 8) & 0xFF);
    out[1] = (uint8_t)(v & 0xFF);
    return 2;
}

float UnpackFloatLE(const uint8_t *in)
{
    union { float f; uint8_t b[4]; } u;
    u.b[0] = in[0]; u.b[1] = in[1];
    u.b[2] = in[2]; u.b[3] = in[3];
    return u.f;
}

// 故意用大端解包, 用于对比错误字节序
float UnpackFloatLE_WRONG(const uint8_t *in)
{
    float v; uint8_t tmp[4] = { in[3], in[2], in[1], in[0] };
    memcpy(&v, tmp, 4);
    return v;
}

// 小端解包 int16
int16_t UnpackInt16LE(const uint8_t *in)
{
    return (int16_t)((in[1] << 8) | in[0]);
}

// 大端解包 int16
int16_t UnpackInt16BE(const uint8_t *in)
{
    return (int16_t)((in[0] << 8) | in[1]);
}

void TestPackUnpack(void)
{
    uint8_t buf[8];
    float  f_in = 23.123456f, f_out;
    int16_t i_in = -50, i_out;

    PackFloatLE(f_in, buf);
    f_out = UnpackFloatLE(buf);
    printf("float : %.6f -> %02X %02X %02X %02X -> %.6f  %s\r\n",
           f_in, buf[0], buf[1], buf[2], buf[3], f_out,
           (f_in == f_out) ? "OK" : "ERR");

    // 故意用大端解包, 看错误字节序长什么样
    float f_wrong = UnpackFloatLE_WRONG(buf);
    printf("wrong : %.6f -> %02X %02X %02X %02X -> %.6f  ERR!\r\n",
           f_in, buf[0], buf[1], buf[2], buf[3], f_wrong);

    // 测试大端
    PackInt16BE(i_in, buf);
    i_out = UnpackInt16BE(buf);
    printf("int16 BE: %d -> %02X %02X -> %d  %s\r\n",
           i_in, buf[0], buf[1], i_out, (i_in == i_out) ? "OK" : "ERR");
}
/* USER CODE END 4 */

void Error_Handler(void)
{
  __disable_irq();
  while (1) { }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line) { }
#endif
