#include "demo.h"
#include <QtConcurrent/QtConcurrent>

demo::demo(QWidget *parent) : QWidget(parent) {
  ui.setupUi(this);

  qRegisterMetaType<BloodPressureRes>("BloodPressureRes");

  connect(ui.btn_start, &QPushButton::clicked, this, &demo::Btn_OnStartClicked);

  serial = new QSerialPort;

  serial->setPortName("COM3"); //待修改
  serial->setBaudRate(QSerialPort::Baud9600);
  serial->setParity(QSerialPort::NoParity);
  serial->setDataBits(QSerialPort::Data8);
  serial->setStopBits(QSerialPort::OneStop);
  serial->setFlowControl(QSerialPort::NoFlowControl);
  if (!serial->open(QIODevice::ReadWrite))
    QMessageBox::warning(this, "警告", "打开串口COM3失败！");

  // ui.textEdit_name->setAlignment(Qt::AlignCenter);
  // ui.plainTextEdit_id->setAlignment(Qt::AlignCenter);
}

demo::~demo() {}

void demo::Btn_OnStartClicked() {
  e_info.name = ui.textEdit_name->toPlainText();
  e_info.id = ui.plainTextEdit_id->toPlainText();

  this->hide();
  // start...
  // new 一个widget
  blood_pressure_widget = new BloodPressureWidget;
  blood_pressure_widget->setSerialPort(serial);
  blood_pressure_widget->show();

  connect(blood_pressure_widget, SIGNAL(onFinished(BloodPressureRes)), this,
          SLOT(OnBPFinished(BloodPressureRes)));
}

void demo::OnBPFinished(BloodPressureRes res) {
  delete blood_pressure_widget;
  bp_res = res;

  // cunpan
  // QtConcurrent::run(&demo::saveFile, );

  ecg_widget = new ECGWidget;
  ecg_widget->setSerialPort(serial);
  ecg_widget->show();
}

void demo::OnAllFinished() {
  // show the res widget
}
