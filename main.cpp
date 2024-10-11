#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>
#include <QPalette>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Fusion 스타일 사용
    //a.setStyle(QStyleFactory::create("Fusion"));

    // 밝은 팔레트를 사용하여 라이트 모드 보장
    QPalette lightPalette;
    lightPalette.setColor(QPalette::Window, QColor(255, 255, 255)); // 배경 흰색
    lightPalette.setColor(QPalette::WindowText, QColor(0, 0, 0));   // 텍스트 검정색
    lightPalette.setColor(QPalette::Base, QColor(240, 240, 240));   // 입력 위젯 배경색
    lightPalette.setColor(QPalette::Text, QColor(0, 0, 0));         // 입력 텍스트 색상
    lightPalette.setColor(QPalette::Button, QColor(240, 240, 240)); // 버튼 배경색
    lightPalette.setColor(QPalette::ButtonText, QColor(0, 0, 0));   // 버튼 텍스트 색상

    // 비활성화 상태에 대한 색상 설정 추가
    lightPalette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(128, 128, 128)); // 비활성화된 텍스트 색상
    lightPalette.setColor(QPalette::Disabled, QPalette::Text, QColor(128, 128, 128));       // 입력 위젯의 비활성화된 텍스트 색상
    lightPalette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(128, 128, 128)); // 버튼의 비활성화된 텍스트 색상

    a.setPalette(lightPalette);

    MainWindow w;
    w.show();
    return a.exec();
}
