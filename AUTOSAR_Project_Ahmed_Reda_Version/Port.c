 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.c
 *
 * Description: Source file for TM4C123GH6PM Microcontroller - Port Driver.
 *
 * Author: Ahmed Reda Mohamed Mahmoud
 ******************************************************************************/
#include "Port.h"

#if (PORT_DEV_ERROR_DETECT == STD_ON)

#include "Det.h"
/* AUTOSAR Version checking between Det and Dio Modules */
#if ((DET_AR_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 || (DET_AR_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 || (DET_AR_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Det.h does not match the expected version"
#endif
#endif
#include "tm4c123gh6pm_registers.h"

/************************************************************************************
* Service Name: Port_Init
* Service ID[hex]: 0x00
* Sync/Async: Synchronous
* Reentrancy: reentrant
* Parameters (in): ConfigPtr - Pointer to post-build configuration data
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to initialize the Port Driver module.
************************************************************************************/
STATIC  const Port_ConfigPins * PortConf = NULL_PTR;
STATIC volatile uint8 Port_Status = PORT_NOT_INITIALIZED;

void Port_Init(const Port_ConfigType * ConfigPtr )
{
#if (DIO_DEV_ERROR_DETECT == STD_ON) 
  /* check if the input configuration pointer is not a NULL_PTR */
	if (NULL_PTR == ConfigPtr)
	{
		Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_SID,
		     PORT_E_PARAM_CONFIG);
	}
  else
#endif    
  {
    Port_Status = PORT_INITIALIZED;
    PortConf = ConfigPtr ->Pins;
    volatile uint32 * Port_Ptr = NULL_PTR; /* point to the required Port Registers base address */
    volatile uint32 delay = 0;
    uint8 Pin_Index;
    for (Pin_Index=0;Pin_Index<PORT_CONFIGURED_PINS;Pin_Index++)
    {
    switch(PortConf[Pin_Index].port_num)
    {
        case  0: Port_Ptr= (volatile uint32 *)PORTA_BASE_ADDRESS; /* PORTA Base Address */
		 break;
	case  1: Port_Ptr = (volatile uint32 *)PORTB_BASE_ADDRESS; /* PORTB Base Address */
		 break;
	case  2: Port_Ptr= (volatile uint32 *)PORTC_BASE_ADDRESS; /* PORTC Base Address */
		 break;
	case  3: Port_Ptr= (volatile uint32 *)PORTD_BASE_ADDRESS; /* PORTD Base Address */
		 break;
        case  4: Port_Ptr = (volatile uint32 *)PORTE_BASE_ADDRESS; /* PORTE Base Address */
		 break;
        case  5: Port_Ptr = (volatile uint32 *)PORTF_BASE_ADDRESS; /* PORTF Base Address */
		 break;
    }
    
    /* Enable clock for PORT and allow time for clock to start*/
    SYSCTL_REGCGC2_REG |= (1<<PortConf[Pin_Index].port_num);
    delay = SYSCTL_REGCGC2_REG;
    
    if( ((PortConf[Pin_Index].port_num == 3) && (PortConf[Pin_Index].pin_num == 7)) || ((PortConf[Pin_Index].port_num == 5) && (PortConf[Pin_Index].pin_num == 0)) ) /* PD7 or PF0 */
    {
        *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_LOCK_REG_OFFSET) = 0x4C4F434B;                     /* Unlock the GPIOCR register */   
        SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_COMMIT_REG_OFFSET) , PortConf[Pin_Index].pin_num);  /* Set the corresponding bit in GPIOCR register to allow changes on this pin */
    }
    else if( (PortConf[Pin_Index].port_num == 2) && (PortConf[Pin_Index].pin_num <= 3) ) /* PC0 to PC3 */
    {
        /* Do Nothing ...  this is the JTAG pins */
    }
    else
    {
        /* Do Nothing ... No need to unlock the commit register for this pin */
    }
        if(PortConf[Pin_Index].direction == PORT_PIN_OUT)
    {
	SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , PortConf[Pin_Index].pin_num);                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
        
        if(PortConf[Pin_Index].initial_value == STD_HIGH)
        {
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , PortConf[Pin_Index].pin_num);          /* Set the corresponding bit in the GPIODATA register to provide initial value 1 */
        }
        else
        {
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , PortConf[Pin_Index].pin_num);        /* Clear the corresponding bit in the GPIODATA register to provide initial value 0 */
        }
    }
    else if(PortConf[Pin_Index].direction == PORT_PIN_IN)
    {
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , PortConf[Pin_Index].pin_num);             /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
        
        if(PortConf[Pin_Index].resistor == PULL_UP)
        {
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , PortConf[Pin_Index].pin_num);       /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
        }
        else if(PortConf[Pin_Index].resistor == PULL_DOWN)
        {
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , PortConf[Pin_Index].pin_num);     /* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
        }
        else
        {
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , PortConf[Pin_Index].pin_num);     /* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , PortConf[Pin_Index].pin_num);   /* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */
        }
    }
    else
    {
        /* Do Nothing */
    }
    if (PortConf[Pin_Index].pin_mode == PORT_PIN_MODE_GPIO )    /* GPIO */
    {
    
    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConf[Pin_Index].pin_num);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConf[Pin_Index].pin_num);             /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
    *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (PortConf[Pin_Index].pin_num * 4));     /* Clear the PMCx bits for this pin */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConf[Pin_Index].pin_num);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
   }
   else if (PortConf[Pin_Index].pin_mode == PORT_PIN_MODE_ADC)
   {
      SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConf[Pin_Index].pin_num);      /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */      
      CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , PortConf[Pin_Index].pin_num);             /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
      SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConf[Pin_Index].pin_num);   /* Set the corresponding bit in the GPIOAMSEL register to enable analog functionality on this pin */
    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConf[Pin_Index].pin_num);        /* Clear the corresponding bit in the GPIODEN register to disable digital functionality on this pin */
   }
    else if (PortConf[Pin_Index].pin_mode == PORT_PIN_MODE_ANALOG_COMP)
  {
     /*  There are two types of pins inputs (Analog) and outputs (Digital) in the analog compartor*/
     SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConf[Pin_Index].pin_num);      /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */      
     CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , PortConf[Pin_Index].pin_num);             /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
     if(PortConf[Pin_Index].direction == PORT_PIN_OUT)
    {
      *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |=(0x00000009 << (PortConf[Pin_Index].pin_num * 4));
	SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , PortConf[Pin_Index].pin_num);                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
        
        if(PortConf[Pin_Index].initial_value == STD_HIGH)
        {
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , PortConf[Pin_Index].pin_num);          /* Set the corresponding bit in the GPIODATA register to provide initial value 1 */
        }
        else
        {
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , PortConf[Pin_Index].pin_num);        /* Clear the corresponding bit in the GPIODATA register to provide initial value 0 */
        }
       SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConf[Pin_Index].pin_num);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */  
    }
    else if(PortConf[Pin_Index].direction == PORT_PIN_IN)
    {
         SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConf[Pin_Index].pin_num);   /* Set the corresponding bit in the GPIOAMSEL register to enable analog functionality on this pin */
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , PortConf[Pin_Index].pin_num);             /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
        
        if(PortConf[Pin_Index].resistor == PULL_UP)
        {
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , PortConf[Pin_Index].pin_num);       /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
        }
        else if(PortConf[Pin_Index].resistor == PULL_DOWN)
        {
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , PortConf[Pin_Index].pin_num);     /* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
        }
        else
        {
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , PortConf[Pin_Index].pin_num);     /* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , PortConf[Pin_Index].pin_num);   /* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */
        }
       CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConf[Pin_Index].pin_num);  /* Clear the corresponding bit in the GPIODEN register to disable digital functionality on this pin */   
    }
    else
    {
        /* Do Nothing */
    }
    
   } 
   
   else if (PortConf[Pin_Index].pin_mode == PORT_PIN_MODE_CAN)
   {
      
    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConf[Pin_Index].pin_num);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConf[Pin_Index].pin_num);             /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */ 
    if ((PortConf[Pin_Index].pin_num == 17 ) || (PortConf[Pin_Index].pin_num == 18 ) || (PortConf[Pin_Index].pin_num == 57 )||(PortConf[Pin_Index].pin_num == 58 ) || (PortConf[Pin_Index].pin_num == 59 ) || (PortConf[Pin_Index].pin_num == 60 ))
    {
    
    *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000008 << (PortConf[Pin_Index].pin_num * 4));     /* Clear the PMCx bits for this pin */
    }
    else if ((PortConf[Pin_Index].pin_num == 17 ) || (PortConf[Pin_Index].pin_num == 18 ))
    {
      *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000003 << (PortConf[Pin_Index].pin_num * 4));     /* Clear the PMCx bits for this pin */
    }
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConf[Pin_Index].pin_num);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
   }
   else if (PortConf[Pin_Index].pin_mode == PORT_PIN_MODE_CORE)
   {
    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConf[Pin_Index].pin_num);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConf[Pin_Index].pin_num);             /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
    *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x0000000E << (PortConf[Pin_Index].pin_num * 4));     /* Set the PMCx bits for this pin */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConf[Pin_Index].pin_num);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
   }
   else if (PortConf[Pin_Index].pin_mode == PORT_PIN_MODE_GPT)
   {
    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConf[Pin_Index].pin_num);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConf[Pin_Index].pin_num);             /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
    *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000007 << (PortConf[Pin_Index].pin_num * 4));     /* Set the PMCx bits for this pin */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConf[Pin_Index].pin_num);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
   }
   else if (PortConf[Pin_Index].pin_mode == PORT_PIN_MODE_I2C)
   {
     CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConf[Pin_Index].pin_num);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConf[Pin_Index].pin_num);             /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
    *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000003 << (PortConf[Pin_Index].pin_num * 4));     /* Set the PMCx bits for this pin */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConf[Pin_Index].pin_num);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
   }
   else if (PortConf[Pin_Index].pin_mode == PORT_PIN_MODE_PWM)
   {
         CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConf[Pin_Index].pin_num);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConf[Pin_Index].pin_num);             /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
   if((PortConf[Pin_Index].pin_num==12) || (PortConf[Pin_Index].pin_num==13) ||PortConf[Pin_Index].pin_num==14 ||PortConf[Pin_Index].pin_num==15||PortConf[Pin_Index].pin_num==20 ||PortConf[Pin_Index].pin_num==21 ||PortConf[Pin_Index].pin_num==24||PortConf[Pin_Index].pin_num==25|PortConf[Pin_Index].pin_num==36 ||PortConf[Pin_Index].pin_num==37)/*PB4--PB7 , PC4,5  ,PD0,1 ,PE4,5*/
       {
       *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000004 << (PortConf[Pin_Index].pin_num * 4));    /* Set the PMCx bits for this pin */
      }
       else
       {
         *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000005<< (PortConf[Pin_Index].pin_num * 4));   /* Set the PMCx bits for this pin */
       }
     SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConf[Pin_Index].pin_num);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
     
   }
   else if (PortConf[Pin_Index].pin_mode == PORT_PIN_MODE_QEI)
   {
       CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConf[Pin_Index].pin_num);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConf[Pin_Index].pin_num);             /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
    *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000006 << (PortConf[Pin_Index].pin_num * 4));     /* Set the PMCx bits for this pin */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConf[Pin_Index].pin_num);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
   }
   else if (PortConf[Pin_Index].pin_mode == PORT_PIN_MODE_SSI)
   {
           CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConf[Pin_Index].pin_num);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConf[Pin_Index].pin_num);             /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
   if((PortConf[Pin_Index].pin_num)==61 ||(PortConf[Pin_Index].pin_num)==62 ||(PortConf[Pin_Index].pin_num)==63 ||(PortConf[Pin_Index].pin_num)==64)
       {
       *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000001 << (PortConf[Pin_Index].pin_num * 4));    /* Set the PMCx bits for this pin */
      }
       else
       {
         *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000002<< (PortConf[Pin_Index].pin_num * 4));   /* Set the PMCx bits for this pin */
       }
     SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConf[Pin_Index].pin_num);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
   }
   else if (PortConf[Pin_Index].pin_mode == PORT_PIN_MODE_NMI)
   {
      CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConf[Pin_Index].pin_num);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConf[Pin_Index].pin_num);             /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
    *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000008 << (PortConf[Pin_Index].pin_num * 4));     /* Clear the PMCx bits for this pin */
   SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConf[Pin_Index].pin_num);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
   }
   else if (PortConf[Pin_Index].pin_mode == PORT_PIN_MODE_UART)
   {
         CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConf[Pin_Index].pin_num);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConf[Pin_Index].pin_num);             /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
   if((PortConf[Pin_Index].pin_num)==15 ||(PortConf[Pin_Index].pin_num)==16 )
       {
       *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000002 << (PortConf[Pin_Index].pin_num * 4));    /* Set the PMCx bits for this pin */
      }
       else
       {
         *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000001<< (PortConf[Pin_Index].pin_num * 4));   /* Set the PMCx bits for this pin */
       }
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConf[Pin_Index].pin_num);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
   }
   else if (PortConf[Pin_Index].pin_mode == PORT_PIN_MODE_USB)
   {
      /*  There are two types of pins inputs (Analog) and outputs (Digital) in the analog compartor*/
     SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConf[Pin_Index].pin_num);      /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */      
     CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , PortConf[Pin_Index].pin_num);             /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
    if((PortConf[Pin_Index].pin_num)==43 ||(PortConf[Pin_Index].pin_num)==44 || (PortConf[Pin_Index].pin_num)==45 || (PortConf[Pin_Index].pin_num)==46 )
      {
             SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConf[Pin_Index].pin_num);   /* Set the corresponding bit in the GPIOAMSEL register to enable analog functionality on this pin */  
             CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConf[Pin_Index].pin_num);  /* Clear the corresponding bit in the GPIODEN register to disable digital functionality on this pin */ 
      }
      else 
      {
        *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |=(0x00000008 << (PortConf[Pin_Index].pin_num * 4));
        SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConf[Pin_Index].pin_num);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */ 
      }
   }
   else 
   {
     /* Do Nothing */
   }
  }
  }
}
/************************************************************************************
* Service Name: Port_SetPinDirection
* Service ID[hex]: 0x01
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
void Port_SetPinDirection(Port_PinType Pin, Port_PinDirectionType Direction)
{
   volatile uint32 * Port_Ptr = NULL_PTR; /* point to the required Port Registers base address */
   const Port_ConfigPins * Configptr = NULL_PTR;
   const  Port_ConfigType * Configptr2 = NULL_PTR;
    Configptr = Configptr2 -> Pins;
    
#if (DIO_DEV_ERROR_DETECT == STD_ON)
  if (Port_Status == PORT_NOT_INITIALIZED )
  {
    Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SetPinDirection_SID,
		     PORT_E_UNINIT);
  }
  if (Pin > PORT_CONFIGURED_PINS )
	{
		Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SetPinDirection_SID,
		     PORT_E_PARAM_PIN);
	}
  if ( STD_OFF == Configptr[Pin].Pin_directionconfig)
  {
    Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SetPinDirection_SID,
		     PORT_E_DIRECTION_UNCHANGEABLE);
    
  }
  else
#endif    
  {
  
   switch(Configptr[Pin].port_num)
    {
        case  0: Port_Ptr= (volatile uint32 *)PORTA_BASE_ADDRESS; /* PORTA Base Address */
		 break;
	case  1: Port_Ptr = (volatile uint32 *)PORTB_BASE_ADDRESS; /* PORTB Base Address */
		 break;
	case  2: Port_Ptr= (volatile uint32 *)PORTC_BASE_ADDRESS; /* PORTC Base Address */
		 break;
	case  3: Port_Ptr= (volatile uint32 *)PORTD_BASE_ADDRESS; /* PORTD Base Address */
		 break;
        case  4: Port_Ptr = (volatile uint32 *)PORTE_BASE_ADDRESS; /* PORTE Base Address */
		 break;
        case  5: Port_Ptr = (volatile uint32 *)PORTF_BASE_ADDRESS; /* PORTF Base Address */
		 break;
    }  
    if(Direction == PORT_PIN_OUT)
    {
	SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , Pin);                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
    }
    else if(Direction== PORT_PIN_IN)
    {
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , Pin);             /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
    }
    
  } 
}
#endif
/************************************************************************************
* Service Name: Port_RefreshPortDirection
* Service ID[hex]: 0x02
* Sync/Async: Synchronous
* Reentrancy: Non Reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to refresh port direction.
************************************************************************************/
void Port_RefreshPortDirection(void)
{
  #if (DIO_DEV_ERROR_DETECT == STD_ON) 
    if (Port_Status == PORT_NOT_INITIALIZED )
  {
    Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_RefreshPortDirection_SID,
		     PORT_E_UNINIT);
  }
  else 
#endif
  {
    volatile uint32 * Port_Ptr = NULL_PTR; /* point to the required Port Registers base address */   
    uint8 Pin_Index;
    const Port_ConfigPins * Configptr = NULL_PTR;
   const  Port_ConfigType * Configptr2 = NULL_PTR;
    Configptr = Configptr2 -> Pins;
    for (Pin_Index=0;Pin_Index<PORT_CONFIGURED_PINS;Pin_Index++)
    {
    switch(Configptr[Pin_Index].port_num)
    {
        case  0: Port_Ptr= (volatile uint32 *)PORTA_BASE_ADDRESS; /* PORTA Base Address */
		 break;
	case  1: Port_Ptr = (volatile uint32 *)PORTB_BASE_ADDRESS; /* PORTB Base Address */
		 break;
	case  2: Port_Ptr= (volatile uint32 *)PORTC_BASE_ADDRESS; /* PORTC Base Address */
		 break;
	case  3: Port_Ptr= (volatile uint32 *)PORTD_BASE_ADDRESS; /* PORTD Base Address */
		 break;
        case  4: Port_Ptr = (volatile uint32 *)PORTE_BASE_ADDRESS; /* PORTE Base Address */
		 break;
        case  5: Port_Ptr = (volatile uint32 *)PORTF_BASE_ADDRESS; /* PORTF Base Address */
		 break;
    }
    if (Configptr[Pin_Index].Pin_directionconfig == STD_ON)
    {
      continue;
    }
  else
  {
     if(Configptr[Pin_Index].direction == PORT_PIN_OUT)
    {
	SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , Configptr[Pin_Index].pin_num);                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
        
        if(Configptr[Pin_Index].initial_value == STD_HIGH)
        {
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , Configptr[Pin_Index].pin_num);          /* Set the corresponding bit in the GPIODATA register to provide initial value 1 */
        }
        else
        {
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , Configptr[Pin_Index].pin_num);        /* Clear the corresponding bit in the GPIODATA register to provide initial value 0 */
        }
    }
    else if(Configptr[Pin_Index].direction == PORT_PIN_IN)
    {
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , Configptr[Pin_Index].pin_num);             /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
        
        if(Configptr[Pin_Index].resistor == PULL_UP)
        {
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , Configptr[Pin_Index].pin_num);       /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
        }
        else if(Configptr[Pin_Index].resistor == PULL_DOWN)
        {
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Configptr[Pin_Index].pin_num);     /* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
        }
        else
        {
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , Configptr[Pin_Index].pin_num);     /* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Configptr[Pin_Index].pin_num);   /* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */
        }
    }
    else
    {
      /* Do Nothing */
    }
  }  
    }
}
}

/************************************************************************************
* Service Name: Port_GetVersionInfo
* Service ID[hex]: 0x03
* Sync/Async: Synchronous
* Reentrancy: Non Reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): VersionInfo - Pointer to where to store the version information of this module.
* Return value: None
* Description: Function to get the version information of this module.
************************************************************************************/
#if (PORT_VERSION_INFO_API == STD_ON)
void Port_GetVersionInfo(Std_VersionInfoType *versioninfo)
{
#if (PORT_DEV_ERROR_DETECT == STD_ON)
	/* Check if input pointer is not Null pointer */
	if(NULL_PTR == versioninfo)
	{
		/* Report to DET  */
		Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
				PORT_GET_VERSION_INFO_SID, PORT_E_PARAM_POINTER);
	}
        if (Port_Status == PORT_NOT_INITIALIZED )
        {
    Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_GET_VERSION_INFO_SID,
		     PORT_E_UNINIT);
         }
	else
#endif /* (PORT_DEV_ERROR_DETECT == STD_ON) */
	{
		/* Copy the vendor Id */
		versioninfo->vendorID = (uint16)PORT_VENDOR_ID;
		/* Copy the module Id */
		versioninfo->moduleID = (uint16)PORT_MODULE_ID;
		/* Copy Software Major Version */
		versioninfo->sw_major_version = (uint8)PORT_SW_MAJOR_VERSION;
		/* Copy Software Minor Version */
		versioninfo->sw_minor_version = (uint8)PORT_SW_MINOR_VERSION;
		/* Copy Software Patch Version */
		versioninfo->sw_patch_version = (uint8)PORT_SW_PATCH_VERSION;
	}
}
#endif
/************************************************************************************
* Service Name: Port_SetPinMode
* Service ID[hex]: 0x04
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
void Port_SetPinMode(Port_PinType Pin, Port_PinModeType Mode)
{
  volatile uint32 * Port_Ptr = NULL_PTR; /* point to the required Port Registers base address */
   const Port_ConfigPins * Configptr = NULL_PTR;
   const  Port_ConfigType * Configptr2 = NULL_PTR;
    Configptr = Configptr2 -> Pins;
    
#if (DIO_DEV_ERROR_DETECT == STD_ON)
  if (Port_Status == PORT_NOT_INITIALIZED )
  {
    Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SetPinMode_SID,
		     PORT_E_UNINIT);
  }
  if ( STD_OFF == Configptr[Pin].Pin_modeconfig)
  {
    Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SetPinMode_SID,
		     PORT_E_MODE_UNCHANGEABLE);
  }
  if (Pin > PORT_CONFIGURED_PINS )
  {
    Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SetPinMode_SID,
		     PORT_E_PARAM_PIN);
  }
  if ((Mode > PORT_PIN_MODE_USB ) || (Mode < PORT_PIN_MODE_GPIO ) )
  {
    Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SetPinMode_SID,
		     PORT_E_PARAM_INVALID_MODE);
  }  
  else
#endif    
  {
    switch(Configptr[Pin].port_num)
    {
        case  0: Port_Ptr= (volatile uint32 *)PORTA_BASE_ADDRESS; /* PORTA Base Address */
		 break;
	case  1: Port_Ptr = (volatile uint32 *)PORTB_BASE_ADDRESS; /* PORTB Base Address */
		 break;
	case  2: Port_Ptr= (volatile uint32 *)PORTC_BASE_ADDRESS; /* PORTC Base Address */
		 break;
	case  3: Port_Ptr= (volatile uint32 *)PORTD_BASE_ADDRESS; /* PORTD Base Address */
		 break;
        case  4: Port_Ptr = (volatile uint32 *)PORTE_BASE_ADDRESS; /* PORTE Base Address */
		 break;
        case  5: Port_Ptr = (volatile uint32 *)PORTF_BASE_ADDRESS; /* PORTF Base Address */
		 break;
    } 
     if(Configptr[Pin].direction == PORT_PIN_OUT)
    {
	SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , Pin);                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
        
        if(Configptr[Pin].initial_value == STD_HIGH)
        {
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , Pin);          /* Set the corresponding bit in the GPIODATA register to provide initial value 1 */
        }
        else
        {
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , Pin);        /* Clear the corresponding bit in the GPIODATA register to provide initial value 0 */
        }
    }
    else if(Configptr[Pin].direction == PORT_PIN_IN)
    {
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , Pin);             /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
        
        if(Configptr[Pin].resistor == PULL_UP)
        {
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , Pin);       /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
        }
        else if(Configptr[Pin].resistor == PULL_DOWN)
        {
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Pin);     /* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
        }
        else
        {
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , Pin);     /* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Pin);   /* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */
        }
    }
    else
    {
        /* Do Nothing */
    }
    
  if (Mode == PORT_PIN_MODE_GPIO )    /* GPIO */
    {
    
    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Pin);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Pin);             /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
    *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Pin * 4));     /* Clear the PMCx bits for this pin */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Pin);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
   }
   else if (Mode == PORT_PIN_MODE_ADC)
   {
      SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Pin);      /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */      
      CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , Pin);             /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
      SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Pin);   /* Set the corresponding bit in the GPIOAMSEL register to enable analog functionality on this pin */
    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Pin);        /* Clear the corresponding bit in the GPIODEN register to disable digital functionality on this pin */
   }
    else if (Mode == PORT_PIN_MODE_ANALOG_COMP)
  {
     /*  There are two types of pins inputs (Analog) and outputs (Digital) in the analog compartor*/
     SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Pin);      /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */      
     CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , Pin);             /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
     if(Configptr[Pin].direction == PORT_PIN_OUT)
    {
      *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |=(0x00000009 << (Pin * 4));
	SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , Pin);                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
        
        if(Configptr[Pin].initial_value== STD_HIGH)
        {
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , Pin);          /* Set the corresponding bit in the GPIODATA register to provide initial value 1 */
        }
        else
        {
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , Pin);        /* Clear the corresponding bit in the GPIODATA register to provide initial value 0 */
        }
       SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Pin);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */  
    }
    else if(Configptr[Pin].direction == PORT_PIN_IN)
    {
         SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Pin);   /* Set the corresponding bit in the GPIOAMSEL register to enable analog functionality on this pin */
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , Pin);             /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
        
        if(Configptr[Pin].resistor == PULL_UP)
        {
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , Pin);       /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
        }
        else if(Configptr[Pin].resistor == PULL_DOWN)
        {
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Pin);     /* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
        }
        else
        {
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , Pin);     /* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Pin);   /* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */
        }
       CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Pin);  /* Clear the corresponding bit in the GPIODEN register to disable digital functionality on this pin */   
    }
    else
    {
        /* Do Nothing */
    }
    
   } 
   
   else if (Mode == PORT_PIN_MODE_CAN)
   {   
    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Pin);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Pin);             /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
    
    if ((Pin == 17 ) || (Pin == 18 ) || (Pin == 57 )||(Pin == 58 ) || (Pin == 59 ) || (Pin == 60 ))
    {
    
    *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000008 << (Pin * 4));     /* Clear the PMCx bits for this pin */
    }
    else if ((Pin == 17 ) || (Pin == 18 ))
    {
      *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000003 << (Pin * 4));     /* Clear the PMCx bits for this pin */
    }
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Pin);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
   }
   else if (Mode == PORT_PIN_MODE_CORE)
   {
    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Pin);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Pin);             /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
    *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x0000000E << (Pin * 4));     /* Set the PMCx bits for this pin */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Pin);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
   }
   else if (Mode == PORT_PIN_MODE_GPT)
   {
    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Pin);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Pin);             /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
    *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000007 << (Pin * 4));     /* Set the PMCx bits for this pin */
     SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Pin);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
   }
   else if (Mode == PORT_PIN_MODE_I2C)
   {
         CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Pin);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Pin);             /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
    *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000003 << (Pin * 4));     /* Set the PMCx bits for this pin */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Pin);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
   }
   else if (Mode == PORT_PIN_MODE_PWM)
   {
     CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Pin);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Pin);             /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
   if((Pin==12) || (Pin==13) ||Pin==14 ||Pin==15||Pin==20 ||Pin==21 ||Pin==24||Pin==25|Pin==36 ||Pin==37)/*PB4--PB7 , PC4,5  ,PD0,1 ,PE4,5*/
       {
       *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000004 << (Pin * 4));    /* Set the PMCx bits for this pin */
      }
       else
       {
         *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000005<< (Pin * 4));   /* Set the PMCx bits for this pin */
       }
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Pin);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
     
   }
   else if (Mode == PORT_PIN_MODE_QEI)
   {
       CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Pin);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Pin);             /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
    *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000006 << (Pin * 4));     /* Set the PMCx bits for this pin */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Pin);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
   }
   else if (Mode == PORT_PIN_MODE_SSI)
   {
           CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Pin);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Pin);             /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
   if((Pin)==61 ||(Pin)==62 ||(Pin)==63 ||(Pin)==64)
       {
       *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000001 << (Pin * 4));    /* Set the PMCx bits for this pin */
      }
       else
       {
         *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000002<< (Pin * 4));   /* Set the PMCx bits for this pin */
       }
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Pin);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
   }
   else if (Mode == PORT_PIN_MODE_NMI)
   {
      CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Pin);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Pin);             /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
    *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000008 << (Pin * 4));     /* Clear the PMCx bits for this pin */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Pin);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
   }
   else if (Mode == PORT_PIN_MODE_UART)
   {
         CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Pin);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Pin);             /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
   if((Pin)==15 ||(Pin)==16 )
       {
       *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000002 << (Pin * 4));    /* Set the PMCx bits for this pin */
      }
       else
       {
         *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000001<< (Pin * 4));   /* Set the PMCx bits for this pin */
       }
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Pin);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
   }
   else if (Mode == PORT_PIN_MODE_USB)
   {
      /*  There are two types of pins inputs (Analog) and outputs (Digital) in the analog compartor*/
     SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Pin);      /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */      
     CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , Pin);             /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
    if((Pin)==43 ||(Pin)==44 || (Pin)==45 || (Pin)==46 )
      {
             SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Pin);   /* Set the corresponding bit in the GPIOAMSEL register to enable analog functionality on this pin */  
             CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Pin);  /* Clear the corresponding bit in the GPIODEN register to disable digital functionality on this pin */ 
      }
      else 
      {
        *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |=(0x00000008 << (Pin * 4));
        SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Pin);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */ 
      }
   }
   else 
   {
     /* Do Nothing */
   }  
  } 
}
#endif
