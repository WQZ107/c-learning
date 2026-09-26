/* ★ 2026-09-23 块D Day③ 核对版（可直接替换工程 Core\Src\main.c）
   相对原版的两处修正：
   1) HAL_Delay(1000) -> rx_count += DrainRx(1000) 并挪到 [STAT] 之前
      —— 命令帧不再憋 1 秒，收到立即回帧；命令帧也计入 rx 统计
   2) 整理 case 0x200 缩进、更新过时注释（不影响功能）
*/
#include "main.h"
#include "can.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
//新增结构体
typedef struct {
    float   lat;        /* 0x100 纬度    float  小端          */
    float   lon;        /* 0x101 经度    float  小端          */
    int16_t speed;      /* 0x102 车速    int16  大端 km/h     */
    int16_t temp_x10;   /* 0x103 温度x10 int16  大端 285=28.5C */
    int16_t acc_x;      /* 0x104 加速度X  int16 大端 0.01g    */
    int16_t acc_y;      /* 0x105 加速度Y  int16 大端 0.01g    */
    int16_t acc_z;      /* 0x106 加速度Z  int16 大端 0.01g    */
    uint8_t sat_num;    /* 0x107 数据[0] 卫星数               */
    uint8_t alarm_flag; /* 0x107 数据[1] 告警标志              */
}VehicleData;

static VehicleData g_vehicle = { 23.123456f, 113.123456f, 60, 285, 0, 0, 0, 8, 0 };
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

// ==================== CAN 驱动层函数（★ 新增声明）====================
// DrainRx 和下面四个发送函数都要用到它们，先声明再定义，避免隐式声明
uint8_t CAN_Send(uint32_t id, uint8_t *data, uint8_t len);
uint8_t CAN_PollReceive(void);

// ==================== 声明协议发送函数 (块E day1 ：改为无参直读 g_vehicle)====================
uint8_t CAN_SendLatitude(void);                          /* 0x100 float 小端      */
uint8_t CAN_SendLongitude(void);                         /* 0x101 float 小端      */
uint8_t CAN_SendSpeed(void);                             /* 0x102 int16 大端      */
uint8_t CAN_SendTempX10(void);                           /* 0x103 int16 大端 x10  */
uint8_t CAN_SendAccAxis(uint32_t id, int16_t acc_g100);  /* 0x104/105/106 共用    */
uint8_t CAN_SendSatInfo(void);                           /* 0x107 卫星数+告警标志 */
uint8_t CAN_ReportGps(void);                             /* 1秒组 4帧             */
uint8_t CAN_ReportAcc(void);                             /* 100ms组 3帧(Day②用)   */
uint8_t CAN_ReportAll(void);                             /* 整包 8帧              */
void Sensor_Update(void);                                /* 模拟数据源(KP-E7)     */

// ==================== 声明接收解析函数 ====================
void CAN_ProcessRx(CAN_RxHeaderTypeDef *rx_header, uint8_t *rx_data);
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
// ==================== 实验开关 ====================
#define FILTER_GPS_ONLY   0      /* 0 = 过滤器全通过（块D 用）；1 = 只放行 0x100~0x103 */
#define RX_PRINT_FULL     1      /* 1 = 完整打印（约 5ms/行）；0 = 精简打印（约 2.2ms/行）*/

// ==================== 重定向 printf 到 UART1 ====================
int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 100);
    return ch;
}

CAN_TxHeaderTypeDef tx_header;

uint32_t tx_mailbox;
/* ★ 旧散装全局 g_lat/g_lon/g_spd/g_tmp 已删除 —— 状态统一在 PV 区的 g_vehicle 里 */

void CAN_Start(void)
{
    CAN_FilterTypeDef filter = {0};      /* ★ 必须清零：FilterBank 也要有确定值 */

    filter.FilterBank           = 0;
    filter.FilterMode           = CAN_FILTERMODE_IDMASK;        /* 掩码模式 */
    filter.FilterScale          = CAN_FILTERSCALE_32BIT;        /* 32 位 */
    filter.FilterFIFOAssignment = CAN_RX_FIFO0;
    filter.FilterActivation     = ENABLE;                       /* 漏了它 = 一帧都收不到 */
    filter.SlaveStartFilterBank = 14;

    if (FILTER_GPS_ONLY) {
        filter.FilterIdHigh     = (0x100 << 5);   /* = 0x2000  期望 ID = 0x100 */
        filter.FilterMaskIdHigh = (0x7FC << 5);   /* = 0xFF80  比较 bit10~bit2 */
    } else {
        filter.FilterIdHigh     = 0x0000;
        filter.FilterMaskIdHigh = 0x0000;         /* 掩码全 0 -> 全放行 */
    }
    filter.FilterIdLow      = 0x0000;             /* 32 位模式下标准帧只用 High 这半 */
    filter.FilterMaskIdLow  = 0x0000;
    /* ★ 每个字段只赋值一次：老代码里那四行重复的全 0 赋值会把上面 FILTER_GPS_ONLY
       的结果覆盖回「全通过」——已经删掉，别再抄回来。 */

    if (HAL_CAN_ConfigFilter(&hcan, &filter) != HAL_OK) {
        printf("[ERR] ConfigFilter failed\r\n");
        Error_Handler();                 /* 配不上就别往下跑，否则后面全是假现象 */
    }
    if (HAL_CAN_Start(&hcan) != HAL_OK) {
        printf("[ERR] CAN_Start failed!\r\n");
        Error_Handler();
    }

    printf("\r\n===== CAN_LOOPBACK | Mode=%s | Filter=%s | Built %s %s =====\r\n",
           (hcan.Init.Mode == CAN_MODE_LOOPBACK) ? "LOOPBACK" : "NORMAL",
           (filter.FilterActivation == ENABLE) ? "ENABLE" : "DISABLE",
           __DATE__, __TIME__);

    printf("[INIT] IER=0x%08lX BTR=0x%08lX RF0R=0x%08lX\r\n",
           hcan.Instance->IER, hcan.Instance->BTR, hcan.Instance->RF0R);

    printf("Filter: IdHigh=0x%04X MaskHigh=0x%04X -> %s\r\n",
           filter.FilterIdHigh, filter.FilterMaskIdHigh,
           (filter.FilterMaskIdHigh == 0) ? "PASS-ALL" : "ONLY 0x100~0x103");
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

/* ================= 接收节奏：在 ms 毫秒内把 FIFO 取空 =================
 * ★ 位置：必须放在 CAN_PollReceive() 之后（上一版放在最前面 -> 隐式声明报错）
 * 用法：发一帧 -> DrainRx(3) -> 再发下一帧
 * 这样任何时刻 FIFO0 里最多只有 1 帧，永远撞不到「3 格满 -> 硬件静默丢帧」。
 */
static uint8_t DrainRx(uint32_t ms)
{
    uint8_t  n  = 0;
    uint32_t t0 = HAL_GetTick();
    while (HAL_GetTick() - t0 < ms) {
        if (CAN_PollReceive() != 0) n++;
        else HAL_Delay(1);
    }
    return n;
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
    printf("=== CAN Sensor Node (8-ID report) ===\r\n");

  // 执行打包解包测试
  TestPackUnpack();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      /* ---- 1) 采集(模拟数据源, KP-E7) ---- */
      Sensor_Update();
      printf("[TX] lat=%.6f lon=%.6f spd=%d tmp=%.1f\r\n",
      g_vehicle.lat, g_vehicle.lon, g_vehicle.speed, g_vehicle.temp_x10 / 10.0f);
      
      /* ---- 2) 整包上报 8 帧(KP-E6), 每帧后的 DrainRx(3) 就是帧间隔(KP-E3) ---- */
      uint8_t tx_err = CAN_ReportAll();
      
      /* ---- 3) 1 秒监听窗口: 0x200 命令随到随回 ---- */
      int rx_count = DrainRx(1000);
      
      printf("[STAT] rx=%d tx_err=0x%02X\r\n",rx_count,tx_err);
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
// ==================== 协议发送层(KP-E1/E2: 直读 g_vehicle, 无参) ====================
// 0x100 纬度 (4B float 小端)
uint8_t CAN_SendLatitude(void)
{
    uint8_t data[8] = {0};
    PackFloatLE(g_vehicle.lat, data);
    return CAN_Send(0x100, data, 4);
}

// 0x101 经度 (4B float 小端)
uint8_t CAN_SendLongitude(void)
{
    uint8_t data[8] = {0};
    PackFloatLE(g_vehicle.lon, data);
    return CAN_Send(0x101, data, 4);
}

// 0x102 车速 (2B int16 大端)
uint8_t CAN_SendSpeed(void)
{
    uint8_t data[8] = {0};
    PackInt16BE(g_vehicle.speed, data);
    return CAN_Send(0x102, data, 2);
}

// 0x103 温度 (2B int16 大端, 结构体里已是 x10 整数, 不再四舍五入)
uint8_t CAN_SendTempX10(void)
{
    uint8_t data[8] = {0};
    PackInt16BE(g_vehicle.temp_x10, data);
    return CAN_Send(0x103, data, 2);
}

// 0x104/0x105/0x106 加速度 XYZ 共用 (2B int16 大端, 单位 0.01g)
uint8_t CAN_SendAccAxis(uint32_t id, int16_t acc_g100)
{
    uint8_t data[8] = {0};
    PackInt16BE(acc_g100, data);
    return CAN_Send(id, data, 2);
}

// 0x107 卫星数 + 告警标志 (2B: data[0]=卫星数, data[1]=标志)
uint8_t CAN_SendSatInfo(void)
{
    uint8_t data[8] = {0};
    data[0] = g_vehicle.sat_num;
    data[1] = g_vehicle.alarm_flag;
    return CAN_Send(0x107, data, 2);
}

// ==================== ★ 分组上报(KP-E6) ====================

/* 1 秒组 4 帧: 0x100/0x101/0x102/0x107 —— 返回错误位图 bit0~3 */
uint8_t CAN_ReportGps(void)
{
    uint8_t err = 0;
    if (CAN_SendLatitude()  != 0) err |= (1U << 0);
    DrainRx(3);                                   /* 帧间隔 + FIFO防积压(KP-E3) */
    if (CAN_SendLongitude() != 0) err |= (1U << 1);
    DrainRx(3);
    if (CAN_SendSpeed()     != 0) err |= (1U << 2);
    DrainRx(3);
    if (CAN_SendSatInfo()   != 0) err |= (1U << 3);
    DrainRx(3);
    return err;
}

/* 100ms 组 3 帧: 0x104/0x105/0x106 —— 返回错误位图 bit0~2 (Day② 调度用) */
uint8_t CAN_ReportAcc(void)
{
    uint8_t err = 0;
    if (CAN_SendAccAxis(0x104, g_vehicle.acc_x) != 0) err |= (1U << 0);
    DrainRx(3);
    if (CAN_SendAccAxis(0x105, g_vehicle.acc_y) != 0) err |= (1U << 1);
    DrainRx(3);
    if (CAN_SendAccAxis(0x106, g_vehicle.acc_z) != 0) err |= (1U << 2);
    DrainRx(3);
    return err;
}

/* ★ 整包 8 帧: bit0~3=GPS组, bit4=温度, bit5~7=加速度组 */
uint8_t CAN_ReportAll(void)
{
    uint8_t err = CAN_ReportGps();
    if (CAN_SendTempX10() != 0) err |= (1U << 4);
    DrainRx(3);
    err |= (uint8_t)(CAN_ReportAcc() << 5);
    return err;
}

// ==================== ★ 模拟数据源(KP-E7: Day46 换真传感器只动这一段) ====================

void UpdateGps(void)
{
    g_vehicle.lat += (rand() % 10 - 5) / 100000.0f;   /* ±0.00005 漂移 */
    g_vehicle.lon += (rand() % 10 - 5) / 100000.0f;
    g_vehicle.speed += (rand() % 7 - 3);              /* ±3 km/h */
    if (g_vehicle.speed < 0)   g_vehicle.speed = 0;
    if (g_vehicle.speed > 120) g_vehicle.speed = 120;
}

void UpdateTemp(void)
{
    g_vehicle.temp_x10 += (rand() % 5 - 2);           /* ±0.2C */
    if (g_vehicle.temp_x10 < 150) g_vehicle.temp_x10 = 150;   /* 15.0C 下限 */
    if (g_vehicle.temp_x10 > 450) g_vehicle.temp_x10 = 450;   /* 45.0C 上限 */
}

void UpdateAcc(void)
{
    g_vehicle.acc_x = g_vehicle.acc_x * 9 / 10 + (rand() % 21 - 10);   /* 回中 + ±0.10g 抖动 */
    g_vehicle.acc_y = g_vehicle.acc_y * 9 / 10 + (rand() % 21 - 10);
    g_vehicle.acc_z = g_vehicle.acc_z * 9 / 10 + (rand() % 21 - 10);
}

/* 一次全更新(Day② 会拆成三档分别调用) */
void Sensor_Update(void)
{
    UpdateGps();
    UpdateAcc();
    UpdateTemp();
}

// ==================== 接收解析函数 ====================
void CAN_ProcessRx(CAN_RxHeaderTypeDef *rx_header, uint8_t *rx_data)
{
#if RX_PRINT_FULL
    /* ---- 完整版：一行约 60 字符 ≈ 5ms @115200 ---- */
    printf("[RX] ID=0x%03X DLC=%d Data: ", rx_header->StdId, rx_header->DLC);
    for (int i = 0; i < rx_header->DLC; i++) printf("%02X ", rx_data[i]);
    printf(" -> ");
    switch (rx_header->StdId)
    {
        case 0x100: printf("lat = %.6f\r\n", UnpackFloatLE(rx_data));          break;
        case 0x101: printf("lon = %.6f\r\n", UnpackFloatLE(rx_data));          break;
        case 0x102: printf("speed = %d\r\n", UnpackInt16BE(rx_data));          break;
        case 0x103: printf("temp = %.1f\r\n", UnpackInt16BE(rx_data) / 10.0f); break;
        case 0x200:                        /* ★ 命令帧：PCAN 发 0x200 / 0x01 请求数据 */
            if (rx_header->DLC >= 1 && rx_data[0] == 0x01) {
                printf("cmd 0x200/01 -> reply latest data\r\n");
                CAN_SendLatitude();
                CAN_SendLongitude();
                CAN_SendSpeed();
                CAN_SendTempX10();
            } else {
                printf("unknown command\r\n");
            }
            break;
        default:    printf("unknown ID\r\n");                                  break;
    }
#else
    /* ---- 精简版：一行约 25 字符 ≈ 2.2ms，做 A/B 对照用 ---- */
    printf("[RX] 0x%03X L=%d -> ", rx_header->StdId, rx_header->DLC);
    switch (rx_header->StdId)
    {
        case 0x100: printf("lat=%.6f\r\n", UnpackFloatLE(rx_data));           break;
        case 0x101: printf("lon=%.6f\r\n", UnpackFloatLE(rx_data));           break;
        case 0x102: printf("spd=%d\r\n", UnpackInt16BE(rx_data));             break;
        case 0x103: printf("tmp=%.1f\r\n", UnpackInt16BE(rx_data) / 10.0f);   break;
        case 0x200:                        /* ★ 命令帧：PCAN 发 0x200 / 0x01 请求数据 */
            if (rx_header->DLC >= 1 && rx_data[0] == 0x01) {
                printf("cmd 0x200/01 -> reply latest data\r\n");
                CAN_SendLatitude(g_lat);
                CAN_SendLongitude(g_lon);
                CAN_SendSpeed(g_spd);
                CAN_SendTemperature(g_tmp);
            } else {
                printf("unknown command\r\n");
            }
            break;
        default:    printf("unknown\r\n");                                     break;
    }
#endif
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

// 小端打包 int16（仅作对照，协议里 int16 一律用大端）
uint8_t PackInt16LE(int16_t v, uint8_t *out)
{
    out[0] = (uint8_t)(v & 0xFF);
    out[1] = (uint8_t)((v >> 8) & 0xFF);
    return 2;
}

// 大端打包 int16（协议在用）
uint8_t PackInt16BE(int16_t v, uint8_t *out)
{
    uint16_t u = (uint16_t)v;       /* 显式无符号，不依赖「负数右移」的实现定义：-50 -> 0xFFCE */
    out[0] = (uint8_t)((u >> 8) & 0xFF);
    out[1] = (uint8_t)(u & 0xFF);
    return 2;
}

float UnpackFloatLE(const uint8_t *in)
{
    union { float f; uint8_t b[4]; } u;
    u.b[0] = in[0]; u.b[1] = in[1];
    u.b[2] = in[2]; u.b[3] = in[3];
    return u.f;
}

// 故意用大端解包, 用于对比错误字节序（教学用）
float UnpackFloatLE_WRONG(const uint8_t *in)
{
    float v; uint8_t tmp[4] = { in[3], in[2], in[1], in[0] };
    memcpy(&v, tmp, 4);
    return v;
}

// 小端解包 int16（仅作对照）
int16_t UnpackInt16LE(const uint8_t *in)
{
    return (int16_t)((in[1] << 8) | in[0]);
}

// 大端解包 int16（协议在用）
int16_t UnpackInt16BE(const uint8_t *in)
{
    return (int16_t)((in[0] << 8) | in[1]);
}

void TestPackUnpack(void)
{
    uint8_t buf[8];
    float  f_in = 23.123456f, f_out;
    int16_t i_in = -50, i_out;
    ShowFloatBytes("100.0f", 100.0f);       /* 应打印 00 00 C8 42 */

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
