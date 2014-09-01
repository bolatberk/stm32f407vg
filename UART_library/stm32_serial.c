/**
  ******************************************************************************
  * @file    stm32_serial.c
  * @author  bolatberk
  * @version V1.0
  * @date    27 08 2014
  * @brief   stm32f407vg MCU icin serial comm. interface ile baska IC/MCU ve PC
  *          haberlestirmek icin yazilmis kutuphanedir...
  *
  *
  *          GPIOA pininin A2 nolu pini kullanarak sadece TX modda
  *          veri gondermek icin ayarlanmistir.
*********************************************************************************
**/
extern void init_usart(void){

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* enable peripheral clock for USART2 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); // seri hab. clock aktif ediliyor


	/* GPIOA clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); // TX modu pin clock aktif ediliyor

	/* GPIOA Configuration:  USART2 TX on PA2 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Connect USART2 pins to AF2 */
	// TX = PA2
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);//AF olarak ayarlanan pin USART olarak belirtiliyor

	USART_InitStructure.USART_BaudRate = 9600; // haberlesme 9600 bit per second olarak ayarlaniyor
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;// data genisligi 8b olarak ayarlaniyor
	USART_InitStructure.USART_StopBits = USART_StopBits_1;// stop bit 1
	USART_InitStructure.USART_Parity = USART_Parity_No;// eslik biti kullanilmiyor
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);

	USART_Cmd(USART2, ENABLE); // enable USART2

}



extern void USART_puts(USART_TypeDef* USARTx, volatile char *s){

	while(*s){
		// wait until data register is empty
		while( !(USARTx->SR & 0x00000040) );
		USART_SendData(USARTx, *s);
		*s++;
	}
}





/* seri porttan rakam gondermek icin
sprintf(str,"result:%d\r\n",ADCResult);
		    	  		USART_puts(USART2,str);// "Init complete! Hello World!rn");
		    	  		kullan*/
