/* stm32f407vgt6 ile yapilmis adc library
   hem dısardan baglanan bir adc okumak icin
   ve hemde islemciye gömülmüs sıcaklık sensöro okumak ıcın hazırlanmıstır.
   */

extern void adc_setup(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE); // adc1 kanalý clock aktif edildi
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);// adc kanal okumasý için pin clock aktif edildi



        GPIO_InitTypeDef GPIO_InitStructure;

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
        GPIO_Init(GPIOA, &GPIO_InitStructure);


	    ADC_CommonInitTypeDef ADC_CommonInitStructure;
	    ADC_InitTypeDef       ADC_InitStructure;

	    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;

	    ADC_CommonInit(&ADC_CommonInitStructure);

	    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	    ADC_InitStructure.ADC_ExternalTrigConv =0;
	    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	    ADC_InitStructure.ADC_NbrOfConversion = 1;



	    ADC_Init(ADC2, &ADC_InitStructure);
	    ADC_RegularChannelConfig(ADC2, ADC_Channel_1, 1, ADC_TwoSamplingDelay_5Cycles);


	    ADC_Cmd(ADC2, ENABLE);


}



extern void adc_tempsen(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); // adc1 kanalý clock aktif edildi




	    ADC_CommonInitTypeDef ADC_CommonInitStructure;
	    ADC_InitTypeDef       ADC_InitStructure;

	    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;

	    ADC_CommonInit(&ADC_CommonInitStructure);

	    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	    ADC_InitStructure.ADC_ExternalTrigConv =0;
	    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	    ADC_InitStructure.ADC_NbrOfConversion = 1;



	    ADC_Init(ADC1, &ADC_InitStructure);
	    ADC_RegularChannelConfig(ADC1,ADC_Channel_TempSensor, 1, ADC_TwoSamplingDelay_5Cycles);
        ADC_TempSensorVrefintCmd(ENABLE);

	    ADC_Cmd(ADC1, ENABLE);


}




extern volatile uint16_t adc_read(ADC_TypeDef* ADCx){
    ADC_SoftwareStartConv(ADCx);
    while(ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC) == RESET);

    return ADC_GetConversionValue(ADCx);
}

