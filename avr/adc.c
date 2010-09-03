#include "config.h"

//-----------------------
// 定义变量
AdcStatusFlag adc_status_flag;
AdcQueue 	  adc_queue;						// 类型于环形队列, 保存最近的转换结果
unsigned int  adc_result;						// 保存了转换结果
unsigned char nadc_mean_num = ADC_MAX_SIZE;		// 在滑动求平地均的算法中, 时间窗的大小,(即对最近的多少个值求平均)

void adc_init(void)
{
	ADC_OFF();									//disable adc
	ADMUX = ADC_REFER_VACC | ADC_CHANNEL_3; 	//设置ADC参考电源为AVCC, select adc input ADC3，ADC3为空
	ACSR  = _BV(ACD);							//模拟比较器禁用
	ADC_ON();
}

void adc_switch_channel(bool channel)
{
	switch(channel)
	{
		case ADC_VOLTAGE:
			ADMUX = ADC_REFER_VACC | ADC_CHANNEL_1;
			break;
		
		case ADC_CURRENT:
			ADMUX = ADC_REFER_VACC | ADC_CHANNEL_2;
			break;
			
		default:
			break;
	}
}

ISR(ADC_vect)
{
	if(adc_status_flag.is_current_convert == TRUE)
	{
		if(adc_status_flag.is_first_time  == TRUE)
		{
			adc_status_flag.is_first_time = FALSE;
			START_ADC();						// 启动AD转换
		}
		else
		{
			adc_result = ADC;
			adc_queue_add_current(adc_result);

			adc_status_flag.is_first_time 	   = TRUE;
			adc_status_flag.is_current_convert = FALSE;

			adc_switch_channel(ADC_CURRENT);			// 切换通道
			START_ADC();						// 启动AD转换
		}
	}
	else
	{
		if(adc_status_flag.is_first_time == TRUE)
		{	
			adc_status_flag.is_first_time = FALSE;
			START_ADC();						// 启动AD转换
		}
		else
		{
			adc_result = ADC;
			adc_queue_add_voltage(adc_result);
		}
	}
}

void adc_queue_init()
{
	memset(&adc_queue, 0x00, sizeof(AdcQueue));
}

void adc_convert_start(void)
{
	adc_status_flag.is_current_convert = TRUE;
	adc_status_flag.is_first_time 	   = TRUE;
	adc_switch_channel(ADC_VOLTAGE);

	START_ADC();
}

void adc_queue_add_current(short int data)
{
	adc_queue.data[adc_queue.tail].current = data;
}

void adc_queue_add_voltage(short int data)
{
	adc_queue.data[adc_queue.tail].voltage = data;
	//adc_queue.data[ adc_queue.tail ].wPower = data * adc_queue.data[adc_queue.tail].current;

	ADCQUEUE_TAIL_ADD();

	adc_queue.count++; 
	adc_queue.count = min(adc_queue.count, ADC_MAX_SIZE);
}

void adc_set_mean_nums(unsigned char num)
{
	nadc_mean_num = min(num, ADC_MAX_SIZE);
	nadc_mean_num = max(num, 1);
}

/*
void adc_queue_get_data(float* pCurrent, float* pVoltage, float* pPower)
{
	unsigned char i;
	unsigned char n;
	unsigned char tail;
	short int resCurrent = 0;
	short int resVoltage = 0;
	short int wTempCurrent = 0;
	short int wTempVoltage = 0;
	float resPower = 0.0f;

	n = min(nadc_mean_num, adc_queue.count);
	if(0 == n)
		return;

	tail = adc_queue.tail;
	tail = (tail-1) & ADC_MAX_SIZE_MASK;	// 减1之后,指向最后加入的一个数据

	for(i=0; i<n; i++)
	{
		wTempCurrent = adc_queue.data[ tail ].current;
		wTempVoltage = adc_queue.data[ tail ].voltage;
		resCurrent += wTempCurrent;
		resVoltage += wTempVoltage;

		resPower += (5.0f * wTempCurrent * MeasureParaConfig.fCurrentRatio / 1024.0f + MeasureParaConfig.fCurrentOffset) * 
			(5.0f * wTempVoltage * MeasureParaConfig.fVoltageRatio / 1024.0f + MeasureParaConfig.fVoltageOffset );
		
		tail = (tail-1) & ADC_MAX_SIZE_MASK;
	}

	*pCurrent = 5.0f * resCurrent * MeasureParaConfig.fCurrentRatio / n / 1024.0f + MeasureParaConfig.fCurrentOffset;
	*pVoltage = 5.0f * resVoltage * MeasureParaConfig.fVoltageRatio / n / 1024.0f + MeasureParaConfig.fVoltageOffset;
	*pPower = resPower / n;
}
*/