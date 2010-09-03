#ifndef __ADC_H__
#define __ADC_H__

//-----------------------
// ADC���ݶ��е���󳤶�
#define ADC_MAX_SIZE				64
#define ADC_MAX_SIZE_MASK			0x3F
#define ADCQUEUE_TAIL_ADD()			adc_queue.tail = (adc_queue.tail + 1) & ADC_MAX_SIZE_MASK
//-----------------------
// ����ADC����
#define	START_ADC()					ADCSRA |= _BV(ADSC)			//ADC��ʼת��
#define	ADC_ON()					ADCSRA  = _BV(ADEN) | _BV(ADIE) | _BV(ADPS0)	//ʹ��ADC�����жϣ�������2��Ƶ
#define	ADC_OFF()					ADCSRA  = 0x00				//��ʹ��ADC

//------------ADC�ο���ѹ---------------------
#define ADC_REFER_VACC				_BV(REFS0)

//------------ADC��������---------------------
#define ADC_CHANNEL_0				0x0
#define ADC_CHANNEL_1				0x1
#define ADC_CHANNEL_2				0x2
#define ADC_CHANNEL_3				0x3
#define ADC_CHANNEL_4				0x4
#define ADC_CHANNEL_5				0x5
#define ADC_CHANNEL_6				0x6
#define ADC_CHANNEL_7				0x7

//-----------������ѹ���Ŷ���------------------
#define ADC_VOLTAGE					ADC_CHANNEL_1
#define ADC_CURRENT					ADC_CHANNEL_2

// ��ʶADCת����״̬
typedef struct
{
	bool is_current_convert;			// ��ʶ���ڽ��е���ת��(true), ���ǵ�ѹת��(false)
	bool is_first_time;					// ��ʶ�˴�ת������Ƿ����л�ͨ����ĵ�һ��ת�����
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
extern unsigned char nadc_mean_num;	// �ڻ�����ƽ�ؾ����㷨��, ʱ�䴰�Ĵ�С,(��������Ķ��ٸ�ֵ��ƽ��)
extern AdcQueue adc_queue;			// �����ڻ��ζ���, ���������ת�����

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
