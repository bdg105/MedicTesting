#ifndef WINSTARTTESTING_H
#define WINSTARTTESTING_H

#include <QMainWindow>

#include <QDebug>
#include <QFile>
#include <QMessageBox>

namespace Ui {
class WinStartTesting;
}

class WinStartTesting : public QMainWindow
{
    Q_OBJECT

public:
    explicit WinStartTesting(QWidget *parent = 0);
    ~WinStartTesting();
    void FuncInit(); // инициализация

private:
    Ui::WinStartTesting *ui;

public slots:
   // void loadTmSensTar(); // слот для работы с ТМ датчиками
private slots:
    void on_winStartBtnStart_clicked(); // создание окна WinQuestion
    void on_winStartBtnAbout_clicked(); // создание окна с сообщением Info
};

#endif // WINSTARTTESTING_H
