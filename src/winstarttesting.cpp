#include "winstarttesting.h"
#include "ui_winstarttesting.h"
#include "winquestion.h"

WinStartTesting::WinStartTesting(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WinStartTesting)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint); // установка флагов, будет доступны кнопки свернуть и закрыть
    setFixedSize(680,470); // фиксированный размер окна программы (при наведении не будет стрелок для увелечения размеров окна программы)

    FuncInit(); //инициализация
}

WinStartTesting::~WinStartTesting()
{
    delete ui;
}

void WinStartTesting::FuncInit() { //инициализация

    /* START Заполнение ComboBox "Выберите часть" */
    ui->winStartComboBoxSelectPart->clear();
    ui->winStartComboBoxSelectPart->setEditable(false);
    ui->winStartComboBoxSelectPart->addItem(QObject::trUtf8("Часть 1"));
    ui->winStartComboBoxSelectPart->addItem(QObject::trUtf8("Часть 2"));
    ui->winStartComboBoxSelectPart->addItem(QObject::trUtf8("Часть 3"));
    ui->winStartComboBoxSelectPart->addItem(QObject::trUtf8("Часть 4"));
    ui->winStartComboBoxSelectPart->addItem(QObject::trUtf8("Часть 5"));
    /* END Заполнение ComboBox "Выберите часть" */
}

void WinStartTesting::on_winStartBtnStart_clicked() { //создание окна WinQuestion

    QString selectPart; //для формирования строки с именем файла с вопросами
    selectPart.setNum(ui->winStartComboBoxSelectPart->currentIndex()+1);
#ifdef Q_OS_WIN32
    selectPart.prepend("quest_");
#elif Q_OS_UNIX
    selectPart.prepend("/Volumes/MUSIC/MedicTesting/bin/quest_");
#endif
    selectPart.append(".txt");

    QFile file(selectPart);
    if (file.exists()) { //существует ли файл - файл есть
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(0, QObject::trUtf8("Внимание!"),
                                  QObject::trUtf8("Файл с вопросами закрыт для чтения"),QMessageBox::Ok);
        }
        else {
            file.close();
            WinQuestion *winQuestion = new WinQuestion(this, selectPart); //создание окна с вопросами
            winQuestion->setWindowFlags(Qt::Sheet | Qt::WindowCloseButtonHint);  //задание свойств окна, можно только закрыть
            winQuestion->exec(); // с методом exec диалоговое окно становится модальным
        }
    }
    else {
        QMessageBox::critical(0, QObject::trUtf8("Внимание!"),
                              QObject::trUtf8("Файл с вопросами не найден!"),QMessageBox::Ok);
    }


}

void WinStartTesting::on_winStartBtnAbout_clicked() // создание окна с сообщением Info
{
    QMessageBox::information(this, QObject::trUtf8("Info"),
                          QObject::trUtf8("<h3>MedicTesting</h3>"
                                          "<p>Version: 1.0</p>"
                                          "<p>Developer: Basov Denis G.</p>"
                                          "<p>E-mail: bdg1105@gmail.com</p>"),
                             QMessageBox::Ok);
}
