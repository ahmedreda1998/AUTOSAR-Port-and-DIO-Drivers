 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port_Cfg.h
 *
 * Description: Pre-Compile Configuration Header file for TM4C123GH6PM Microcontroller - Port Driver
 *
 * Author: Ahmed Reda Mohamed Mahmoud
 ******************************************************************************/

#ifndef PORT_CFG_H
#define PORT_CFG_H

/*
 * Module Version 1.0.0
 */
#define PORT_CFG_SW_MAJOR_VERSION              (1U)
#define PORT_CFG_SW_MINOR_VERSION              (0U)
#define PORT_CFG_SW_PATCH_VERSION              (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define PORT_CFG_AR_RELEASE_MAJOR_VERSION     (4U)
#define PORT_CFG_AR_RELEASE_MINOR_VERSION     (0U)
#define PORT_CFG_AR_RELEASE_PATCH_VERSION     (3U)

/******************************************************************************
 *                      API Service Id Macros                                 *
 ******************************************************************************/
/*  the number of Pins in a specific Port */
#define PORT_CONFIGURED_PINS            (uint16)43

/* The Development Error is on or off */
#define PORT_DEV_ERROR_DETECT           (STD_OFF)

/* Service ID for Port direction to be Changeable or not*/
#define PORT_SET_PIN_DIRECTION_API      (STD_ON)

/* Service ID for Port mode to be Changeable or not*/
#define PORT_SET_PIN_MODE_API           (STD_ON)

/* Service ID for Port write Port */
#define PORT_VERSION_INFO_API           (STD_ON)

/* Service ID for Port GetVersionInfo */
#define PORT_GET_VERSION_INFO_SID       (uint8)0x03

/* Service ID for Port Init Channel */
#define PORT_INIT_SID                   (uint8)0x00             

/* Service ID for Port  Set Pin Direction */
#define PORT_SetPinDirection_SID        (uint8)0x01

/* Service ID for Port  Refresh Direction */
#define PORT_RefreshPortDirection_SID   (uint8)0x02

/* Service ID for Setting new Pin Mode  */
#define PORT_SetPinMode_SID             (uint8)0x04
/*******************************************************************************
 *                              Module Definitions                             *
 *******************************************************************************/

/* Ports Registers base addresses */
#define PORTA_BASE_ADDRESS           0x40004000
#define PORTB_BASE_ADDRESS           0x40005000
#define PORTC_BASE_ADDRESS           0x40006000
#define PORTD_BASE_ADDRESS           0x40007000
#define PORTE_BASE_ADDRESS           0x40024000
#define PORTF_BASE_ADDRESS           0x40025000

/* Ports Registers offset addresses */
#define PORT_DATA_REG_OFFSET              0x3FC
#define PORT_DIR_REG_OFFSET               0x400
#define PORT_ALT_FUNC_REG_OFFSET          0x420
#define PORT_PULL_UP_REG_OFFSET           0x510
#define PORT_PULL_DOWN_REG_OFFSET         0x514
#define PORT_DIGITAL_ENABLE_REG_OFFSET    0x51C
#define PORT_LOCK_REG_OFFSET              0x520
#define PORT_COMMIT_REG_OFFSET            0x524
#define PORT_ANALOG_MODE_SEL_REG_OFFSET   0x528
#define PORT_CTL_REG_OFFSET               0x52C

/* Pin Modes */
#define PORT_PIN_MODE_GPIO           (uint8)0 
#define PORT_PIN_MODE_ADC            (uint8)1
#define PORT_PIN_MODE_ANALOG_COMP    (uint8)2
#define PORT_PIN_MODE_CAN            (uint8)3 
#define PORT_PIN_MODE_CORE           (uint8)4
#define PORT_PIN_MODE_GPT            (uint8)5
#define PORT_PIN_MODE_I2C            (uint8)6
#define PORT_PIN_MODE_PWM            (uint8)7
#define PORT_PIN_MODE_QEI            (uint8)8
#define PORT_PIN_MODE_SSI            (uint8)9
#define PORT_PIN_MODE_NMI            (uint8)10
#define PORT_PIN_MODE_UART           (uint8)11
#define PORT_PIN_MODE_USB            (uint8)12

/*PORT INDEXS*/  
   
#define PORT_A     (0U)
#define PORT_B     (1U)
#define PORT_C     (2U)
#define PORT_D     (3U)
#define PORT_E     (4U)
#define PORT_F     (5U) 
   
   /*PIN INDEXS*/    
#define PORT_PIN0 				(uint8)0
#define PORT_PIN1 				(uint8)1
#define PORT_PIN2 				(uint8)2
#define PORT_PIN3 				(uint8)3
#define PORT_PIN4 				(uint8)4
#define PORT_PIN5 				(uint8)5
#define PORT_PIN6 				(uint8)6
#define PORT_PIN7 				(uint8)7   
   
   
   
   
   /*Port Pin ID number */
   /*Used as a parameters to SetPin_Direction();*/
#define PORT_A_PIN_0    (Port_PinType)0
#define PORT_A_PIN_1    (Port_PinType)1
#define PORT_A_PIN_2    (Port_PinType)2
#define PORT_A_PIN_3    (Port_PinType)3
#define PORT_A_PIN_4    (Port_PinType)4
#define PORT_A_PIN_5    (Port_PinType)5
#define PORT_A_PIN_6    (Port_PinType)6
#define PORT_A_PIN_7    (Port_PinType)7
   
#define PORT_B_PIN_0    (Port_PinType)8
#define PORT_B_PIN_1    (Port_PinType)9
#define PORT_B_PIN_2    (Port_PinType)10 
#define PORT_B_PIN_3    (Port_PinType)11 
#define PORT_B_PIN_4    (Port_PinType)12 
#define PORT_B_PIN_5    (Port_PinType)13 
#define PORT_B_PIN_6    (Port_PinType)14
#define PORT_B_PIN_7    (Port_PinType)15
   
#define PORT_C_PIN_0    (Port_PinType)16
#define PORT_C_PIN_1    (Port_PinType)17 
#define PORT_C_PIN_2    (Port_PinType)18
#define PORT_C_PIN_3    (Port_PinType)19
#define PORT_C_PIN_4    (Port_PinType)20
#define PORT_C_PIN_5    (Port_PinType)21
#define PORT_C_PIN_6    (Port_PinType)22 
#define PORT_C_PIN_7    (Port_PinType)23
   
#define PORT_D_PIN_0    (Port_PinType)24
#define PORT_D_PIN_1    (Port_PinType)25   
#define PORT_D_PIN_2    (Port_PinType)26   
#define PORT_D_PIN_3    (Port_PinType)27   
#define PORT_D_PIN_4    (Port_PinType)28   
#define PORT_D_PIN_5    (Port_PinType)29   
#define PORT_D_PIN_6    (Port_PinType)30   
#define PORT_D_PIN_7    (Port_PinType)31
   
#define PORT_E_PIN_0    (Port_PinType)32
#define PORT_E_PIN_1    (Port_PinType)33
#define PORT_E_PIN_2    (Port_PinType)34
#define PORT_E_PIN_3    (Port_PinType)35
#define PORT_E_PIN_4    (Port_PinType)36
#define PORT_E_PIN_5    (Port_PinType)37
   
#define PORT_F_PIN_0    (Port_PinType)38
#define PORT_F_PIN_1    (Port_PinType)39
#define PORT_F_PIN_2    (Port_PinType)40
#define PORT_F_PIN_3    (Port_PinType)41
#define PORT_F_PIN_4    (Port_PinType)42   
   
   
  /* Pin changable direction. RunTime */
#define PORT_A_PIN_0_DIR_CHANG         STD_OFF
#define PORT_A_PIN_1_DIR_CHANG         STD_OFF
#define PORT_A_PIN_2_DIR_CHANG         STD_OFF
#define PORT_A_PIN_3_DIR_CHANG         STD_OFF
#define PORT_A_PIN_4_DIR_CHANG         STD_OFF
#define PORT_A_PIN_5_DIR_CHANG         STD_OFF
#define PORT_A_PIN_6_DIR_CHANG         STD_OFF
#define PORT_A_PIN_7_DIR_CHANG         STD_OFF

#define PORT_B_PIN_0_DIR_CHANG         STD_OFF
#define PORT_B_PIN_1_DIR_CHANG         STD_OFF
#define PORT_B_PIN_2_DIR_CHANG         STD_OFF
#define PORT_B_PIN_3_DIR_CHANG         STD_OFF
#define PORT_B_PIN_4_DIR_CHANG         STD_OFF
#define PORT_B_PIN_5_DIR_CHANG         STD_OFF
#define PORT_B_PIN_6_DIR_CHANG         STD_OFF
#define PORT_B_PIN_7_DIR_CHANG         STD_OFF

#define PORT_C_PIN_0_DIR_CHANG         STD_OFF
#define PORT_C_PIN_1_DIR_CHANG         STD_OFF
#define PORT_C_PIN_2_DIR_CHANG         STD_OFF
#define PORT_C_PIN_3_DIR_CHANG         STD_OFF
#define PORT_C_PIN_4_DIR_CHANG         STD_OFF
#define PORT_C_PIN_5_DIR_CHANG         STD_OFF 
#define PORT_C_PIN_6_DIR_CHANG         STD_OFF
#define PORT_C_PIN_7_DIR_CHANG         STD_OFF

#define PORT_D_PIN_0_DIR_CHANG         STD_OFF
#define PORT_D_PIN_1_DIR_CHANG         STD_OFF
#define PORT_D_PIN_2_DIR_CHANG         STD_OFF 
#define PORT_D_PIN_3_DIR_CHANG         STD_OFF
#define PORT_D_PIN_4_DIR_CHANG         STD_OFF
#define PORT_D_PIN_5_DIR_CHANG         STD_OFF
#define PORT_D_PIN_6_DIR_CHANG         STD_OFF
#define PORT_D_PIN_7_DIR_CHANG         STD_OFF 

#define PORT_E_PIN_0_DIR_CHANG         STD_OFF
#define PORT_E_PIN_1_DIR_CHANG         STD_OFF
#define PORT_E_PIN_2_DIR_CHANG         STD_OFF 
#define PORT_E_PIN_3_DIR_CHANG         STD_OFF
#define PORT_E_PIN_4_DIR_CHANG         STD_OFF
#define PORT_E_PIN_5_DIR_CHANG         STD_OFF   

#define PORT_F_PIN_0_DIR_CHANG         STD_OFF
#define PORT_F_PIN_1_DIR_CHANG         STD_OFF
#define PORT_F_PIN_2_DIR_CHANG         STD_OFF 
#define PORT_F_PIN_3_DIR_CHANG         STD_OFF
#define PORT_F_PIN_4_DIR_CHANG         STD_OFF   

   /* Pin changable mode. RunTime */
#define PORT_A_PIN_0_DIR_MODE         STD_OFF
#define PORT_A_PIN_1_DIR_MODE         STD_OFF
#define PORT_A_PIN_2_DIR_MODE         STD_OFF
#define PORT_A_PIN_3_DIR_MODE         STD_OFF
#define PORT_A_PIN_4_DIR_MODE         STD_OFF
#define PORT_A_PIN_5_DIR_MODE         STD_OFF
#define PORT_A_PIN_6_DIR_MODE         STD_OFF
#define PORT_A_PIN_7_DIR_MODE         STD_OFF

#define PORT_B_PIN_0_DIR_MODE         STD_OFF
#define PORT_B_PIN_1_DIR_MODE         STD_OFF
#define PORT_B_PIN_2_DIR_MODE         STD_OFF
#define PORT_B_PIN_3_DIR_MODE         STD_OFF
#define PORT_B_PIN_4_DIR_MODE         STD_OFF
#define PORT_B_PIN_5_DIR_MODE         STD_OFF
#define PORT_B_PIN_6_DIR_MODE         STD_OFF
#define PORT_B_PIN_7_DIR_MODE         STD_OFF

#define PORT_C_PIN_0_DIR_MODE         STD_OFF
#define PORT_C_PIN_1_DIR_MODE         STD_OFF
#define PORT_C_PIN_2_DIR_MODE         STD_OFF
#define PORT_C_PIN_3_DIR_MODE         STD_OFF
#define PORT_C_PIN_4_DIR_MODE         STD_OFF
#define PORT_C_PIN_5_DIR_MODE         STD_OFF 
#define PORT_C_PIN_6_DIR_MODE         STD_OFF
#define PORT_C_PIN_7_DIR_MODE         STD_OFF

#define PORT_D_PIN_0_DIR_MODE         STD_OFF
#define PORT_D_PIN_1_DIR_MODE         STD_OFF
#define PORT_D_PIN_2_DIR_MODE         STD_OFF 
#define PORT_D_PIN_3_DIR_MODE         STD_OFF
#define PORT_D_PIN_4_DIR_MODE         STD_OFF
#define PORT_D_PIN_5_DIR_MODE         STD_OFF
#define PORT_D_PIN_6_DIR_MODE         STD_OFF
#define PORT_D_PIN_7_DIR_MODE         STD_OFF 

#define PORT_E_PIN_0_DIR_MODE         STD_OFF
#define PORT_E_PIN_1_DIR_MODE         STD_OFF
#define PORT_E_PIN_2_DIR_MODE         STD_OFF 
#define PORT_E_PIN_3_DIR_MODE         STD_OFF
#define PORT_E_PIN_4_DIR_MODE         STD_OFF
#define PORT_E_PIN_5_DIR_MODE         STD_OFF   

#define PORT_F_PIN_0_DIR_MODE         STD_OFF
#define PORT_F_PIN_1_DIR_MODE         STD_OFF
#define PORT_F_PIN_2_DIR_MODE         STD_OFF 
#define PORT_F_PIN_3_DIR_MODE         STD_OFF
#define PORT_F_PIN_4_DIR_MODE         STD_OFF   
      
 

 #endif /* PORT_CFG_H */

