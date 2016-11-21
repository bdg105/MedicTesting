#ifndef WINQUESTION_H
#define WINQUESTION_H

#include <QDialog>

#include <QMultiMap>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>

namespace Ui {
class WinQuestion;
}

class WinQuestion : public QDialog
{
    Q_OBJECT

public:
    explicit WinQuestion(QWidget *parent = 0, const QString &strSelectPart = "");
    ~WinQuestion();
    void FuncInit(const QString &strSelectPart = ""); // парсинг файла с вопросами, заполнение вектора со строками и словаря с правильными ответами
    void Results(); //вывод информации с результатами

public slots:
    void ControlElementsAndWorkQuest(); // создание элементов (вопросы, варианты ответов и т.д.) и работа с ними
    void clickedOnWinQuestBtnEnd();     // нажатие на кнопку Закончить


private:
    Ui::WinQuestion *ui;
    QVector<QString> stringVector; // вектор заполняется строками из файла с вопросами
    quint32 numberPosInVector; // для отслеживания розиции в векторе
    QVector<QString> resultQuest; // для хранения результатов
    QMultiMap<quint32, QString> correctResponse; // мультисловарь для хранения правильных ответов
    QMultiMap<quint32, QString> inputResponse; // мультисловарь для хранения введённых ответов
    quint32 numberQuesting; // номера вопросов, нужно для QMultiMap, так же используется для отслеживания кол-ва вопросов
    quint32 amountCorrectResponce; // кол-во правильных ответов
};

#endif // WINQUESTION_H
