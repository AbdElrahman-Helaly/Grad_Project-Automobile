/*********************************************************************/
/*********************************************************************/
/*********************************************************************/
/*****************     Author : AbdElrahman Elnahas      *************/
/******************    Layer : MCAL                      *************/ 
/******************    SWC: PORT                         *************/
/******************    Version : 1.00=                   *************/
/*********************************************************************/
/*********************************************************************/
/*********************************************************************/

#ifndef PORT_CONFIG_H_
#define PORT_CONFIG_H_

/*
   Choose 0 : for input
          1 : for output
*/

#define PORTA_PIN0_DIR              1
#define PORTA_PIN1_DIR              1
#define PORTA_PIN2_DIR              1
#define PORTA_PIN3_DIR              1
#define PORTA_PIN4_DIR              1
#define PORTA_PIN5_DIR              1
#define PORTA_PIN6_DIR              1
#define PORTA_PIN7_DIR              1

#define PORTB_PIN0_DIR              1
#define PORTB_PIN1_DIR              0
#define PORTB_PIN2_DIR              0
#define PORTB_PIN3_DIR              0
#define PORTB_PIN4_DIR              0
#define PORTB_PIN5_DIR              0
#define PORTB_PIN6_DIR              0
#define PORTB_PIN7_DIR              0

#define PORTC_PIN0_DIR              1
#define PORTC_PIN1_DIR              1
#define PORTC_PIN2_DIR              1
#define PORTC_PIN3_DIR              0
#define PORTC_PIN4_DIR              0
#define PORTC_PIN5_DIR              1
#define PORTC_PIN6_DIR              1
#define PORTC_PIN7_DIR              1

#define PORTD_PIN0_DIR              1
#define PORTD_PIN1_DIR              1
#define PORTD_PIN2_DIR              0
#define PORTD_PIN3_DIR              1
#define PORTD_PIN4_DIR              1
#define PORTD_PIN5_DIR              1
#define PORTD_PIN6_DIR              1
#define PORTD_PIN7_DIR              1

/*
    Choose 1 : for HIGH if output, PULL_UP if input
           0 : for LOW if output , FLOATING if input
		 
*/


#define PORTA_PIN0_INIT_VAL             1
#define PORTA_PIN1_INIT_VAL             1
#define PORTA_PIN2_INIT_VAL             1
#define PORTA_PIN3_INIT_VAL             1
#define PORTA_PIN4_INIT_VAL             1
#define PORTA_PIN5_INIT_VAL             1
#define PORTA_PIN6_INIT_VAL             1
#define PORTA_PIN7_INIT_VAL             1
                   
#define PORTB_PIN0_INIT_VAL             0
#define PORTB_PIN1_INIT_VAL             0
#define PORTB_PIN2_INIT_VAL             0
#define PORTB_PIN3_INIT_VAL             0
#define PORTB_PIN4_INIT_VAL             0
#define PORTB_PIN5_INIT_VAL             0
#define PORTB_PIN6_INIT_VAL             0
#define PORTB_PIN7_INIT_VAL             0
                   
#define PORTC_PIN0_INIT_VAL             0
#define PORTC_PIN1_INIT_VAL             0
#define PORTC_PIN2_INIT_VAL             0
#define PORTC_PIN3_INIT_VAL             0
#define PORTC_PIN4_INIT_VAL             0
#define PORTC_PIN5_INIT_VAL             0
#define PORTC_PIN6_INIT_VAL             0
#define PORTC_PIN7_INIT_VAL             0
                   
#define PORTD_PIN0_INIT_VAL             0
#define PORTD_PIN1_INIT_VAL             0
#define PORTD_PIN2_INIT_VAL             0
#define PORTD_PIN3_INIT_VAL             0
#define PORTD_PIN4_INIT_VAL             0
#define PORTD_PIN5_INIT_VAL             0
#define PORTD_PIN6_INIT_VAL             0
#define PORTD_PIN7_INIT_VAL             0

#endif
