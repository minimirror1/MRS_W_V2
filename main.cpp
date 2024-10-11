#include "mainwindow.h"
#include <QApplication>

void forceLightMode() {
    QString lightStyleSheet = R"(
        QWidget {
            background-color: #ffffff; /* 흰색 배경 */
            color: #000000; /* 검정색 텍스트 */
        }
        QPushButton {
            background-color: #f0f0f0; /* 버튼의 밝은 배경 */
            color: #000000; /* 버튼 텍스트 색상 */
        }
    )";

    qApp->setStyleSheet(lightStyleSheet);
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // 라이트 모드 강제 적용
    forceLightMode();

    MainWindow w;
    w.show();
    return a.exec();
}
