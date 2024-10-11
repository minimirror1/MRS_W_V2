#include "mainwindow.h"
#include <QApplication>
#include <QPalette>
#include <QColor>

void forceLightMode() {
    QPalette lightPalette;
    lightPalette.setColor(QPalette::Window, QColor(255, 255, 255)); // 흰색 배경
    lightPalette.setColor(QPalette::WindowText, QColor(0, 0, 0));   // 검정색 텍스트
    lightPalette.setColor(QPalette::Base, QColor(240, 240, 240));   // 입력 위젯의 배경색
    lightPalette.setColor(QPalette::Text, QColor(0, 0, 0));         // 입력 텍스트 색상
    lightPalette.setColor(QPalette::Button, QColor(240, 240, 240)); // 버튼 배경색
    lightPalette.setColor(QPalette::ButtonText, QColor(0, 0, 0));   // 버튼 텍스트 색상

    QApplication::setPalette(lightPalette);
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // 라이트 모드 강제 적용
    forceLightMode();

    MainWindow w;
    w.show();
    return a.exec();
}
