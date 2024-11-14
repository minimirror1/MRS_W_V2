#include "firmwareupdate.h"

FirmwareUpdate::FirmwareUpdate(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("펌웨어 업데이트");
    setupUI();
    resize(500, 400);
}

FirmwareUpdate::~FirmwareUpdate()
{
}

void FirmwareUpdate::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // 프로그레스 프레임 추가
    progressFrame = new QFrame(this);
    progressFrame->setFixedHeight(40);
    progressFrame->setStyleSheet(
        "QFrame {"
        "background-color: #0078D7;"
        "border-radius: 10px;"
        "}"
    );
    
    // 프로그레스 라벨 추가
    progressLabel = new QLabel("펌웨어 업데이트", progressFrame);
    progressLabel->setStyleSheet("color: white;");
    progressLabel->setAlignment(Qt::AlignCenter);
    
    QHBoxLayout *frameLayout = new QHBoxLayout(progressFrame);
    frameLayout->addWidget(progressLabel);
    
    mainLayout->addWidget(progressFrame);
    
    // 테이블 생성
    createTable();
    mainLayout->addWidget(tableWidget);
    
    // 하단 버튼 레이아웃
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    scanButton = new QPushButton("스캔");
    selectAllButton = new QPushButton("전체 선택");
    nextButton = new QPushButton("다음");

    buttonLayout->addWidget(scanButton);
    buttonLayout->addWidget(selectAllButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(nextButton);
    
    mainLayout->addLayout(buttonLayout);

    connect(scanButton, &QPushButton::clicked, this, [this]() {
        // 실제 스캔 작업 시작
        for (int i = 0; i <= 100; i++) {
            QThread::msleep(50); // 실제 구현에서는 실제 스캔 작업으로 대체
            updateScanProgress(i);
            QApplication::processEvents();
        }
    });

    // 전체 선택 버튼 연결
    connect(selectAllButton, &QPushButton::clicked, this, [this]() {
        for (QCheckBox* checkbox : checkBoxes) {
            checkbox->setChecked(true);
        }
    });
}

void FirmwareUpdate::createTable()
{
    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(5);
    
    // 헤더 설정
    QStringList headers;
    headers << "Board Type" << "ID" << "FW ver" << "Progress" << "";
    tableWidget->setHorizontalHeaderLabels(headers);
    
    // 헤더 스타일 설정
    tableWidget->horizontalHeader()->setStyleSheet(
        "QHeaderView::section {"
        "background-color: #0078D7;"
        "color: white;"
        "padding: 4px;"
        "border: none;"
        "}"
    );
    
    // 체크박스를 포함한 샘플 데이터 추가
    tableWidget->setRowCount(3);
    
    // 샘플 데이터 추가
    for(int i = 0; i < 3; i++) {
        tableWidget->setItem(i, 0, new QTableWidgetItem("XM430-W210"));
        tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(i + 1)));
        tableWidget->setItem(i, 2, new QTableWidgetItem("V1.0"));
        tableWidget->setItem(i, 3, new QTableWidgetItem("0%"));
        
        // 체크박스 위젯 설정
        QWidget *checkBoxWidget = new QWidget();
        QHBoxLayout *layout = new QHBoxLayout(checkBoxWidget);
        QCheckBox *checkBox = new QCheckBox();
        checkBoxes.append(checkBox);
        layout->addWidget(checkBox);
        layout->setAlignment(Qt::AlignCenter);
        layout->setContentsMargins(0, 0, 0, 0);
        tableWidget->setCellWidget(i, 4, checkBoxWidget);
    }
    
    // 테이블 스타일 및 동작 설정
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->verticalHeader()->setVisible(false);
}

// 스캔 진행률을 업데이트하는 새로운 메서드
void FirmwareUpdate::updateScanProgress(int progress)
{
    scanProgress = progress;
    
    if (progress < 100) {
        // 스캔 진행 중일 때의 그라데이션 스타일
        progressFrame->setStyleSheet(QString(
            "QFrame {"
            "background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
            "stop:0 #0078D7, stop:%1 #0078D7,"
            "stop:%2 #B3D7FF, stop:1 #B3D7FF);"
            "border-radius: 10px;"
            "}"
        ).arg(progress/100.0).arg(progress/100.0));
        progressLabel->setText(QString("스캔 진행중... %1%").arg(progress));
    } else {
        // 스캔 완료 시 원래 스타일로 복원
        progressFrame->setStyleSheet(
            "QFrame {"
            "background-color: #0078D7;"
            "border-radius: 10px;"
            "}"
        );
        progressLabel->setText("펌웨어 업데이트");
    }
}