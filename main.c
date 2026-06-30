
// PIC18F4550 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1L
#pragma config PLLDIV = 1       // PLL Prescaler Selection bits (No prescale (4 MHz oscillator input drives PLL directly))
#pragma config CPUDIV = OSC1_PLL2// System Clock Postscaler Selection bits ([Primary Oscillator Src: /1][96 MHz PLL Src: /2])
#pragma config USBDIV = 1       // USB Clock Selection bit (used in Full-Speed USB mode only; UCFG:FSEN = 1) (USB clock source comes directly from the primary oscillator block with no postscale)

// CONFIG1H
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator (HS))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR = OFF        // Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
#pragma config BORV = 3         // Brown-out Reset Voltage bits (Minimum setting 2.05V)
#pragma config VREGEN = OFF     // USB Voltage Regulator Enable bit (USB voltage regulator disabled)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = OFF     // CCP2 MUX bit (CCP2 input/output is multiplexed with RB3)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = OFF      // MCLR Pin Enable bit (RE3 input pin enabled; MCLR pin disabled)

// CONFIG4L
#pragma config STVREN = OFF     // Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config ICPRT = OFF      // Dedicated In-Circuit Debug/Programming Port (ICPORT) Enable bit (ICPORT disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) is not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) is not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) is not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) is not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) is not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM is not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) is not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) is not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) is not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) is not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) are not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) is not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM is not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) is not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) is not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include "nxlcd.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define _XTAL_FREQ 20000000 //20MHz
#define DELAY 200


// configuracao das variveis do teclado matricial
int coluna = 0;
int linha = 4;
char value = 'B';
int clicado = 1;
char table[4][4] = {{'1','2','3','A'},
                    {'4','5','6','B'},
                    {'7','8','9','C'},
                    {'*','0','#','D'}};

// configuracao das variaveis para medicao da temperatura
int raw = 0;
float temperatura = 20;
char temperaturaS[20];
int tempAlta = 100;
int tempCritica = 100;


// funcao para ler o valor do AD
int lerTemp() {
    ADCON0bits.GO_DONE = 1; 			    // Inicia a convers�o A/D
    while (ADCON0bits.GO_DONE);  			// Fim da Convers�o
    int temp = 256 * ADRESH + ADRESL; 	    // Armazena valor
    return temp;
}

void lerTeclado() {
    clicado = 0;
    
    // aciona a coluna da vez
    if(coluna == 0) {
        PORTBbits.RB3 = 0;
    } else {
        PORTBbits.RB3 = 1;
    }
    if(coluna == 1) {
        PORTBbits.RB2 = 0;
    } else {
        PORTBbits.RB2 = 1;
    }
    if(coluna == 2) {
        PORTBbits.RB1 = 0;
    } else {
        PORTBbits.RB1 = 1;
    }
    if(coluna == 3) {
        PORTBbits.RB0 = 0;
    } else {
        PORTBbits.RB0 = 1;
    }

    linha = 4;

    // chega se botao foi apertado
    if(!PORTBbits.RB4) {
        linha = 0;
    }
    if(!PORTBbits.RB5) {
        linha = 1;
    }
    if(!PORTBbits.RB6) {
        linha = 2;
    }
    if(!PORTBbits.RB7) {
        linha = 3;
    }

    // se alguma linha foi apertada guarda o valor do botao em value
    if(linha != 4) {
        value = table[linha][coluna];
        clicado = 1;  // guarda a informacao que um botao foi apertado
    }

    // troca para a proxima coluna ou volta pra primeira
    if(coluna == 3) {
        coluna = 0;
    } else {
        coluna++;
    }
}

void menuTemperatura() {
    // Formatacao da temperatura inicial
    snprintf(temperaturaS, sizeof(temperaturaS), "%.2f", temperatura);
    strcat(temperaturaS, " oC");

    WriteCmdXLCD(0x01);             // Limpa o LCD com retorno do cursor
	__delay_ms(10);                 // Atraso de 10ms para aguardar a execucao do comando
    
    // imprime temperatura e status inicial
    WriteCmdXLCD(0x80);
    putsXLCD("Temp:");
    __delay_ms(10);
    WriteCmdXLCD(0x86);
    putsXLCD(temperaturaS);
    __delay_ms(10);
    WriteCmdXLCD(0xC0);
    putsXLCD("Status: OFF");
    __delay_ms(10);

    while(value != 'A') {
        // le e converte a leitura do AD para graus celsius
        raw = lerTemp();
        temperatura = raw * 4.8875 / 10;
        
        // transforma o valor num�rico em string
        snprintf(temperaturaS, sizeof(temperaturaS), "%.2f", temperatura);
        strcat(temperaturaS, " oC");
        strcat(temperaturaS, "   ");

        // mostra o valor da temperatura
        WriteCmdXLCD(0x86);
        putsXLCD(temperaturaS);
        __delay_ms(10);
        
        if (temperatura >= tempAlta) { // SIM:
            PORTDbits.RD0 = 1; 		// Liga ventoinha
            WriteCmdXLCD(0xC8);
            putsXLCD("ON ");        // Imprime status "ON"
            __delay_ms(10);
        } else {                       // NAO:
            PORTDbits.RD0 = 0; 		// Desliga ventoinha
            WriteCmdXLCD(0xC8);
            putsXLCD("OFF");        // Imprime status "OFF"
            __delay_ms(10);
        }
        if (temperatura > tempCritica) {
            PORTDbits.RD1 = 1;     // SIM - Liga o LED
        } else {
            PORTDbits.RD1 = 0;     // NAO - Desliga o LED
        }
        
        lerTeclado();
    }
}

void menuConfig() {
    char novaTemp[10] = "";
    int i = 0;

    WriteCmdXLCD(0x01);             // Limpa o LCD com retorno do cursor
	__delay_ms(10);                 // Atraso de 10ms para aguardar a execucao do comando
    
    // imprime o menu
    WriteCmdXLCD(0x80);
    putsXLCD("Temp:");
    __delay_ms(10);
    WriteCmdXLCD(0xC0);
    putsXLCD("* para confirmar");
    __delay_ms(10);

    while(1) {
        lerTeclado();
        __delay_ms(80);
        
        // se nao for numero ignorar
        if(value == 'A' || value == 'B' || value == 'C' || value == 'D' || value == '#') {}
        
        // se for "*" e tiver pelo menos um numero:
        else if(value == '*' && strlen(novaTemp) > 0) {
            tempAlta = atoi(novaTemp); // checa se estourou a temperatura limite
            if(tempAlta > 499) {
                tempAlta = 499;
            }
            tempCritica = tempAlta;    // define nova temperatura
            value = 'B';               // troca pro menuTemperatura
            break;
        }
        
        // se clicou algum numero:
        else if(clicado && strlen(novaTemp) < 3) {
            novaTemp[i] = value;  // adiciona o numero na string
            i++;
            WriteCmdXLCD(0x86);   // mostra ele na tela
            putsXLCD(novaTemp);
            __delay_ms(10);
        }
    }
}

void main(void) {
    // configuracao ventoinha
    TRISDbits.RD0 = 0;
    PORTDbits.RD0 = 0;
    
    // configuracao LED de alerta
    TRISDbits.RD1 = 0;
    PORTDbits.RD1 = 0;
    
    // configuracao do teclado matricial
    TRISB = 0xF0;
    PORTB = 0x0F;
    
    // configuracao do AD
    TRISAbits.TRISA1 = 1; // AN1 � entrada
    ADCON1 = 0b00001101; // Habilita AN0 e AN1, Vref+=Vcc e Vref?=Vss
    ADCON0 = 0b00000101; // Seleciona AN1 e liga m�dulo A/D
    ADCON2 = 0b10010101; // Resultado alinhado � direita TACQ = 4TAD e TAD = FOSC/16
    __delay_ms(10); // Delay de 10ms
    
    //Inicializa o do LCD
    OpenXLCD(FOUR_BIT & LINES_5X7); // Modo 4 bits de dados e caracteres 5x7
	WriteCmdXLCD(0x01);             // Limpa o LCD com retorno do cursor
	__delay_ms(10);                 // Atraso de 10ms para aguardar a execucao do comando
    
    while(1) {
        if(value == 'A') {
            menuConfig();
        } else if(value == 'B') {
            menuTemperatura();
        }
    }
}

