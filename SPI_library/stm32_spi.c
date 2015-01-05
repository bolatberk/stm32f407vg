/**
  ******************************************************************************
  * @file    stm32_spi.c
  * @author  bolatberk
  * @version V1.0
  * @date    xx xx xxxx
  * @brief   stm32f407vg MCU icin SPI ile baska SPI destekleyen cihazlar icin yazilmistir.
  *
  *
  *
  *          Kutuphane hazırlanırken temelde STM32F4 Discovery icinde bulunan MEMS
  *          Accelloremetre den yararlanarak yapilmistir baska cihazlar icin ufak ayar
  *			 gerekli olabilir cihazların datasheeti incelenmelidir.
*********************************************************************************
**/

void spi_unit(){

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE); // kullanilacak SPI1 clock aktif edildi
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE , ENABLE);
	// GPIOA ile MISO MISO VE SCK pınlerının clocklari verildi
	// GPIOE ile ise CS pinin olarak kullanılacak pinin clocku aktif edildi

	GPIO_InitTypeDef GPIO_InitTypeDefStruct;
// SPI MISO MISO VE SCK pinleri GPIO ayarları yapiliyor.
	GPIO_InitTypeDefStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_6;
	GPIO_InitTypeDefStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitTypeDefStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitTypeDefStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitTypeDefStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitTypeDefStruct);
// SPI CS pininin ayarlari yapiliyor.
	GPIO_InitTypeDefStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitTypeDefStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitTypeDefStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitTypeDefStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitTypeDefStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOE, &GPIO_InitTypeDefStruct);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);

	GPIO_SetBits(GPIOE, GPIO_Pin_3);//CS pini default olarak HIGH levelde bırakılıyor
	// gonderme veya alma yapildiginda bu manuel olarak LOW level seviyeye cekilecektir.

	/* SPI AYARLARI YAPILIYOR */
	SPI_InitTypeDef SPI_InitTypeDefStruct;

	SPI_InitTypeDefStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;// SPI CLOCK HIZI AYARI YAPILIYOR
	SPI_InitTypeDefStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;// RX VR TX MODDUN HER İKİSİDE KULLANILACAGI SOYLENIYOR
	SPI_InitTypeDefStruct.SPI_Mode = SPI_Mode_Master;// STMNIN MASTER MODDA OLUCAGI AYARLANIYOR.
	SPI_InitTypeDefStruct.SPI_DataSize = SPI_DataSize_8b;// SPI DATA SIZE 8 BIT OLACAGI SOYLENIYOR
	SPI_InitTypeDefStruct.SPI_NSS = SPI_NSS_Soft;// CS LOW LEVEL YAPMA ISLEMININ MANUEL OLARAK YAPILCAGI SOYLENIYOR
	SPI_InitTypeDefStruct.SPI_FirstBit = SPI_FirstBit_MSB;//ILK BITIN MSB OLUCAGI AYARLANIYOR MEMS DATASHEETTEN BAKTIM BUNA
	SPI_InitTypeDefStruct.SPI_CPOL = SPI_CPOL_High;// ??
	SPI_InitTypeDefStruct.SPI_CPHA = SPI_CPHA_2Edge;// ??


	SPI_Init(SPI1, &SPI_InitTypeDefStruct);
	
	SPI_Cmd(SPI1, ENABLE);


}



uint8_t spi_read(uint8_t adress){
GPIO_ResetBits(GPIOE, GPIO_Pin_3); // CS low seviyealındı
adress |= 1<<7; // okuma yapılcagı ıcın ılk bıtın bır olması gerekıyor onun ıcın normal adress
// 1000 0000 ıle or ıslemenı tabı tutularak  adresın okuma ıcın gonderıldı bılgısı verılıyor.


while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));  //tx buff bosmu diye bakılıyor
	SPI_I2S_SendData(SPI1, adress);
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)); //data alınmısmı dıye bakılıyor.

	SPI_I2S_ReceiveData(SPI1);	//Clear RXNE bit

	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));  //tx buff bosmu diye bakılıyor
	SPI_I2S_SendData(SPI1, 0x00);
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)); //data alınmısmı dıye bakılıyor
	GPIO_SetBits(GPIOE, GPIO_Pin_3);// CS tekrar hıgh yapılıyor

	return SPI_I2S_ReceiveData(SPI1); //data geri donus olarak veriliyor.
}


void spi_wirite(uint8_t adress, uint8_t data){

	GPIO_ResetBits(GPIOE, GPIO_Pin_3);//CS low seviyealındı

	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));  //tx buff bosmu diye bakılıyor
	SPI_I2S_SendData(SPI1, adress);
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)); //data alınmısmı dıye bakılıyor
	SPI_I2S_ReceiveData(SPI1);

	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));  //tx buff bosmu diye bakılıyor
	SPI_I2S_SendData(SPI1, data);
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)); //data alınmısmı dıye bakılıyor
	SPI_I2S_ReceiveData(SPI1);

	GPIO_SetBits(GPIOE, GPIO_Pin_3);//CS tekrar hıgh yapılıyor
}
