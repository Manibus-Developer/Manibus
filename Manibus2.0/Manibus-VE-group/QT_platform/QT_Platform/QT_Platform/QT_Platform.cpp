#include "QT_Platform.h"
#include <stdio.h>
#include <tchar.h>
#include <QDebug>

QT_Platform::QT_Platform(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	port_refresh = new QTimer();
	connect(port_refresh,SIGNAL(timeout()),this,SLOT(serial_dector()));
	port_refresh->start(2000);

	all_Init();

	serial_.SerialWaitFeedBack = true;   //开启接受反馈

	connect(&refresh_params, SIGNAL(timeout()), this, SLOT(adc_refresh()));


	connect(ui.pB_Init,SIGNAL(released()),this,SLOT(set_Init()));

	ui.param_show1->setText(QString::number(wash_time_h)+":"+ QString::number(wash_time_m) +" min");
	ui.param_show3->setText(QString::number(0) + ":" + QString::number(0) + " min");
	ui.param_show5->setText((QString::number(weightofwashing)+" g"));
	ui.param_show2->setText((QString::number(weightofmedice) + " g"));
	ui.param_show6->setText((QString::number(temperatureofwashing) + QStringLiteral(" ℃")));
	ui.param_show7->setText((QString::number(weightofbeenwashed) + " g"));
	ui.param_show4->setText((QString::number(presure_) + " kPa"));

	connect(ui.pB_switch2, SIGNAL(released()), this, SLOT(switch_2()));
	connect(ui.pB_switch4, SIGNAL(released()), this, SLOT(switch_4()));
	connect(ui.pB_switch1, SIGNAL(released()), this, SLOT(switch_1()));
	connect(ui.pB_switch3, SIGNAL(released()), this, SLOT(switch_3()));
	connect(ui.pB_switch5, SIGNAL(released()), this, SLOT(switch_5()));
	ui.lN_contral3->display(ntime_);
	ui.pB_switch4->setText(QStringLiteral("结束"));
}


void QT_Platform::serial_dector() {

	QString serial_portName;
	QString serial_systemLocation;
	QString serial_description;
	QString temp;


	foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {

		serial_portName = info.portName();
		serial_systemLocation = info.systemLocation();
		serial_description = info.description();

		QString tempOut = serial_portName + serial_systemLocation + serial_description;

		if (!flag) {

			ui.comboBox_port->addItem(serial_portName);
			flag = true;
		}
		else {

			for (char i = ui.comboBox_port->count(); i>1; i--) {

				if (serial_portName != ui.comboBox_port->itemText(i)) {
					ui.comboBox_port->addItem(serial_portName);
				}
			}

		}

		ui.textBrowser_info->setText(temp + tempOut);
		temp = tempOut;
	}
}

void QT_Platform::serial_connect() {

	if (serial_flag) {
	

		QString temp = ui.comboBox_port->currentText();
		std::string port = temp.toStdString();
		
		port_refresh->stop();

		ui.textBrowser_info->setText("Done!");
		serial_.Port = serial_.SerialInit(_T("COM5"),115200);
		ui.pB_connect->setText(QStringLiteral("断开"));

		serial_flag = false;
	}
	else {

		serial_.SerialClose(_T("COM5"));

		ui.textBrowser_info->setText("Shut Down!");
		port_refresh->start();
		ui.pB_connect->setText(QStringLiteral("链接"));
		serial_flag = true;
		ui.comboBox_port->clear();
		flag = false;

	}
}

void QT_Platform::all_Init(){

	QStringList Periph_putin = QStringList()
		<< tr("GPIO")<<tr("PWM")<<tr("ADC");

	ui.cB_Periph->addItems(Periph_putin);

	QStringList Fuc_putin = QStringList()
		<< tr("switch1") << tr("switch2") << tr("switch3")<<tr("switch4")
		<<tr("contral1") << tr("contral2") << tr("contral3")
		<<tr("param_show1") << tr("param_show2") << tr("param_show3") << tr("param_show4")<<tr("switch5");

	ui.cB_Surface->addItems(Fuc_putin);

	periphChange();

	ui.sB_param1->setEnabled(false);
	ui.sB_param2->setEnabled(false);


	ui.hS_contral1->setEnabled(false);

	ui.hS_contral3->setEnabled(true);


}

unsigned char choose_IO(short index) {

	switch (index)
	{
	case 0 :
		return GPIOA;
	case 1:
		return GPIOB;
	case 2:
		return GPIOC;
	case 3:
		return GPIOD;
	case 4:
		return GPIOE;
	default:
		break;
	}

}

unsigned char choose_Pin(short index) {

	switch (index) {
	case 0:
		return GPIO_Pin_0;
	case 1:
		return GPIO_Pin_1;
	case 2:
		return GPIO_Pin_2;
	case 3:
		return GPIO_Pin_3;
	case 4:
		return GPIO_Pin_4;
	case 5:
		return GPIO_Pin_5;
	case 6:
		return GPIO_Pin_6;
	case 7:
		return GPIO_Pin_7;
	case 8:
		return GPIO_Pin_8;
	case 9:
		return GPIO_Pin_9;
	case 10:
		return GPIO_Pin_10;
	case 11:
		return GPIO_Pin_11;
	case 12:
		return GPIO_Pin_12;
	case 13:
		return GPIO_Pin_13;
	case 14:
		return GPIO_Pin_14;
	case 15:
		return GPIO_Pin_15;
	default:
		break;
	}

}

unsigned char shooseMode(short index) {

	switch (index) {
	case 0:
		return GPIO_Mode_AIN;
	case 1:
		return GPIO_Mode_IN_FLOATING;
	case 2:
		return GPIO_Mode_IPD;
	case 3:
		return GPIO_Mode_IPU;
	case 4:
		return GPIO_Mode_Out_OD;
	case 5:
		return GPIO_Mode_Out_PP;
	case 6:
		return GPIO_Mode_AF_OD;
	case 7:
		return GPIO_Mode_AF_PP;
	default:
		break;
	}

}

unsigned char choose_PWM(short index) {

	switch (index) {
	case 0:
		return TIM1;
	case 1:
		return TIM2;
	case 2:
		return TIM3;
	case 3:
		return TIM4;
	case 4:
		return TIM5;
	case 5:
		return TIM8;
	default:
		break;
	}

}

unsigned char choose_Channel(short index) {

	switch (index) {
	
	case 0:
		return PWMOC_Channel_1;
	case 1:
		return PWMOC_Channel_2;
	case 2:
		return PWMOC_Channel_3;
	case 3:
		return PWMOC_Channel_4;
	case 4:
		return PWMIC_Channel_1;
	case 5:
		return PWMIC_Channel_2;
	default:

		break;
	}
}

ADCCHANNEL_Type choose_ADC(short index) {

	switch (index) {
	case 0:
		return ADC_Channel_0;
	case 1:
		return ADC_Channel_1;
	case 2:
		return ADC_Channel_2;
	case 3:
		return ADC_Channel_3;
	case 4:
		return ADC_Channel_4;
	case 5:
		return ADC_Channel_5;
	case 6:
		return ADC_Channel_6;
	case 7:
		return ADC_Channel_7;
	case 8:
		return ADC_Channel_8;
	case 9:
		return ADC_Channel_9;
	case 10:
		return ADC_Channel_10;
	case 11:
		return ADC_Channel_11;
	case 12:
		return ADC_Channel_12;
	case 13:
		return ADC_Channel_13;
	case 14:
		return ADC_Channel_14;
	case 15:
		return ADC_Channel_15;
	case 16:
		return ADC_Channel_16;
	case 17:
		return ADC_Channel_17;
	}

}

void QT_Platform::add_contral_adc(ADCCHANNEL_Type channel) {

	if (adc_param_count == 0) { adc_param_show[adc_param_count] = channel; adc_param_count++; return; }
	for (short i = 0; i < adc_param_count;i++) {
	
		if (channel == adc_param_show[i]) {
			return;
		}
	}

	adc_param_show[adc_param_count] = channel;
	adc_param_count++;

}

void QT_Platform::add_contral_channel(unsigned char tim, unsigned char channel) {

	char temp = contral_tim[tim];

	if (temp >= 1) {
	
		for (char i = 0; i < temp;i++) {
		
			if (contral_channelGroup[tim][i] == channel) { return; }
		}

		contral_channelGroup[tim][temp] = channel;
		contral_tim[tim]++;

	}
	else {
		contral_tim[tim] = 1;
		contral_channelGroup[tim][0] = channel;
		contral_channelGroup[tim][1] = OC_None;
		contral_channelGroup[tim][2] = OC_None;
		contral_channelGroup[tim][3] = OC_None;
	}


}

void QT_Platform::periphChange() {

	if (ui.cB_Periph->currentText() == "GPIO") {

		QStringList Port_putin = QStringList()
			<< tr("GPIOA") << tr("GPIOB") << tr("GPIOC") << tr("GPIOD") << tr("GPIOE");
		ui.cB_param1->clear();
		ui.cB_param1->addItems(Port_putin);
		QStringList IO_putin = QStringList()
			<< tr("Pin0") << tr("Pin1") << tr("Pin2") << tr("Pin3") << tr("Pin4")
			<< tr("Pin5") << tr("Pin6") << tr("Pin7") << tr("Pin8") << tr("Pin9")
			<< tr("Pin10") << tr("Pin11") << tr("Pin12") << tr("Pin13") << tr("Pin14") << tr("Pin15");
		ui.cB_param2->clear();
		ui.cB_param2->addItems(IO_putin);

		QStringList Mode_putin = QStringList()
			<< tr("AIN") << tr("IN_FLOATING") << tr("IPD") << tr("IPU") << tr("Out_OD") << tr("Out_PP")
			<< tr("AF_OD") << tr("AF_PP");
		ui.cB_param3->clear();
		ui.cB_param3->addItems(Mode_putin);

		ui.sB_param1->setEnabled(false);
		ui.sB_param2->setEnabled(false);
		ui.lb_sB_param1->setText(QStringLiteral("参数1"));
		ui.lb_sB_param2->setText(QStringLiteral("参数2"));


	}
	else if (ui.cB_Periph->currentText() == "PWM") {

		QStringList PWM_putin = QStringList()
			<< tr("TIM1") << tr("TIM2") << tr("TIM3") << tr("TIM4") << tr("TIM5") << tr("TIM8");
		ui.cB_param1->clear();
		ui.cB_param1->addItems(PWM_putin);

		QStringList Channel_putin = QStringList()
			<< tr("OC_Channel1") << tr("OC_Channel2") << tr("OC_Channel3") << tr("OC_Channel4")
			<< tr("IC_Channel1") << tr("IC_Channel2");

		ui.cB_param2->clear();
		ui.cB_param2->addItems(Channel_putin);

		ui.cB_param3->clear();

		ui.sB_param1->setEnabled(true);
		ui.sB_param2->setEnabled(true);

		ui.lb_sB_param1->setText(QStringLiteral("预分频"));
		ui.lb_sB_param2->setText(QStringLiteral("占空比"));

	}
	else if (ui.cB_Periph->currentText() == "ADC") {
	
		QStringList ADCChannel_putin = QStringList()
			<< tr("ADC1_Channe0") << tr("ADC1_Channel") << tr("ADC1_Channe2") << tr("ADC1_Channe3") << tr("ADC1_Channe4")
			<< tr("ADC1_Channe5") << tr("ADC1_Channe6") << tr("ADC1_Channe7") << tr("ADC1_Channe8") << tr("ADC1_Channe9")
			<< tr("ADC1_Channe10") << tr("ADC1_Channe11") << tr("ADC1_Channe12") << tr("ADC1_Channe13") << tr("ADC1_Channe14")
			<< tr("ADC1_Channe15") << tr("ADC1_Channe16") << tr("ADC1_Channe17");
		ui.cB_param1->clear();
		ui.cB_param1->addItems(ADCChannel_putin);

		QStringList TIM_Get_putin = QStringList()
			<< tr("100") << tr("200") << tr("500") << tr("1000") << tr("1500")
			<< tr("2000");
		ui.cB_param2->clear();
		ui.cB_param2->addItems(TIM_Get_putin);
	
	}

}


void QT_Platform::set_Config() {

	short Surface_ = ui.cB_Surface->currentIndex();

	switch (Surface_) {

	case 0 : //"switch1"


		switch_[0][0] = choose_IO(ui.cB_param1->currentIndex());
		switch_[0][1] = choose_Pin(ui.cB_param2->currentIndex());
		
		serial_.pPinInit(serial_.Port, (GPIOIO_Type)switch_[0][0], (GPIOPIN_Type)switch_[0][1], GPIO_Mode_Out_PP, GPIO_Speed_50MHz);

		break;
	case 1://"switch2"

		switch_[1][0] = choose_IO(ui.cB_param1->currentIndex());
		switch_[1][1] = choose_Pin(ui.cB_param2->currentIndex());
		
		serial_.pPinInit(serial_.Port, (GPIOIO_Type)switch_[1][0], (GPIOPIN_Type)switch_[1][1], GPIO_Mode_Out_PP, GPIO_Speed_50MHz);
		break;
	case 2://"switch3"

		switch_[2][0] = choose_IO(ui.cB_param1->currentIndex());
		switch_[2][1] = choose_Pin(ui.cB_param2->currentIndex());

		serial_.pPinInit(serial_.Port, (GPIOIO_Type)switch_[2][0], (GPIOPIN_Type)switch_[2][1], GPIO_Mode_Out_PP, GPIO_Speed_50MHz);
		break;


	case 3://"switch4"


		switch_[3][0] = choose_IO(ui.cB_param1->currentIndex());
		switch_[3][1] = choose_Pin(ui.cB_param2->currentIndex());
		
		serial_.pPinInit(serial_.Port, (GPIOIO_Type)switch_[3][0], (GPIOPIN_Type)switch_[3][1], GPIO_Mode_Out_PP, GPIO_Speed_50MHz);
		break;

	case 11://"switch5"


		switch_[4][0] = choose_IO(ui.cB_param1->currentIndex());
		switch_[4][1] = choose_Pin(ui.cB_param2->currentIndex());

		serial_.pPinInit(serial_.Port, (GPIOIO_Type)switch_[4][0], (GPIOPIN_Type)switch_[4][1], GPIO_Mode_Out_PP, GPIO_Speed_50MHz);
		break;
	case 4://"contral1"


		if (ui.cB_Periph->currentText() == "GPIO") {


			contral_[0][0] = choose_IO(ui.cB_param1->currentIndex());
			contral_[0][1] = choose_Pin(ui.cB_param2->currentIndex());
			serial_.pPinInit(serial_.Port, (GPIOIO_Type)contral_[0][0], (GPIOPIN_Type)contral_[0][1], GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
		}
		else if (ui.cB_Periph->currentText() == "PWM") {


			contral_[0][2] = choose_PWM(ui.cB_param1->currentIndex());
			
			contral_[0][3] = choose_Channel(ui.cB_param2->currentIndex());

			add_contral_channel(ui.cB_param1->currentIndex(), contral_[0][3]);

			contral_[0][4] = ui.cB_param1->currentIndex();

			unsigned short param1_ = ui.sB_param1->value();
			unsigned short param2_ = ui.sB_param2->value();

			serial_.pPWMInit( serial_.Port, (PWMTIM_Type)contral_[0][2], (PWMOCCHANNEL_Type *)contral_channelGroup[contral_[0][4]],800, param1_, param2_);
			ui.hS_contral1->setEnabled(true);
			ui.hS_contral1->setValue(0);
		}
	
		break;

	case 5://"contral2"



		if (ui.cB_Periph->currentText() == "GPIO") {

			contral_[1][0] = choose_IO(ui.cB_param1->currentIndex());
			contral_[1][1] = choose_Pin(ui.cB_param2->currentIndex());
			serial_.pPinInit(serial_.Port, (GPIOIO_Type)contral_[1][0], (GPIOPIN_Type)contral_[1][1], GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
		}
		else if (ui.cB_Periph->currentText() == "PWM") {


			contral_[1][2] = choose_PWM(ui.cB_param1->currentIndex());

			contral_[1][3] = choose_Channel(ui.cB_param2->currentIndex());
			add_contral_channel(ui.cB_param1->currentIndex(), contral_[1][3]);
			contral_[1][4] = ui.cB_param1->currentIndex();

			unsigned short param1_ = ui.sB_param1->value();
			unsigned short param2_ = ui.sB_param2->value();


			serial_.pPWMInit(serial_.Port, (PWMTIM_Type)contral_[1][2], (PWMOCCHANNEL_Type *)contral_channelGroup[contral_[1][4]], 800, param1_, param2_);
			//ui.hS_contral2->setEnabled(true);
			//ui.hS_contral2->setValue(0);
		}
		break;

	case 6://"contral3"


		if (ui.cB_Periph->currentText() == "GPIO") {

			ui.sB_param1->setEnabled(false);
			ui.sB_param2->setEnabled(false);
			contral_[2][0] = choose_IO(ui.cB_param1->currentIndex());
			contral_[2][1] = choose_Pin(ui.cB_param2->currentIndex());
			serial_.pPinInit(serial_.Port, (GPIOIO_Type)contral_[2][0], (GPIOPIN_Type)contral_[2][1], GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
		}
		else if (ui.cB_Periph->currentText() == "PWM") {

			ui.sB_param1->setEnabled(true);
			ui.sB_param2->setEnabled(true);


			contral_[2][2] = choose_PWM(ui.cB_param1->currentIndex());

			contral_[2][3] = choose_Channel(ui.cB_param2->currentIndex());
			add_contral_channel(ui.cB_param1->currentIndex(), contral_[2][3]);
			contral_[2][4] = ui.cB_param1->currentIndex();

			unsigned short param1_ = ui.sB_param1->value();
			unsigned short param2_ = ui.sB_param2->value();

			serial_.pPWMInit(serial_.Port, (PWMTIM_Type)contral_[2][2], (PWMOCCHANNEL_Type *)contral_channelGroup[contral_[2][4]], 800, param1_, param2_);
			ui.hS_contral3->setEnabled(true);
			ui.hS_contral3->setValue(0);
		}

		break;

	default:
		set_Config2();
		break;
	}

}

void QT_Platform::set_Config2() {

	short Surface_ = ui.cB_Surface->currentIndex();

	switch (Surface_) {

	case 7:
		if (ui.cB_Periph->currentText() == "GPIO") {

			ui.sB_param1->setEnabled(false);
			ui.sB_param2->setEnabled(false);
			serial_.pPinInit(serial_.Port, (GPIOIO_Type)choose_IO(ui.cB_param1->currentIndex()), (GPIOPIN_Type)choose_Pin(ui.cB_param2->currentIndex()), GPIO_Mode_AIN, GPIO_Speed_50MHz);
		}
		else if(ui.cB_Periph->currentText() == "ADC"){
		
			ui.sB_param1->setEnabled(false);
			ui.sB_param2->setEnabled(false);
			
			add_contral_adc(choose_ADC(ui.cB_param1->currentIndex()));
			ADCCHANNEL_Type * temp;
			memcpy(temp, adc_param_show, adc_param_count);

			serial_.pADC_Init(serial_.Port, (ADCSPEED_Type)ADC_9MHz, (ADCCHANNEL_Type*)temp);
			adc_paramrefresh[0] = 1;
			
		}
		break;

	case 8:
		if (ui.cB_Periph->currentText() == "GPIO") {

			ui.sB_param1->setEnabled(false);
			ui.sB_param2->setEnabled(false);
			serial_.pPinInit(serial_.Port, (GPIOIO_Type)choose_IO(ui.cB_param1->currentIndex()), (GPIOPIN_Type)choose_Pin(ui.cB_param2->currentIndex()), GPIO_Mode_AIN, GPIO_Speed_50MHz);
			
		}
		else if (ui.cB_Periph->currentText() == "ADC") {

			ui.sB_param1->setEnabled(false);
			ui.sB_param2->setEnabled(false);

			add_contral_adc(choose_ADC(ui.cB_param1->currentIndex()));

			ADCCHANNEL_Type * temp;
			memcpy(temp, adc_param_show, adc_param_count);

			serial_.pADC_Init(serial_.Port, (ADCSPEED_Type)ADC_9MHz, (ADCCHANNEL_Type*)temp);
			adc_paramrefresh[1] = 1;
		}
		break;

	}
}

void QT_Platform::switch_1() {

	static bool switch_status = true;
	switch_status = !switch_status;

	if (switch_status) {

		serial_.pPinInit(serial_.Port, (GPIOIO_Type)GPIOA, GPIO_Pin_6, GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
		PWMOCCHANNEL_Type temp[4] = { PWMOC_Channel_1,OC_None,OC_None,OC_None };
		serial_.pPWMInit(serial_.Port, (PWMTIM_Type)TIM3, temp, 800, 14, 400);

		//serial_.pPinOutPut(serial_.Port, (GPIOIO_Type)switch_[0][0], (GPIOPIN_Type)switch_[0][1], Bit_LOW);
		ui.pB_switch1->setText(QStringLiteral("关闭"));
	}
	else {
		serial_.pPinInit(serial_.Port, (GPIOIO_Type)GPIOA, GPIO_Pin_6, GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
		PWMOCCHANNEL_Type temp[4] = { PWMOC_Channel_1,OC_None,OC_None,OC_None };
		//serial_.pPinOutPut(serial_.Port, (GPIOIO_Type)switch_[0][0], (GPIOPIN_Type)switch_[0][1], Bit_HIGH);
		serial_.pPWMInit(serial_.Port, (PWMTIM_Type)TIM3, temp, 800, 14, 800);
		ui.pB_switch1->setText(QStringLiteral("开始"));
	
	}

}

void QT_Platform::switch_2() {

	static bool switch_status = true;
	switch_status = !switch_status;

	if (switch_status) {
		serial_.pPinOutPut(serial_.Port, (GPIOIO_Type)switch_[1][0], (GPIOPIN_Type)switch_[1][1], Bit_LOW);
		ui.pB_switch2->setText(QStringLiteral("打开"));
	}
	else {
		serial_.pPinOutPut(serial_.Port, (GPIOIO_Type)switch_[1][0], (GPIOPIN_Type)switch_[1][1], Bit_HIGH);
		ui.pB_switch2->setText(QStringLiteral("已打开"));

	}
	serial_.pPinInit(serial_.Port, GPIOB, GPIO_Pin_0, GPIO_Mode_Out_PP, GPIO_Speed_50MHz);
	serial_.pPinOutPut(serial_.Port, GPIOB, GPIO_Pin_0, Bit_LOW);

	refresh_params.start(1000);
	flag_temp = true;
	contral1_adjust();
}

void QT_Platform::switch_3() {

	static bool switch_status = false;
	switch_status = !switch_status;

	//if (switch_status) {
	//	serial_.pPinOutPut(serial_.Port, (GPIOIO_Type)switch_[2][0], (GPIOPIN_Type)switch_[2][1], Bit_LOW);
	//	ui.pB_switch3->setText(QStringLiteral("打开"));
	//}
	//else {
	//	serial_.pPinOutPut(serial_.Port, (GPIOIO_Type)switch_[2][0], (GPIOPIN_Type)switch_[2][1], Bit_HIGH);
	//	ui.pB_switch3->setText(QStringLiteral("关闭"));

	//}

	serial_.pPinDeInit(serial_.Port,GPIOA);
	serial_.pPinDeInit(serial_.Port,GPIOB);


}

void QT_Platform::switch_4() {

	static bool switch_status = true;
	switch_status = !switch_status;

	if (switch_status) {
	//	serial_.pPinOutPut(serial_.Port, (GPIOIO_Type)switch_[3][0], (GPIOPIN_Type)switch_[3][1], Bit_LOW);
		ui.pB_switch4->setText(QStringLiteral("结束"));
	}
	else {
	//	serial_.pPinOutPut(serial_.Port, (GPIOIO_Type)switch_[3][0], (GPIOPIN_Type)switch_[3][1], Bit_HIGH);
		ui.pB_switch4->setText(QStringLiteral("已结束"));

	}
	PWMOCCHANNEL_Type temp[4] = { PWMOC_Channel_1,OC_None,OC_None,OC_None };
	serial_.pPWMInit(serial_.Port, (PWMTIM_Type)TIM4, temp, 800, 14, 800);
	refresh_params.stop();
	serial_.pPinOutPut(serial_.Port, GPIOB, GPIO_Pin_0, Bit_HIGH);
	flag_temp = false;

}
void QT_Platform::switch_5() {

	static bool switch_status = true;
	switch_status = !switch_status;

	if (switch_status) {
		serial_.pPinOutPut(serial_.Port, (GPIOIO_Type)GPIOB, GPIO_Pin_5, Bit_LOW);
		ui.pB_switch5->setText(QStringLiteral("关闭"));
	}
	else {
		serial_.pPinOutPut(serial_.Port, (GPIOIO_Type)GPIOB, GPIO_Pin_5, Bit_HIGH);
		ui.pB_switch5->setText(QStringLiteral("开始"));

	}
	refresh_params.stop();
}


void QT_Platform::contral1_adjust() {

	unsigned short value_ = ui.hS_contral1->value();
	unsigned short value_max = ui.hS_contral1->maximum();
	unsigned short true_value = abs(value_max - value_);

	ui.lN_contral1->display(value_);

	speed_value = true_value;
	if (!flag_temp)return;
	if (value_ ==0 ){
	
		serial_.pPWMSetPluseWid(serial_.Port, (PWMTIM_Type)contral_[0][2], (PWMOCCHANNEL_Type)contral_[0][3],800);

	}
	else {

		serial_.pPWMSetPluseWid(serial_.Port, (PWMTIM_Type)contral_[0][2], (PWMOCCHANNEL_Type)contral_[0][3], 400);
		serial_.pPWMSetFrequency(serial_.Port, (PWMTIM_Type)contral_[0][2], speed_value);

	}

}

void QT_Platform::contral2_adjust() {

	//unsigned short value_ = ui.hS_contral2->value();
	//unsigned short value_max = ui.hS_contral2->maximum();
	//unsigned short true_value = abs(value_max - value_);

	//ui.lN_contral2->display(value_);

	//if (value_ == 0) {

	//	serial_.pPWMSetPluseWid(serial_.Port, (PWMTIM_Type)contral_[1][2], (PWMOCCHANNEL_Type)contral_[1][3], 800);

	//}
	//else {

	//	serial_.pPWMSetPluseWid(serial_.Port, (PWMTIM_Type)contral_[1][2], (PWMOCCHANNEL_Type)contral_[1][3], 400);
	//	serial_.pPWMSetFrequency(serial_.Port, (PWMTIM_Type)contral_[1][2], true_value);

	//}

}

void QT_Platform::contral3_adjust() {

	unsigned short value_ = ui.hS_contral3->value();
	unsigned short value_max = ui.hS_contral3->maximum();
	unsigned short true_value = abs(value_max - value_);

	value_++;
	ui.lN_contral3->display(value_);

	//if (value_ == 0) {

	//	serial_.pPWMSetPluseWid(serial_.Port, (PWMTIM_Type)contral_[2][2], (PWMOCCHANNEL_Type)contral_[2][3], 800);

	//}
	//else {

	//	serial_.pPWMSetPluseWid(serial_.Port, (PWMTIM_Type)contral_[2][2], (PWMOCCHANNEL_Type)contral_[2][3], 400);
	//	serial_.pPWMSetFrequency(serial_.Port, (PWMTIM_Type)contral_[2][2], true_value);

	//}

}

void QT_Platform::adc_refresh() {


	wash_count++;
	wash_time_m = wash_time_m_get -(wash_count % 60);

	wash_time_h = wash_time_h_get - (wash_count / 60);

	unsigned short temp1 = wash_count % 60;
	unsigned short temp2 = wash_count / 60 ;
	ui.param_show1->setText(QString::number(wash_time_h) + ":" + QString::number(wash_time_m) + " min");
	ui.param_show3->setText(QString::number(temp2) + ":" + QString::number(temp1) + " min");

	weightofmedice -=  0.05;
	ui.param_show2->setText((QString::number(weightofmedice) + " g"));

	double temp3 = pow(-1, (rand() % 10)); 

	temperatureofwashing += temp3*(rand() % 60) / 100;

	if (temperatureofwashing>33.5) {
		temperatureofwashing = 33.5;
	}
	else if (temperatureofwashing<27.5) {

		temperatureofwashing = 27.5;
	}

	weightofbeenwashed += 0.05;
	ui.param_show6->setText((QString::number(temperatureofwashing) + QStringLiteral(" ℃")));
	ui.param_show7->setText((QString::number(weightofbeenwashed) + " g"));

	//if (adc_paramrefresh[0]) {
	//	float temp;

	//	temp = serial_.pADC_Read(serial_.Port, ADC_Channel_3);

	//	ui.param_show1->setText(QString::number(temp));

	//}
	//if (adc_paramrefresh[1]) {
	//	float temp;

	//	temp = serial_.pADC_Read(serial_.Port, ADC_Channel_10);

	//	ui.param_show2->setText(QString::number(temp));

	//}
}

void QT_Platform::set_Init() {

	//serial_.pPinInit(serial_.Port, GPIOA,GPIO_Pin_3, GPIO_Mode_AIN, GPIO_Speed_50MHz);

	//serial_.pPinInit(serial_.Port, GPIOC, GPIO_Pin_0, GPIO_Mode_AIN, GPIO_Speed_50MHz);

	//ADCCHANNEL_Type adc_channel[2];

	//adc_channel[0] = ADC_Channel_3;
	//adc_channel[1] = ADC_Channel_10;

	//serial_.pADC_Init(serial_.Port,ADC_9MHz, adc_channel);
	//adc_paramrefresh[0] = 1;
	//adc_paramrefresh[1] = 1;
	serial_.pPinInit(serial_.Port, GPIOB, GPIO_Pin_5, GPIO_Mode_Out_PP, GPIO_Speed_50MHz);
}