#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QStackedWidget>
#include <QGroupBox>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <QSlider>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QSerialPortInfo>
#include <QStandardItem>
#include <QRegularExpression>
#include <QSerialPort>

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

private:
    QVBoxLayout *mainLayout;
    QListWidget *menuList;
    QStackedWidget *contentStack;
    
    // 검색 페이지 위젯들
    QWidget *searchPage;
    QVBoxLayout *searchLayout;
    QGroupBox *protocolGroup;
    QGroupBox *portGroup;
    QGroupBox *baudRateGroup;
    QGroupBox *idRangeGroup;
    
    QCheckBox *mrsBox;
    
    QComboBox *portCombo;
    QComboBox *baudRateCombo;
    
    QSpinBox *startIdSpin;
    QSpinBox *endIdSpin;
    QSlider *startIdSlider;
    QSlider *endIdSlider;
    
    QPushButton *connectBtn;
    QPushButton *confirmBtn;
    QPushButton *cancelBtn;

    QSerialPort *serialPort;

    void setupUI();
    void createSearchPage();
    void refreshPortList();

private slots:
    void handleConnect();
};

#endif // SETTINGS_H