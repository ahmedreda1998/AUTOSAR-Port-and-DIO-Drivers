 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.h
 *
 * Description: Header file for TM4C123GH6PM Microcontroller - Port Driver.
 *
 * Author: Ahmed Reda Mohamed Mahmoud
 ******************************************************************************/

#ifndef PORT_H
#define PORT_H
/* Id for the company in the AUTOSAR
 * for example Mohamed Tarek's ID = 1000 :) */
#define PORT_VENDOR_ID    (1000U)

/* PORT Module Id */
#define PORT_MODULE_ID    (120U)

/* PORT Instance Id */
#define PORT_INSTANCE_ID  (0U)

/*
 * Module Version 1.0.0
 */
#define PORT_SW_MAJOR_VERSION           (1U)
#define PORT_SW_MINOR_VERSION           (0U)
#define PORT_SW_PATCH_VERSION           (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define PORT_AR_RELEASE_MAJOR_VERSION   (4U)
#define PORT_AR_RELEASE_MINOR_VERSION   (0U)
#define PORT_AR_RELEASE_PATCH_VERSION   (3U)

/*
 * Macros for Dio Status
 */
#define PORT_INITIALIZED                (1U)
#define PORT_NOT_INITIALIZED            (0U)   
   
   
/* Standard AUTOSAR types */
#include "Std_Types.h"

/* AUTOSAR checking between Std Types and PORT Modules */
#if ((STD_TYPES_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Std_Types.h does not match the expected version"
#endif

/* PORT Pre-Compile Configuration Header file */
#include "Port_Cfg.h"

/* AUTOSAR Version checking between PORT_Cfg.h and PORT.h files */
#if ((PORT_CFG_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 ||  (PORT_CFG_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 ||  (PORT_CFG_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of PORT_Cfg.h does not match the expected version"
#endif

/* Software Version checking between PORT_Cfg.h and PORT.h files */
#if ((PORT_CFG_SW_MAJOR_VERSION != PORT_SW_MAJOR_VERSION)\
 ||  (PORT_CFG_SW_MINOR_VERSION != PORT_SW_MINOR_VERSION)\
 ||  (PORT_CFG_SW_PATCH_VERSION != PORT_SW_PATCH_VERSION))
  #error "The SW version of PORT_Cfg.h does not match the expected version"
#endif
   
/* Non AUTOSAR files */
#include "Common_Macros.h"
/*******************************************************************************
 *                      DET Error Codes                                        *
 *******************************************************************************/
/* DET code to report Invalid Pin */
#define PORT_E_PARAM_PIN                         (uint8)0x0A
/* DET code to report not configured Port Pin as changeable */
#define PORT_E_DIRECTION_UNCHANGEABLE            (uint8)0x0B
/* DET code to report that the Port Driver is called with wrong parameters */
#define PORT_E_PARAM_CONFIG                      (uint8)0x0C
/* DET code to report that the mode is invalid */
#define PORT_E_PARAM_INVALID_MODE                (uint8)0x0D
/* DET code to report that the mode is unchangeable */
#define PORT_E_MODE_UNCHANGEABLE                 (uint8)0x0E   
/* DET code to report that the Config. Ptr is Null*/ 
#define PORT_E_PARAM_POINTER                     (uint8)0x10
/* DET code to report that the Port driver wasn't initialized */
#define PORT_E_UNINIT                            (uint8)0x0F
/*
 * The API service shall return immediately without any further action,
 * beside reporting this development error.
 */
    
/*******************************************************************************
 *                              Module Data Types                              *
 *******************************************************************************/
typedef uint8 Port_Type;               /* Number of Port */
typedef uint16 Port_PinType;             /* Number of pin */
typedef uint8 Port_PinModeType;         /* Mode of pin */
typedef uint8 Port_Pinlevel;            /* Initial value of pin */
typedef uint8 PortPinModeChangeable;
typedef uint8 PortPinDirectionChangeable;
/* Description: Enum to hold PIN direction */
typedef enum
{
    PORT_PIN_IN,
    PORT_PIN_OUT
}Port_PinDirectionType;

/* Description: Enum to hold internal resistor type for PIN */
typedef enum
{
    OFF,PULL_UP,PULL_DOWN
}Port_InternalResistor;

/* Description: Structure to configure each individual PIN:
 *	1. the PORT Which the pin belongs to. 0, 1, 2, 3, 4 or 5
 *	2. the number of the pin in the PORT.
 *      3. the direction of pin --> INPUT or OUTPUT
 *      4. Pin Mode (ex. GPIO, ADC, etc.)
 *      5. the internal resistor --> Disable, Pull up or Pull down
 *      6. pin initial value -->  STD_HIGH, STD_LOW
 *      7. Pin direction changeable during runtime
 *      8. Pin mode changeable during runtime
 */
typedef struct 
{
    Port_Type    port_num;                /*  Port Number  */
    Port_PinType pin_num;                /*  pin Number of the port  */     
    Port_PinDirectionType direction;     /*  Direction of pin  */
    #if (PORT_SET_PIN_DIRECTION_API == STD_ON)
    PortPinDirectionChangeable Pin_directionconfig;   /*  Pin Direction to be changed to during runtime  */
#endif
    Port_InternalResistor resistor;     /*  InternaL resistor status if the direction is input */
     Port_Pinlevel initial_value;       /* Initial value if the direction is output */
    Port_PinModeType pin_mode;           /*  Pin Mode (ex. GPIO, ADC, etc.)  */
#if (PORT_SET_PIN_MODE_API == STD_ON) 
    PortPinModeChangeable  Pin_modeconfig;           /*  Pin Mode to be changed to during runtime  */
#endif    
}Port_ConfigPins;

typedef struct Port_ConfigType
{
  Port_ConfigPins  Pins[PORT_CONFIGURED_PINS];      
}Port_ConfigType;

/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/

/************************************************************************************
* Service Name: Port_Init
* Sync/Async: Synchronous
* Reentrancy: reentrant
* Parameters (in): ConfigPtr - Pointer to post-build configuration data
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to initialize the Port Driver module.
************************************************************************************/
void Port_Init( const Port_ConfigType* ConfigPtr );
/************************************************************************************
* Service Name: Port_SetPinDirection
* Sync/Async: Synchronous
* Reentrancy: reentrant
* Parameters (in): Pin - Port Pin ID number
*                  Direction - Port Pin Direction
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to set the port pin direction
************************************************************************************/
#if (PORT_SET_PIN_DIRECTION_API == STD_ON)
void Port_SetPinDirection( Port_PinType Pin, Port_PinDirectionType Direction );
#endif
/************************************************************************************
* Service Name: Port_RefreshPortDirection
* Sync/Async: Synchronous
* Reentrancy: Non Reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to refresh port direction.
************************************************************************************/
void Port_RefreshPortDirection(void);
/************************************************************************************
* Service Name: Port_GetVersionInfo
* Sync/Async: Synchronous
* Reentrancy: Non Reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): versioninfo - Pointer to where to store the version information of this module
* Return value: None
* Description: Function to return the version information of this module.
************************************************************************************/
#if (PORT_VERSION_INFO_API == STD_ON)
void Port_GetVersionInfo( Std_VersionInfoType* versioninfo);
#endif
/************************************************************************************
* Service Name: Port_SetPinMode
* Sync/Async: Synchronous
* Reentrancy: Reentrant
* Parameters (in): Pin - Port Pin ID number
*                  Direction - New Port Pin mode to be set on port pin.
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to Set the port pin mode.
************************************************************************************/
#if (PORT_SET_PIN_MODE_API == STD_ON)
void Port_SetPinMode(Port_PinType Pin, Port_PinModeType Mode);
#endif
/*******************************************************************************
 *                      External Variables                                   *
 *******************************************************************************/
extern const Port_ConfigType Port_Configuration;



#endif /* PORT_H */
