#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QIcon>
#include <QPalette>
#include <QSplitter>
#include <QTextEdit>
#include <QListWidget>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 윈도우 이름 설정
    setWindowTitle("MRS 위자드 1.0 - v0.0.0.1");
    // 프로그램 아이콘 설정
    setWindowIcon(QIcon(":/icons/appicon.png"));

    // 메뉴 생성
    createMenus();

    // 스플리터 추가를 통해 영역 분할 및 크기 조절 가능하게 설정
    createCentralLayout();

    //상태표시줄 생성
    createStatusBar();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 중앙 레이아웃 생성 함수 추가
void MainWindow::createCentralLayout() {
    // 고정된 도구 버튼을 상단에 고정시킨 위젯 추가
    createFixedToolWidget(); // 도구 버튼 위젯 생성

    // 1번 구역 - 예를 들어 장치 목록을 보여주는 ListWidget
    QListWidget *listWidget = new QListWidget();
    listWidget->addItem("장치 1");
    listWidget->addItem("장치 2");
    listWidget->addItem("장치 3");

    // 2번 구역 - 예를 들어 정보를 표시하는 TextEdit
    QTextEdit *infoTextEdit = new QTextEdit();
    infoTextEdit->setText("이곳에 선택한 장치의 정보가 표시됩니다.");

    // 스플리터 생성 및 1, 2번 구역 추가
    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    splitter->addWidget(listWidget);
    splitter->addWidget(infoTextEdit);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 3);

    // 스플리터 핸들 스타일 설정
    splitter->setHandleWidth(5); // 핸들의 너비를 설정 (기본보다 넓게 설정하여 드래그 표시 강조)
    splitter->setStyleSheet("QSplitter::handle { background: lightgray; }"); // 핸들 스타일 설정

    // 중앙 레이아웃 설정
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *centralLayout = new QVBoxLayout(centralWidget);
    centralLayout->setContentsMargins(0, 0, 0, 0);
    centralLayout->setSpacing(0); // 상단에 고정시키기 위해 간격을 0으로 설정
    centralLayout->addWidget(mainToolWidget); // 고정된 도구 버튼 추가
    centralLayout->addWidget(splitter);       // 스플리터 추가

    setCentralWidget(centralWidget);
}

// 상태 표시줄 생성 함수 추가
void MainWindow::createStatusBar() {
    // 상태 표시줄 생성 및 설정
    QStatusBar *statusBar = new QStatusBar(this);
    statusBar->setStyleSheet("QStatusBar { background-color: white; }"); // 상태 표시줄의 배경색을 흰색으로 설정
    setStatusBar(statusBar);

    // 송신(T) 및 수신(R) 라벨을 하나의 레이아웃에 추가
    QWidget *statusContainer = new QWidget(this);
    QHBoxLayout *statusLayout = new QHBoxLayout(statusContainer);
    statusLayout->setContentsMargins(0, 0, 0, 0); // 여백 제거
    statusLayout->setSpacing(5); // T와 R 사이의 간격 설정

    // 송신(T) 라벨 생성
    QLabel *tLabel = new QLabel("T", this);
    tLabel->setAlignment(Qt::AlignCenter);
    tLabel->setFixedSize(20, 20);
    tLabel->setStyleSheet("background-color: gray; border: 1px solid lightgray; color: white; border-radius: 5px;");
    statusLayout->addWidget(tLabel);

    // 수신(R) 라벨 생성
    QLabel *rLabel = new QLabel("R", this);
    rLabel->setAlignment(Qt::AlignCenter);
    rLabel->setFixedSize(20, 20);
    rLabel->setStyleSheet("background-color: gray; border: 1px solid lightgray; color: white; border-radius: 5px;");
    statusLayout->addWidget(rLabel);

    // 상태 표시줄에 라벨을 포함하는 위젯 추가
    statusBar->addWidget(statusContainer);

    // 송신 신호를 받을 때의 동작
    auto sendSignal = [tLabel]() {
        tLabel->setStyleSheet("background-color: red; border: 1px solid lightgray; color: white; border-radius: 5px;");
        QTimer::singleShot(32, [tLabel]() {
            tLabel->setStyleSheet("background-color: gray; border: 1px solid lightgray; color: white; border-radius: 5px;");
        });
    };

    // 수신 신호를 받을 때의 동작
    auto receiveSignal = [rLabel]() {
        rLabel->setStyleSheet("background-color: green; border: 1px solid lightgray; color: white; border-radius: 5px;");
        QTimer::singleShot(32, [rLabel]() {
            rLabel->setStyleSheet("background-color: gray; border: 1px solid lightgray; color: white; border-radius: 5px;");
        });
    };

    // 예시: 임의로 신호를 발생시켜 보기 위해 버튼을 추가 (실제 송수신 이벤트와 연결 필요)
    QPushButton *testSendButton = new QPushButton("Send Test", this);
    connect(testSendButton, &QPushButton::clicked, sendSignal);
    statusBar->addPermanentWidget(testSendButton);

    QPushButton *testReceiveButton = new QPushButton("Receive Test", this);
    connect(testReceiveButton, &QPushButton::clicked, receiveSignal);
    statusBar->addPermanentWidget(testReceiveButton);
}





// 메뉴 생성 함수의 구현
void MainWindow::createMenus() {
    // 메뉴 바 생성
    QMenuBar *menuBar = new QMenuBar(this);

    // "장치" 메뉴 생성
    QMenu *deviceMenu = new QMenu("장치", this);
    deviceMenu->setStyleSheet(
        "QMenu { background-color: white; border: 1px solid gray; }"
        "QMenu::item:selected { background-color: #0078D7; border: 1px solid #41ADFF; color: white; }"
        );

    QAction *searchAction = new QAction(QIcon(":/icons/search.png"), "검색", this);
    searchAction->setShortcut(Qt::Key_F2);
    deviceMenu->addAction(searchAction);

    QAction *connectAction = new QAction(QIcon(":/icons/connect.png"), "연결하기", this);
    connectAction->setShortcut(Qt::Key_F3);
    deviceMenu->addAction(connectAction);

    QAction *factoryResetAction = new QAction(QIcon(":/icons/factory_reset.png"), "팩토리 리셋", this);
    deviceMenu->addAction(factoryResetAction);

    QAction *rebootAction = new QAction(QIcon(":/icons/reboot.png"), "리부팅", this);
    deviceMenu->addAction(rebootAction);

    deviceMenu->addSeparator();

    QAction *exitAction = new QAction("종료", this);
    exitAction->setShortcut(QKeySequence::Quit);
    deviceMenu->addAction(exitAction);

    menuBar->addMenu(deviceMenu);

    // "보기" 메뉴 생성
    QMenu *viewMenu = new QMenu("보기", this);
    viewMenu->setStyleSheet(
        "QMenu { background-color: white; border: 1px solid gray; }"
        "QMenu::item:selected { background-color: #0078D7; border: 1px solid #41ADFF; color: white; }"
        );

    QAction *graphAction = new QAction(QIcon(":/icons/graph.png"), "그래프", this);
    graphAction->setShortcut(Qt::Key_F5);
    viewMenu->addAction(graphAction);

    QAction *packetAction = new QAction(QIcon(":/icons/packet.png"), "패킷", this);
    packetAction->setShortcut(Qt::Key_F6);
    viewMenu->addAction(packetAction);

    viewMenu->addSeparator();

    QAction *mainMenuAction = new QAction("도구 모음", this);
    mainMenuAction->setCheckable(true);
    mainMenuAction->setChecked(true);
    viewMenu->addAction(mainMenuAction);

    connect(mainMenuAction, &QAction::toggled, this, &MainWindow::toggleMainMenuVisibility);

    menuBar->addMenu(viewMenu);

    setMenuBar(menuBar);


    // "도구" 메뉴 생성
    QMenu *toolsMenu = new QMenu("도구", this);
    toolsMenu->setStyleSheet(
        "QMenu { background-color: white; border: 1px solid gray; }"
        "QMenu::item:selected { background-color: #0078D7; border: 1px solid #41ADFF; color: white; }"
        );

    // QAction *restoreFirmwareAction = new QAction(QIcon(":/icons/restore_firmware.png"), "펌웨어 복구", this);
    // restoreFirmwareAction->setShortcut(Qt::Key_F7);
    // toolsMenu->addAction(restoreFirmwareAction);

    QAction *updateFirmwareAction = new QAction(QIcon(":/icons/update_firmware.png"), "펌웨어 업데이트", this);
    updateFirmwareAction->setShortcut(Qt::Key_F8);
    toolsMenu->addAction(updateFirmwareAction);

    QAction *updateAllFirmwareAction = new QAction(QIcon(":/icons/update_all.png"), "펌웨어 전체 업데이트", this);
    toolsMenu->addAction(updateAllFirmwareAction);

    toolsMenu->addSeparator();

    QAction *calibrationAction = new QAction("캘리브레이션", this);
    calibrationAction->setDisabled(true); // 비활성화 상태로 설정
    toolsMenu->addAction(calibrationAction);

    QAction *selfDiagnosisAction = new QAction("자가진단", this);
    selfDiagnosisAction->setDisabled(true); // 비활성화 상태로 설정
    toolsMenu->addAction(selfDiagnosisAction);

    QAction *idDiagnosisAction = new QAction("ID 진단", this);
    idDiagnosisAction->setDisabled(true); // 비활성화 상태로 설정
    toolsMenu->addAction(idDiagnosisAction);

    // "컨트롤 테이블" 서브 메뉴 추가
    // QMenu *controlTableMenu = new QMenu("컨트롤 테이블", this);
    // toolsMenu->addMenu(controlTableMenu);

    toolsMenu->addSeparator();

    QAction *settingsAction = new QAction(QIcon(":/icons/settings.png"), "설정", this);
    settingsAction->setShortcut(Qt::Key_F4);
    toolsMenu->addAction(settingsAction);

    menuBar->addMenu(toolsMenu);

    setMenuBar(menuBar);

    // 신호 연결 예시
    connect(searchAction, &QAction::triggered, this, &MainWindow::openAddDeviceDialog);
    connect(exitAction, &QAction::triggered, this, &MainWindow::close);
}

// 고정된 도구 버튼 위젯 생성 함수의 구현
void MainWindow::createFixedToolWidget() {
    // 메인 메뉴 고정 위젯 생성
    mainToolWidget = new QWidget(this);

    // 수평 레이아웃 생성 및 간격 설정
    QHBoxLayout *toolLayout = new QHBoxLayout(mainToolWidget);
    toolLayout->setContentsMargins(0, 0, 0, 0); // 위, 좌, 우 여백 0
    toolLayout->setSpacing(10); // 버튼 간 간격 10px 설정
    toolLayout->setAlignment(Qt::AlignLeft); // 좌측 정렬

    // 버튼 스타일 설정 (테두리 제거, 마우스 올렸을 때 배경색 변경 및 둥근 모서리 설정)
    QString buttonStyle = R"(
        QPushButton {
            border: none;
            background: transparent;
        }
        QPushButton:hover {
            background-color: #0078D7;
            border-radius: 10px; /* 둥근 모서리 설정 */
        }
    )";

    QSize buttonSize(30, 30); // 버튼 크기 설정 (30x30)
    QSize iconSize(20, 20);   // 아이콘 크기 설정 (20x20)

    // 도구 버튼 추가 및 크기 설정
    QPushButton *searchButton = new QPushButton(QIcon(":/icons/search.png"), "", this);
    searchButton->setStyleSheet(buttonStyle);
    searchButton->setFixedSize(buttonSize);
    searchButton->setIconSize(iconSize); // 아이콘 크기를 20x20으로 설정
    searchButton->setToolTip("다이나믹셀 검색");
    toolLayout->addWidget(searchButton);

    QPushButton *connectButton = new QPushButton(QIcon(":/icons/connect.png"), "", this);
    connectButton->setStyleSheet(buttonStyle);
    connectButton->setFixedSize(buttonSize);
    connectButton->setIconSize(iconSize);
    connectButton->setToolTip("연결하기");
    toolLayout->addWidget(connectButton);

    QPushButton *graphButton = new QPushButton(QIcon(":/icons/graph.png"), "", this);
    graphButton->setStyleSheet(buttonStyle);
    graphButton->setFixedSize(buttonSize);
    graphButton->setIconSize(iconSize);
    graphButton->setToolTip("그래프");
    toolLayout->addWidget(graphButton);

    QPushButton *packetButton = new QPushButton(QIcon(":/icons/packet.png"), "", this);
    packetButton->setStyleSheet(buttonStyle);
    packetButton->setFixedSize(buttonSize);
    packetButton->setIconSize(iconSize);
    packetButton->setToolTip("패킷 보기");
    toolLayout->addWidget(packetButton);

    QPushButton *firmwareRestoreButton = new QPushButton(QIcon(":/icons/restore_firmware.png"), "", this);
    firmwareRestoreButton->setStyleSheet(buttonStyle);
    firmwareRestoreButton->setFixedSize(buttonSize);
    firmwareRestoreButton->setIconSize(iconSize);
    firmwareRestoreButton->setToolTip("펌웨어 복구");
    toolLayout->addWidget(firmwareRestoreButton);

    QPushButton *notificationButton = new QPushButton(QIcon(":/icons/notification.png"), "", this);
    notificationButton->setStyleSheet(buttonStyle);
    notificationButton->setFixedSize(buttonSize);
    notificationButton->setIconSize(iconSize);
    notificationButton->setToolTip("알림");
    toolLayout->addWidget(notificationButton);

    // 메인 메뉴 위젯의 배경을 흰색으로 설정
    mainToolWidget->setStyleSheet("background-color: white;");
    mainToolWidget->setFixedHeight(30); // 높이를 버튼 높이와 일치하게 설정
}





// "장치 추가" 다이얼로그를 여는 함수의 구현
void MainWindow::openAddDeviceDialog() {
    QDialog addDeviceDialog(this);
    addDeviceDialog.setWindowTitle("장치 추가");

    QVBoxLayout *layout = new QVBoxLayout;
    QLabel *label = new QLabel("새로운 장치를 추가하는 창입니다.");
    QPushButton *okButton = new QPushButton("확인");

    layout->addWidget(label);
    layout->addWidget(okButton);
    addDeviceDialog.setLayout(layout);

    connect(okButton, &QPushButton::clicked, &addDeviceDialog, &QDialog::accept);

    addDeviceDialog.exec();
}

// 메인 메뉴 가시성을 토글하는 함수의 구현
void MainWindow::toggleMainMenuVisibility(bool visible) {
    if (mainToolWidget) {
        mainToolWidget->setVisible(visible);
    }
}
