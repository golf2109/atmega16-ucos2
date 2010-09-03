#ifndef __ADC_H__
#define __ADC_H__

//-----------------------
// ADC数据队列的最大长度
#define ADC_MAX_SIZE				64
#define ADC_MAX_SIZE_MASK			0x3F
#define ADCQUEUE_TAIL_ADD()			adc_queue.tail = (adc_queue.tail + 1) & ADC_MAX_SIZE_MASK
//-----------------------
// 启动ADC测量
#define	START_ADC()					ADCSRA |= _BV(ADSC)			//ADC开始转换
#define	ADC_ON()					ADCSRA  = _BV(ADEN) | _BV(ADIE) | _BV(ADPS0)	//使能ADC及其中断，并进行2分频
#define	ADC_OFF()					ADCSRA  = 0x00				//不使能ADC

//------------ADC参考电压---------------------
#define ADC_REFER_VACC				_BV(REFS0)

//------------ADC输入引脚---------------------
#define ADC_CHANNEL_0				0x0
#define ADC_CHANNEL_1				0x1
#define ADC_CHANNEL_2				0x2
#define ADC_CHANNEL_3				0x3
#define ADC_CHANNEL_4				0x4
#define ADC_CHANNEL_5				0x5
#define ADC_CHANNEL_6				0x6
#define ADC_CHANNEL_7				0x7

//-----------电流电压引脚定义------------------
#define ADC_VOLTAGE					ADC_CHANNEL_1
#define ADC_CURRENT					ADC_CHANNEL_2

// 标识ADC转换的状态
typedef struct
{
	bool is_current_convert;			// 标识正在进行电流转换(true), 还是电压转换(false)
	bool is_first_time;					// 标识此次转换结果是否是切换通道后的第一次转换结果
}AdcStatusFlag;

typedef struct
{
	short int current;
	short int voltage;
	//long int power;
}Pair;

typedef struct
{
	unsigned char tail;
	unsigned char count;
	Pair data[ADC_MAX_SIZE];
} AdcQueue;

//-----------------------
extern AdcStatusFlag adc_status_flag;
extern unsigned char nadc_mean_num;	// 在滑动求平地均的算法中, 时间窗的大小,(即对最近的多少个值求平均)
extern AdcQueue adc_queue;			// 类型于环形队列, 保存最近的转换结果

//-----------------------
void adc_init(void);
void adc_switch_channel(bool channel);

void adc_queue_init(void);
void adc_convert_start(void);
void adc_queue_add_current(short int data);
void adc_queue_add_voltage(short int data);
void adc_queue_get_data(float* pCurrent, 
					    float* pVoltage,
					    float* pPower);
void adc_set_mean_nums(unsigned char num);

ISR(ADC_vect);

#endif
