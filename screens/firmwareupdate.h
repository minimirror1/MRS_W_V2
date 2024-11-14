#ifndef FIRMWAREUPDATE_H
#define FIRMWAREUPDATE_H

#include <QDialog>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QCheckBox>
#include <QFrame>
#include <QLabel>
#include <QThread>
#include <QApplication>

class FirmwareUpdate : public QDialog
{
    Q_OBJECT

public:
    explicit FirmwareUpdate(QWidget *parent = nullptr);
    ~FirmwareUpdate();
    void updateScanProgress(int progress);

private:
    void setupUI();
    void createTable();
    
    QTableWidget *tableWidget;
    QPushButton *selectAllButton;
    QPushButton *nextButton;
    QPushButton *scanButton;
    
    QList<QCheckBox*> checkBoxes;
    
    QFrame *progressFrame;
    QLabel *progressLabel;
    int scanProgress;

};

#endif // FIRMWAREUPDATE_H