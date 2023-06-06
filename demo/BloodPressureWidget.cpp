#include "BloodPressureWidget.h"
#include <QMetaType>

BloodPressureWidget::BloodPressureWidget(QWidget *parent) : QWidget(parent) {
  ui.setupUi(this);

  qRegisterMetaType<BloodPressureRes>("BloodPressureRes");

  connect(ui.pushButton_start, &QPushButton::clicked, this,
          &BloodPressureWidget::Btn_OnStartClicked);
  connect(ui.pushButton_stop, &QPushButton::clicked, this,
          &BloodPressureWidget::Btn_OnStopClicked);
  connect(ui.pushButton_next, &QPushButton::clicked, this,
          &BloodPressureWidget::Btn_OnNextClicked);

  receiveMsg_ = 0;

  // QCustomPlot p;
  // p.show();
}

BloodPressureWidget::~BloodPressureWidget() {}

void BloodPressureWidget::Btn_OnStartClicked() {
  connect(serial_, &QSerialPort::readyRead, this,
          &BloodPressureWidget::handleReadyRead);

  timer_.start(500); // 每隔0.5秒发送一次数据
  connect(&timer_, &QTimer::timeout, this, &BloodPressureWidget::sendData);

  ui.pushButton_start->setText("正在测量");
}

void BloodPressureWidget::Btn_OnStopClicked() {
  ui.pushButton_start->setText("开始测量");
  timer_.stop();

  QByteArray data = "S";
  serial_->write(data);

  // emit(onFinished(res));
}

void BloodPressureWidget::Btn_OnNextClicked() {
  timer_.stop();

  this->hide();
  emit(onFinished(res)); /////
}

//接收到串口数据
void BloodPressureWidget::handleReadyRead() {
  QByteArray data = serial_->readAll(); // 读取接收到的数据
  qDebug() << "Received data:" << data;
  // 停止发送数据
  timer_.stop();

  int time = 0, sys = 0, dia = 0;
  ///
  res.time.push_back(time);
  res.systolic_pressure.push_back(sys);
  res.diastolic_pressure.push_back(dia);
}

void BloodPressureWidget::sendData() {
  QByteArray data = "P"; // 要发送的数据
  serial_->write(data);  // 发送数据
}
