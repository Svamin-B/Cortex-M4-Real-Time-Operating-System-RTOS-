extern void runFirstThread(void);
/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include<stdio.h>
#include "kernel.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
uint32_t* stackptr;
static uint32_t magic_number = 0xBA5EBA11;
int __io_putchar(int ch)
{
HAL_UART_Transmit(&huart2,(uint8_t*)&ch,1,HAL_MAX_DELAY);
return ch;
}



void print_success(void)
{
	__asm("SVC #17");
}

void print_pass(void)
{
	__asm("SVC #18");
}

void print_fail(void)
{
	__asm("SVC #19");
}

void thread1(void)
{
	__asm("SVC #10");
}

//void print_continuously(void *arg)
//{
//	(void)arg;
//	while(1)
//	{
//		printf("Hello, PC!\r\n");
//
//	}
//}

void first_thread(void *arg)
{
	uint32_t val = *(uint32_t *)arg;
    while (1) {
        printf("Thread 1 - Value: 0x%08lX\r\n", val);
        for(int i = 0; i < 20002; i++){}
        osYield();
    }
}

void second_thread(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2\r\n");
        for(int i = 0; i < 30002; i++){}
        osYield();
    }
}


void jumpAssembly(void* fcn)
{
	__asm("MOV PC, R0");
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */

void yielding_thread(void *ignored) {
    while (1) {
        printf("Yielding Thread\r\n");
        for (volatile uint32_t i = 0; i < 50000; ++i) {}
        osYield();
    }
}

typedef struct { uint32_t a, b; } math_t;
void preemptive_thread(void *arg) {
    math_t inputs = *(math_t *)arg;
    static uint32_t result = 0;
    while (1) {
        result = result * inputs.a + inputs.b;
        printf("Pre-emptive thread result=%lu\r\n", result);
        for (volatile uint32_t i = 0; i < 50000; ++i) {}
    }
}

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  osKernelInitialize();

//  if (!osCreateThread(first_thread, &magic_number) ||
//      !osCreateThread(second_thread, NULL))
//  {
//      printf("ran out of stack space\r\n");
//      while (1);
//  }

  math_t nums = { 7, 3 };
  osCreateThread(yielding_thread, NULL);             // gets default 5 ms slice
  osCreateThreadWithDeadline(preemptive_thread, &nums, 10); // 2 ms slice
  osKernelStart();
  /* USER CODE BEGIN 2 */
//  char m = 'm';


//  printf("Hello, world!\r\n"); //\r\n means newline and return. It must be in all print statements
//  print_success();
//  print_pass();
//  print_fail();
//
//  uint32_t* MSP_INIT_VAL = *(uint32_t**)0x0;
//  printf("MSP Init is: %p\n\r", MSP_INIT_VAL); //note the %p to print a pointer. It will be in hex
//  uint32_t PSP_val = (uint32_t)MSP_INIT_VAL - 0x400;
//
//  stackptr=(uint32_t*) PSP_val;
//
//  *(--stackptr) = 1<<24; //A magic number, this is xPSR
//  *(--stackptr) = (uint32_t)print_continuously; //the function name
//  *(--stackptr) = 0xA; //An arbitrary number
//  *(--stackptr) = 0xA; //An arbitrary number
//  *(--stackptr) = 0xA; //An arbitrary number
//  *(--stackptr) = 0xA; //An arbitrary number
//  *(--stackptr) = 0xA; //An arbitrary number
//  *(--stackptr) = 0xA; //An arbitrary number
//  *(--stackptr) = 0xA; //An arbitrary number
//  *(--stackptr) = 0xA; //An arbitrary number
//  *(--stackptr) = 0xA; //An arbitrary number
//  *(--stackptr) = 0xA; //An arbitrary number
//  *(--stackptr) = 0xA; //An arbitrary number
//  *(--stackptr) = 0xA; //An arbitrary number
//  *(--stackptr) = 0xA; //An arbitrary number
//  *(--stackptr) = 0xA; //An arbitrary number
//
//  thread1();


//  print_continuously();
//  (void*)print_continuously;

    /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

//  uint32_t PSP_val = (uint32_t)MSP_INIT_VAL - 0x400;
//  __set_PSP(PSP_val);
//  __set_CONTROL(2);
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
