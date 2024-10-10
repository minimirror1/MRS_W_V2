#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openAddDeviceDialog(); // "장치 추가" 다이얼로그를 여는 함수 선언
    void toggleMainMenuVisibility(bool visible); // 메인 메뉴 표시 토글 함수 선언

private:
    Ui::MainWindow *ui;

    void createCentralLayout();
    void createStatusBar();
    void createMenus(); // 메뉴 생성을 위한 함수 선언
    void createFixedToolWidget(); // 고정된 도구 버튼 위젯 생성 함수 선언

    QWidget *mainToolWidget; // 메인 메뉴 도구 버튼들이 있는 고정된 위젯
};

#endif // MAINWINDOW_H
