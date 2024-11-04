#include "settings.h"
#include <QMessageBox>

Settings::Settings(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("설정");
    serialPort = new QSerialPort(this);
    setupUI();
}

Settings::~Settings()
{
    // 필요한 경우 동적 할당된 메모리 해제
}

void Settings::setupUI()
{
    // 메인 레이아웃 설정
    mainLayout = new QVBoxLayout(this);
    
    // 좌측 메뉴 리스트
    menuList = new QListWidget;
    menuList->addItem(new QListWidgetItem(QIcon(":/icons/search.png"), "검색"));
    menuList->addItem(new QListWidgetItem(QIcon(":/icons/chart.png"), "그래프"));
    menuList->addItem(new QListWidgetItem(QIcon(":/icons/log.png"), "단축키"));
    menuList->addItem(new QListWidgetItem(QIcon(":/icons/log.png"), "로깅"));
    menuList->setFixedWidth(150);
    
    // 스택 위젯 생성
    contentStack = new QStackedWidget;
    
    // 검색 페이지 생성
    createSearchPage();
    
    // 수평 레이아웃에 메뉴와 컨텐츠 추가
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(menuList);
    hLayout->addWidget(contentStack);
    
    mainLayout->addLayout(hLayout);
    
    // 메뉴 선택 시 페이지 전환
    connect(menuList, &QListWidget::currentRowChanged,
            contentStack, &QStackedWidget::setCurrentIndex);
}

void Settings::createSearchPage()
{
    searchPage = new QWidget;
    searchLayout = new QVBoxLayout(searchPage);
    
    // 프로토콜 그룹
    protocolGroup = new QGroupBox("프로토콜을 선택하세요.");
    QVBoxLayout *protocolLayout = new QVBoxLayout;
    mrsBox = new QCheckBox("MRS Protocol");
    mrsBox->setChecked(true); // 기본값으로 체크된 상태로 설정
    protocolLayout->addWidget(mrsBox);
    protocolGroup->setLayout(protocolLayout);
    
    // 포트 그룹
    portGroup = new QGroupBox("연결할 포트를 선택하세요.");
    QVBoxLayout *portLayout = new QVBoxLayout;
    portCombo = new QComboBox;
    
    // 새로고침 버튼 추가
    QPushButton *refreshBtn = new QPushButton("새로고침");
    connect(refreshBtn, &QPushButton::clicked, this, &Settings::refreshPortList);
    
    QHBoxLayout *portControlLayout = new QHBoxLayout;
    portControlLayout->addWidget(portCombo);
    portControlLayout->addWidget(refreshBtn);
    
    portLayout->addLayout(portControlLayout);
    portGroup->setLayout(portLayout);
    
    // 초기 포트 목록 로드
    refreshPortList();
    
    // 통신속도 그룹
    baudRateGroup = new QGroupBox("연결할 통신속도를 선택해주세요.");
    QVBoxLayout *baudLayout = new QVBoxLayout;
    baudRateCombo = new QComboBox;
    baudRateCombo->addItem("9600 bps");
    baudRateCombo->addItem("57600 bps");
    baudRateCombo->addItem("115200 bps");
    baudRateCombo->addItem("1000000 bps");
    baudRateCombo->setCurrentText("1000000 bps"); // 1000000 bps를 기본값으로 설정
    baudLayout->addWidget(baudRateCombo);
    baudRateGroup->setLayout(baudLayout);
    
    // ID 범위 그룹 수정
    idRangeGroup = new QGroupBox("검색할 ID의 범위를 설정하세요.");
    QGridLayout *idLayout = new QGridLayout;
    QLabel *startLabel = new QLabel("시작:");
    QLabel *endLabel = new QLabel("끝:");
    startIdSpin = new QSpinBox;
    endIdSpin = new QSpinBox;
    
    // QRangeSlider 대신 QSlider 두 개 사용
    startIdSlider = new QSlider(Qt::Horizontal);
    endIdSlider = new QSlider(Qt::Horizontal);
    
    startIdSpin->setRange(0, 255);
    endIdSpin->setRange(0, 255);
    endIdSpin->setValue(20);
    
    startIdSlider->setRange(0, 255);
    endIdSlider->setRange(0, 255);
    endIdSlider->setValue(20);
    
    // startIdSpin과 slider 연동
    connect(startIdSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            startIdSlider, &QSlider::setValue);
    connect(startIdSlider, &QSlider::valueChanged,
            startIdSpin, &QSpinBox::setValue);
            
    // endIdSpin과 slider 연동
    connect(endIdSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            endIdSlider, &QSlider::setValue);
    connect(endIdSlider, &QSlider::valueChanged,
            endIdSpin, &QSpinBox::setValue);
    
    // 시작값이 끝값을 넘지 않도록 제한
    connect(startIdSlider, &QSlider::valueChanged, this, [this](int value) {
        if (value > endIdSlider->value()) {
            endIdSlider->setValue(value);
        }
    });
    
    connect(endIdSlider, &QSlider::valueChanged, this, [this](int value) {
        if (value < startIdSlider->value()) {
            startIdSlider->setValue(value);
        }
    });
    
    idLayout->addWidget(startLabel, 0, 0);
    idLayout->addWidget(startIdSpin, 0, 1);
    idLayout->addWidget(endLabel, 0, 2);
    idLayout->addWidget(endIdSpin, 0, 3);
    idLayout->addWidget(startIdSlider, 1, 0, 1, 2);
    idLayout->addWidget(endIdSlider, 1, 2, 1, 2);
    idRangeGroup->setLayout(idLayout);

    // 버튼
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    connectBtn = new QPushButton("연결");
    buttonLayout->addWidget(connectBtn);
    buttonLayout->addStretch();
    confirmBtn = new QPushButton("확인");
    cancelBtn = new QPushButton("취소");
    buttonLayout->addWidget(confirmBtn);
    buttonLayout->addWidget(cancelBtn);
    
    // 버튼 연결 추가
    connect(connectBtn, &QPushButton::clicked, this, &Settings::handleConnect);
    
    // 레이아웃에 추가
    searchLayout->addWidget(protocolGroup);
    searchLayout->addWidget(portGroup);
    searchLayout->addWidget(baudRateGroup);
    searchLayout->addWidget(idRangeGroup);    
    searchLayout->addLayout(buttonLayout);
    
    contentStack->addWidget(searchPage);
}

void Settings::refreshPortList()
{
    // 현재 포트 목록 저장
    QStringList oldPorts;
    for (int i = 1; i < portCombo->count(); i++) { // 0번은 "전체 선택"이므로 제외
        oldPorts << portCombo->itemText(i).split(" (").first(); // 설명 부분 제외하고 포트 이름만 저장
    }

    portCombo->clear();
    portCombo->addItem("전체 선택");
    
    // 시스템에 연결된 모든 COM 포트 검색
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    
    // 포트 이름을 기준으로 정렬하기 위한 맵 생성
    QMap<int, QSerialPortInfo> sortedPorts;
    for(const QSerialPortInfo &port : ports) {
        // 포트 이름에서 숫자만 추출
        QString numStr = port.portName();
        numStr.replace(QRegularExpression("[^0-9]"), "");
        int num = numStr.toInt();  // 문자열을 정수로 변환
        sortedPorts.insert(num, port);
    }
    
    // 정렬된 포트 정보를 콤보박스에 추가
    for(const QSerialPortInfo &port : sortedPorts.values()) {
        QString portName = port.portName();
        QString description = port.description();
        QString displayText = portName;
        
        if(!description.isEmpty()) {
            displayText += QString(" (%1)").arg(description);
        }
        
        // 새로 추가된 포트인 경우 강조 표시
        QStandardItem* item = new QStandardItem(displayText);
        if (!oldPorts.contains(portName)) {
            QFont font = item->font();
            font.setBold(true);
            item->setFont(font);
            item->setForeground(QBrush(QColor("#2196F3"))); // 파란색으로 강조
        }
        
        ((QStandardItemModel*)portCombo->model())->appendRow(item);
    }
}

void Settings::handleConnect()
{
    if (serialPort->isOpen()) {
        serialPort->close();
        connectBtn->setText("연결");
        QMessageBox::information(this, "알림", "포트 연결이 해제되었습니다.");
        return;
    }

    QString selectedPort = portCombo->currentText().split(" (").first();
    if (selectedPort == "전체 선택") {
        QMessageBox::warning(this, "경고", "포트를 선택해주세요.");
        return;
    }

    serialPort->setPortName(selectedPort);
    
    // 통신속도 설정
    QString baudRateStr = baudRateCombo->currentText();
    baudRateStr.replace(" bps", "");
    int baudRate = baudRateStr.toInt();
    serialPort->setBaudRate(baudRate);
    
    // 기본 설정
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (serialPort->open(QIODevice::ReadWrite)) {
        connectBtn->setText("연결 해제");
        QMessageBox::information(this, "성공", "포트가 성공적으로 연결되었습니다.");
    } else {
        QMessageBox::critical(this, "오류", "포트 연결에 실패했습니다: " + serialPort->errorString());
    }
}

