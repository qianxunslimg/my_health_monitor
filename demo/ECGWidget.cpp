#include "ECGWidget.h"

ECGWidget::ECGWidget(QWidget *parent) : QWidget(parent) {
  ui.setupUi(this);

  qRegisterMetaType<ECGRes>("ECGRes");

  connect(ui.pushButton_start, &QPushButton::clicked, this,
          &ECGWidget::Btn_OnStartClicked);

  connect(ui.pushButton_finish, &QPushButton::clicked, this,
          &ECGWidget::Btn_OnFinishClicked);

  connect(ui.pushButton_stop, &QPushButton::clicked, this,
          &ECGWidget::Btn_OnStopClicked);
  connect(ui.pushButton_next, &QPushButton::clicked, this,
          &ECGWidget::Btn_OnNextClicked);

  stop_flag_ = 0;

  customPlot = new QCustomPlot(this);         // 创建 QCustomPlot 控件
  ui.horizontalLayout->addWidget(customPlot); // 将 QCustomPlot 添加到布局中
  customPlot->addGraph();
}

ECGWidget::~ECGWidget() {
  delete customPlot;
  int a = 10;
}

void ECGWidget::Btn_OnStartClicked() {
  stop_flag_ = 0;

  // clear all
  xData.clear();
  yData.clear();
  res.time.clear();
  res.ECG.clear();
  res.sum = 0;

  connect(serial_, &QSerialPort::readyRead, this, &ECGWidget::handleReadyRead);

  timer_.start(500); // 每隔0.5秒发送一次数据
  connect(&timer_, &QTimer::timeout, this, &ECGWidget::sendData);

  ui.pushButton_start->setText("正在测量");

  // 设置定时器，定时刷新绘图
  timer_plot_.start(100); // 每100毫秒刷新一次绘图
  connect(&timer_plot_, &QTimer::timeout, this, &ECGWidget::updatePlot);

  base_time_ = QDateTime::currentDateTime().toMSecsSinceEpoch() / 1000.0;
}

void ECGWidget::Btn_OnFinishClicked() {
  timer_.stop();
  timer_plot_.stop();

  ui.pushButton_start->setText("开始测量");
  QByteArray data = "S";
  serial_->write(data);
}

void ECGWidget::Btn_OnNextClicked() {
  timer_.stop();

  this->hide();

  //处理res 保存最好10s
  // 限制数据范围，只保留最近5秒的数据

  auto &xData = res.time;
  auto &yData = res.ECG;
  if (xData.size() != 0) {
    double timeLimit = 10;
    double timeend = xData.back();
    double timestart = timeend - timeLimit;
    while (!xData.isEmpty() && xData.first() < timestart) {
      xData.removeFirst();
      yData.removeFirst();
    }
  }

  emit(onFinished(res)); /////
}

void ECGWidget::Btn_OnStopClicked() {
  timer_.stop();
  timer_plot_.stop();
  stop_flag_ = 1;

  QByteArray data = "S";
  serial_->write(data);
}

void ECGWidget::handleReadyRead() {
  if (stop_flag_) {
    return;
  }

  // bool step1 = 0;
  // if (!step1) {
  //  QByteArray data = serial_->readAll(); // 读取接收到的
  //  std::string aa = data.toStdString();
  //  if (aa == "FF") {
  //    step1 = 1;
  //  }
  //}
  // if (step1 == 1) {
  //  QByteArray data = serial_->readAll(); // 读取接收到的
  //  std::string aa = data.toStdString();
  //  if (aa == "FF") {
  //    step1 = 1;
  //  }
  //}
  ////检查ff,
  // if () {
  //}

  // serial_->clear();
  QByteArray data = serial_->readAll(); // 读取接收到的数据
  qDebug() << "Received data:" << data;
  // 停止发送数据
  timer_.stop();

  double value = data.toDouble(); // 假设收到的数据是整数
  // 在这里可以根据需要处理接收到的数据

  // 将数据添加到绘图数据列表中
  double timestamp = QDateTime::currentDateTime().toMSecsSinceEpoch() /
                     1000.0; // 当前时间作为 x 轴数据（单位为秒）
  timestamp -= base_time_;

  xData.append(timestamp); // 将时间戳添加到 x 轴数据中
  yData.append(value);     // 接收到的数据作为 y 轴数据

  res.time.append(timestamp);
  res.ECG.append(value);

  // 限制数据范围，只保留最近5秒的数据
  double timeLimit = timestamp - 5.0; // 当前时间减去5秒
  while (!xData.isEmpty() && xData.first() < timeLimit) {
    xData.removeFirst();
    yData.removeFirst();
  }
}

void ECGWidget::sendData() {
  serial_->clear();
  QByteArray data = "Ppppppppppppppppppppp"; // 要发送的数据
  serial_->write(data);                      // 发送数据
}

void ECGWidget::updatePlot() {
  // 更新绘图
  if (xData.empty()) {
    return;
  }
  customPlot->graph(0)->setData(xData, yData); // 设置绘图数据
  customPlot->xAxis->setRange(xData.first(), xData.last()); // 设置 x 轴范围
  customPlot->rescaleAxes();
  customPlot->replot(); // 刷新绘图
}
