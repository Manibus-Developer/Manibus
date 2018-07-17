#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QT_Platform.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include "MSerial.h"
#include <QStringListModel>


class QT_Platform : public QMainWindow
{
	Q_OBJECT

public:

	QT_Platform(QWidget *parent = Q_NULLPTR);


private:

	Ui::QT_PlatformClass ui;

	bool flag = false;
	QTimer *port_refresh;

	MSerial serial_;
	MGPIO mgpio_;
	bool serial_flag = true;


private:

	void all_Init();
	unsigned char switch_[4][3] = {0};
	unsigned char contral_[3][5]={ 0 };
	unsigned char contral_channelGroup[6][4] = { 0 };
	unsigned short contral_tim[6] = {0};

	ADCCHANNEL_Type adc_param_show[4];
	unsigned char  adc_param_count =0;
	unsigned char adc_paramrefresh[4] = {0};

	void add_contral_channel(unsigned char tim, unsigned char channel);
	void add_contral_adc(ADCCHANNEL_Type channel);

	QTimer refresh_params;

	unsigned short wash_time_h = 29;
	unsigned short wash_time_m = 59;

	unsigned short wash_time_h_get = wash_time_h;
	unsigned short wash_time_m_get = wash_time_m;
	unsigned short wash_count = 0;

	unsigned short weightofwashing = 200;
	float weightofmedice = 30.125;
	float temperatureofwashing = 30.5;
	double  weightofbeenwashed = 0;
	float presure_ = 0.08;
	char ntime_ = 0 ;
	bool flag_temp = false;
	char speed_value = 0;

private slots:

	void serial_dector();
	void serial_connect();
	void periphChange();

	void set_Config();
	void set_Config2();

	void switch_1();
	void switch_2();
	void switch_3();
	void switch_4();
	void switch_5();

	void contral1_adjust();
	void contral2_adjust();
	void contral3_adjust();

	void adc_refresh();

	void set_Init();

};
