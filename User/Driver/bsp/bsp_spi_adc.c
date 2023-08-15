/***********************************************************
* ģ������bsp_spi_adc.c
* ���ߣ�  
*
*
************************************************************/
#include	"bsp.h"

/***********************************************************
** ��������MCP3204_SPI_Init
** �䡡�룺None  
** �䡡����None
** ����������Initialize the SPI controller for the AD.
** ȫ�ֱ�����None
** ����ģ�飺None
************************************************************/
void MCP3204_SPI_Init(void)
{
	SPI_InitTypeDef  SPI_InitStructure;

	SPI_Cmd(AD_SPI, DISABLE);

	/* SPI configuration */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;

	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;//SPI speed: 328.125KHz
	
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(AD_SPI, &SPI_InitStructure);

	/* Enable the MCP3204_SPI  */
	SPI_Cmd(AD_SPI, ENABLE);
}
/***********************************************************
** ��������MCP3204_Init
** �䡡�룺None  
** �䡡����None
** ����������Initialize the SPI controller for the AD.
** ȫ�ֱ�����None
** ����ģ�飺None
************************************************************/
void MCP3204_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(AD_GPIO_RCC 
							| AD_CS_GPIO_RCC,
							ENABLE); 


	RCC_APB2PeriphClockCmd(AD_SPI_RCC, ENABLE); 

	//����SPI���Ÿ��ù���
	GPIO_PinAFConfig(AD_SPI_PORT, AD_SPI_SCK_SOURCE, AD_SPI_GPIO_AF);
	GPIO_PinAFConfig(AD_SPI_PORT, AD_SPI_MISO_SOURCE, AD_SPI_GPIO_AF);
	GPIO_PinAFConfig(AD_SPI_PORT, AD_SPI_MOSI_SOURCE, AD_SPI_GPIO_AF);

	/* Configure AD_SPI_SCK_PIN in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = AD_SPI_SCK_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	//GPIO_PuPd_DOWN;
	GPIO_Init(AD_SPI_PORT, &GPIO_InitStructure);

	/* Configure AD_SPI_MISO_PIN in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = AD_SPI_MISO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(AD_SPI_PORT, &GPIO_InitStructure);

	/* Configure AD_SPI_MOSI_PIN in input no pull mode */
	GPIO_InitStructure.GPIO_Pin = AD_SPI_MOSI_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(AD_SPI_PORT, &GPIO_InitStructure);

	/* Configure AD_SPI_CS_PIN in output nopull mode */
	GPIO_InitStructure.GPIO_Pin = AD_SPI_CS_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(AD_SPI_CS_PORT, &GPIO_InitStructure);

	MCP3204_CS_HIGH();
	MCP3204_SPI_Init();
}
/***********************************************************
* �������ƣ�SPI_MCP3204_ReadWriteByte
* ����������ͨ��SPI1д�Ĵ���
* ��ڲ����� - TxData: Write Byte
* ���ڲ�������
* ʹ��˵������
************************************************************/
static u16  SPI_MCP3204_ReadWriteByte(u16 TxData)
{
	u32	i;
	
	/* Loop while DR register in not emplty */
	i = 0;
	while ((SPI_I2S_GetFlagStatus(AD_SPI, SPI_I2S_FLAG_TXE) == RESET) && (i++<65536));

	/* Send byte through the SPI peripheral */
	SPI_I2S_SendData(AD_SPI, TxData);

	/* Wait to receive a byte */
	i = 0;
	while ((SPI_I2S_GetFlagStatus(AD_SPI, SPI_I2S_FLAG_RXNE) == RESET) && (i++<65536));

	/* Return the byte read from the SPI bus */
	return SPI_I2S_ReceiveData(AD_SPI);
}
/***********************************************************
* �������ƣ�MCP3204_Write
* ����������д�Ĵ���
* ��ڲ�����
* ���ڲ�������
* ʹ��˵������
************************************************************/
void MCP3204_Write(u8 value)
{      
   SPI_MCP3204_ReadWriteByte(value);   	               
}
/***********************************************************
* �������ƣ�MCP3204_Read
* ����������
* ��ڲ�����
* ���ڲ�������
* ʹ��˵������
************************************************************/
u8 MCP3204_Read(u8 data)
{ 
	u16	value;
	value = SPI_MCP3204_ReadWriteByte(data);		
	return value; 
}

u16 Calc_MCP3204_Value(uint8_t addr,u8 i)
{
	u16 temp1;
	u16 temp2;
	u16	Sum=0;
	
	// Ƭѡʹ��
	MCP3204_CS_LOW();
	temp1 = MCP3204_Read(0x06);		//������ͨ������
	temp1 = MCP3204_Read(addr);		//����ͨ��������12λAD������ֽ�
	temp2 = MCP3204_Read(0x00);		//ʱ�򱣳֣�����12λAD������ֽ�
	// Ƭѡʧ��	
	MCP3204_CS_HIGH();
	Sum = ((temp1 << 8) | temp2) & 0x0FFF; 	//�������AD�����ȡ12λADת��ֵ
	
	return Sum;
}

#define PRESS_COEF    1000/819		// x * 2 * 2500 / 4095	//ʵ�ʵ�ѹ�����룬�ο���ѹ2.5V

/***********************************************************
* �������ƣ�MCP3204_Value
* ����������
* ��ڲ�����
* ���ڲ�������
* ʹ��˵������
************************************************************/
u16 sensorValue[4][10] = {0};
u8 sensorCalcCount[4] = {0};
u16 sensortmpvalue = 0;
u16 MCP3204_Value(u8 channel, u16 *value)
{
	u16	res=0,iTemp = 0,iPos;
	u8 i,j;
	switch(channel)
	{
		case Analog1:
			sensorValue[channel][sensorCalcCount[channel]] = Calc_MCP3204_Value(0x00,0);
			sensorCalcCount[channel]++;
		break;
		case Analog2:
			sensorValue[channel][sensorCalcCount[channel]] = Calc_MCP3204_Value(0x40,1);
			sensorCalcCount[channel]++;
		break;
		case Analog3:
			sensorValue[channel][sensorCalcCount[channel]] = Calc_MCP3204_Value(0x80,2);
			sensorCalcCount[channel]++;
		break;
		case Analog4:
			sensorValue[channel][sensorCalcCount[channel]] = Calc_MCP3204_Value(0xC0,3);
			sensorCalcCount[channel]++;

		break;
		default:
		break;
	}
	
	if(sensorCalcCount[channel] == 9)
	{			
		// ��С��������
		for(i=0; i<9; i++)                              // �������ѭ��Ϊ�±� 0~8 ��Ԫ��
		{
			iTemp = sensorValue[channel][i];                               // ���õ�ǰԪ�ص���Сֵ
			iPos = i;                                   // ��¼��ǰԪ�ص�λ��
			for(j=i+1; j<10; j++)                       // �����ڲ�ѭ��Ϊ i+1 ~ 9
			{
				if(sensorValue[channel][j] < iTemp)                        // �����ǰԪ�ر���Сֵ��С
				{
					iTemp = sensorValue[channel][j];                       // ����������Сֵ
					iPos = j;                           // ��¼�µ�Ԫ��λ��
				}
			}
			// ��������Ԫ��ֵ
			sensorValue[channel][iPos] = sensorValue[channel][i];
			sensorValue[channel][i] = iTemp;

		}
		sensortmpvalue = 0;
		for(i=1; i<9; i++)  
			sensortmpvalue += sensorValue[channel][i];
		
		sensortmpvalue = sensortmpvalue/ 8*PRESS_COEF;
		sensorCalcCount[channel] = 0;
		*value = sensortmpvalue;
		res = 1;
	}
	return res;
}

