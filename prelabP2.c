// ------------------------------------------------------------------------------
// DATOS Y ENCABEZADO
//-------------------------------------------------------------------------------
    
// Archivo:                 prelab8.c
// Dispositivo:             PIC16F887
// Autor:                   GUSTABO CÓRDOVA 
// Programa:                Cadena de caracteres utilizando comunicación serial
// Creado:                  16 APR, 2023 
// Última modificación:     17 APR, 2023

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
#include <stdint.h>

// ------------------------------------------------------------------------------
// CONSTANTES
//-------------------------------------------------------------------------------
const char data = 71;
#define _XTAL_FREQ 4000000

// ------------------------------------------------------------------------------
// INTERRUPCIONES
//-------------------------------------------------------------------------------
void __interrupt() isr (void)
{
    if (PIR1bits.RCIF)
    {
    PORTB = RCREG;
    }
}

// ------------------------------------------------------------------------------
// PROTOTIPO DE FUNCIONES
//-------------------------------------------------------------------------------
void setup(void);

// ------------------------------------------------------------------------------
// CICLO PRINCIPAL
//-------------------------------------------------------------------------------
void main(void){
    setup();                 // Llama a la función setup
    while (1) 
    { 
        __delay_ms(500); 
        if (PIR1bits.TXIF) { // Comprueba si el bit TXIF está activo
            TXREG = data;    // Transmite la variable data
        }
    }
    
    return; // Termina la función
}
// ------------------------------------------------------------------------------
// CONFIGURACIÓN GENERAL
//-------------------------------------------------------------------------------
void setup(void){

    ANSEL = 0;
    ANSELH = 0;
    
    TRISB = 0;
    PORTB =0;
    
   //Configurando el oscilador
    OSCCONbits.IRCF = 0b100; // 1Mhz
    OSCCONbits.SCS = 1;
  
   //Configurando el TMR2
    T2CONbits.TOUTPS = 0b0100;  
    T2CONbits.T2CKPS = 0b11;    
    
   //Configurando las interrupciones
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    PIR1bits.RCIF = 0;
    PIE1bits.RCIE = 1;
    
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 1;
    
    BAUDCTLbits.BRG16 = 1;
    
    SPBRG = 25;
    SPBRGH =0;
    
    RCSTAbits.SPEN = 1;
    RCSTAbits.RX9 = 0;
    RCSTAbits.CREN = 1;
    
    TXSTAbits.TXEN = 1;
}

