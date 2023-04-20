// ------------------------------------------------------------------------------
// DATOS Y ENCABEZADO
//-------------------------------------------------------------------------------
    
// Archivo:                 postlab8.c
// Dispositivo:             PIC16F887
// Autor:                   GUSTABO CÓRDOVA 
// Programa:                Opciones en terminal
// Creado:                  18 APR, 2023 
// Última modificación:     19 APR, 2023

// ------------------------------------------------------------------------------
// CONFIGURACIÓN DEL PIC
//-------------------------------------------------------------------------------
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// ------------------------------------------------------------------------------
// LIBRERÍAS
//-------------------------------------------------------------------------------
#include <xc.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

// ------------------------------------------------------------------------------
// CONSTANTES
//-------------------------------------------------------------------------------
#define _XTAL_FREQ 1000000

const char menu[] = "          /////////////// MENU PRINCIPAL \\\\\\\\\\\\\\\\\\\\\\\\\\ \n\r";
const char I1[] = "Elija entre 1 o 2, dependiendo de la opción que desee realizar --> \n\r";
const char I2[] = "1. Leer Pot \n\r";
const char I3[] = "2. Enviar ASCII \n\r";
const char I4[] = "Digite una opción: ";
const char opc1[] = "1 \n\r";
const char opc2[] = "2 \n\r";
const char I5[] = "Por favor elija un numero entre 1 y 2 \n\r";
const char I6[] = "El valor del Pot es: ";
const char espacio[] = "\n\r";
int ans = 0;
char pot[];
const char ASCII[] = "¡Se ve que le sabes a la progra eh!";

// ------------------------------------------------------------------------------
// INTERRUPCIONES
//-------------------------------------------------------------------------------
void __interrupt() isr(void) 
{
    if (PIR1bits.RCIF)          // Verifica si hay un carácter recibido en el registro del USART
    {
        PORTD = RCREG;          // Asigna el valor recibido al puerto D
        ans = PORTD;            // Asigna el valor del puerto D a la variable "ans"
    }
}

// ------------------------------------------------------------------------------
// PROTOTIPO DE FUNCIONES
//-------------------------------------------------------------------------------
void setup(void);
void print_string(char* cadena);

// ------------------------------------------------------------------------------
// CICLO PRINCIPAL
//-------------------------------------------------------------------------------
void main(void) {
    setup();                                // Llama a la función setup

    print_string(menu);                     // Imprime el menú de opciones por USART
    print_string(I1);                       // Imprime el texto I1 por USART
    print_string(I2);                       // Imprime el texto I2 por USART
    print_string(I3);                       // Imprime el texto I3 por USART
    print_string(espacio);                  // Imprime el texto "espacio" por USART    
    print_string(I4);                       // Imprime el texto I4 por USART
    
    while (ans == 0);                       // Espera a que el usuario seleccione una opción del menú
    
    if (ans == 49) {                        // Si se seleccionó la opción 1
        print_string(opc1);                 // Imprime el texto correspondiente a la opción 1 por USART
        ADCON0bits.CHS = 0;                 // Selecciona el canal 0 del ADC
        ADCON0bits.GO = 1;                  // Inicia la conversión del ADC
        while (ADCON0bits.GO);              // Espera a que la conversión del ADC termine
        int adc_value_1 = ADRESH;           // Obtiene el resultado de la conversión del ADC
        PORTB = (char) adc_value_1;         // Asigna el valor del ADC al puerto B
        __delay_ms(10); 
        
        sprintf(pot, "%d", adc_value_1);    // Convierte el valor del ADC a una cadena de caracteres
        print_string(I6);                   // Imprime el texto I6 por USART
        print_string(pot);                  // Imprime el valor del ADC por USART
        print_string(espacio);              // Imprime un espacio en blanco por USART
        print_string(espacio);            
        print_string(espacio);             

    }
    else if (ans == 50) {                   // Si se seleccionó la opción 2
        print_string(opc2);                 // Imprime el texto correspondiente a la opción 2 por USART
        print_string(ASCII);                // Imprime el texto ASCII por USART
        print_string(espacio);              
        print_string(espacio);              
        print_string(espacio);              
       
    }
    else {                                  // Si se seleccionó otra opción
        print_string(I5);                   // Imprime el texto I5 por USART
        print_string(espacio);              
    }
}

// ------------------------------------------------------------------------------
// CONFIGURACIÓN GENERAL
//-------------------------------------------------------------------------------
void setup(void){

    // Pines digitales
    ANSEL = 0;
    ANSELH = 0;
    
    TRISB = 0;
    
    TRISD = 0;
    PORTD = 0;
    
    //Configurando el oscilador
    OSCCONbits.IRCF = 0b100;    // 1MHz
    OSCCONbits.SCS = 1;         // Oscilador interno
    
    BAUDCTLbits.BRG16 = 1;
    SPBRG = 25;
    SPBRGH = 0;
    
    //Configurando TX y RX
    TXSTAbits.SYNC = 0;     
    TXSTAbits.BRGH = 1;    
    
    RCSTAbits.SPEN = 1;     
    RCSTAbits.RX9 = 0;     
    RCSTAbits.CREN = 1;    
    TXSTAbits.TXEN = 1;  
     
    // Configurando el ADC
    ANSELbits.ANS0 = 1;
    ANSELbits.ANS1 = 1;
    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA1 = 1;
    ADCON0bits.ADCS = 0b10; 
    __delay_ms(1);
    
    //Configurando las interrupciones
    PIR1bits.RCIF = 0;
    PIE1bits.RCIE = 1;
    INTCONbits.PEIE = 1; 
    INTCONbits.GIE = 1; 
    
    ADCON1bits.ADFM = 0; 
    ADCON1bits.VCFG0 = 0; 
    ADCON1bits.VCFG1 = 0; 
    ADCON0bits.ADON = 1; 
    ADIF = 0;
    return;
}

// ------------------------------------------------------------------------------
// FUNCIONES
//-------------------------------------------------------------------------------
void print_string(char* cadena) {   // Función que imprime una cadena de caracteres por USART
    int largo = strlen(cadena);     // Obtiene el largo de la cadena
    int i;
    __delay_ms(100); 
        
    for (i = 0; i < largo; i++) {   // Recorre la cadena de caracteres
        __delay_ms(10); 
        if (PIR1bits.TXIF) {        // Comprueba si el registro de transmisión está listo
            TXREG = cadena[i];      // Transmite el byte de datos actual
        }
    }
    return; 
}
