#include "winquestion.h"
#include "ui_winquestion.h"

WinQuestion::WinQuestion(QWidget *parent, const QString &strSelectPart) :
    QDialog(parent),
    ui(new Ui::WinQuestion)
{
    ui->setupUi(this);

    FuncInit(strSelectPart);

    this->setAttribute(Qt::WA_DeleteOnClose); // установка атрибута, по слоту close удалится диалоговое окно и все объекты (отработает деструктор)

    connect(ui->winQuestBtnNext, SIGNAL(clicked(bool)), this, SLOT(ControlElementsAndWorkQuest())); // работа с вопросами по кнопке Далее
    connect(ui->winQuestBtnEnd,  SIGNAL(clicked(bool)), this, SLOT(clickedOnWinQuestBtnEnd())); // вывод рез-тов по кнопке Закончить

    connect(ui->winQuestBtnExit, SIGNAL(clicked(bool)), this, SLOT(close())); // закрыть окно по кнопке Выход

}

WinQuestion::~WinQuestion()
{
    delete ui;
}

void WinQuestion::FuncInit(const QString &strSelectPart) { // парсинг файла с вопросами, заполнение вектора со строками и словаря с ответами

    /* START обнуление переменных */
    numberPosInVector = 0;
    numberQuesting = 0;
    amountCorrectResponce = 0;
    /* END обнуление переменных */

    quint32 numberStringFile = 1; // число строк в файле, для выделения строки с вопросом и строк с ответами
    quint32 numberQuest = 1; // число вопросов, используется для ведения индекса в Словаре

    /* START получение данных из файла */
    // данные берутся из файла построчно, и записываются в элементы вектора, далее работа ведётся с вектором
    QFile file(strSelectPart);
    if (file.isOpen()) { // если файл открыт, закрываю
        file.close();
    }
    file.open(QIODevice::ReadOnly); // файл открыт для чтения
    QTextStream stream(&file); // для хождения по файлу, привязка к объекту класса QTextStream
    QString strOfFile; // считываются строки из файла
    strOfFile = stream.readLine();          // читается строка из файла
    while (!strOfFile.isNull()) {           // если строки нет, не путать с пустой строкой!
        numberStringFile++;                 // увеличивается число строки
        stringVector.push_back(strOfFile);  // в конец вектора добавляется элемент (новая строка)
        strOfFile = stream.readLine();      // читается строка из файла
        switch (numberStringFile) {         // если строка 3 - 6, то это ответ, нужно анализировать ответы и заносить их в Словарь
        case 3: case 4: case 5: case 6:
        {
            QString strForFindPlus;                 // строка для поиска +
            strForFindPlus = strOfFile.mid(0,1);    // выделяется первый символ
            if (strForFindPlus.contains("+", Qt::CaseInsensitive)) { // это +
                correctResponse.insert(numberQuest,"+"); // да - заносим +
            }
            else {
                correctResponse.insert(numberQuest,"-"); // нет - заносим +
            }
            if(numberStringFile == 6) { // обработано уже 6 строк?
                numberStringFile = 0;   // обнуляется счётчик
                numberQuest++;          // для словаря фиксируется следующий вопрос
            }
            break;
        }
        default:
            break;
        }
    }

    file.close(); // закрывается файл

    /* END получение данных из файла */

    // для отладки
    /*
    for(QString str : stringVector)
        qDebug() << str;

    for(int j = 1; j <= numberQuest; j++) {
        QList<QString> values = correctResponse.values(j);
        for (int i = 0; i < values.size(); ++i)
            qDebug() << j << values.at(i);
    }
    */

    /* START скрыть с экрана элементы */
    ui->winQuestLabelOptionalInfo->setVisible(false);
    ui->winQuestGroupBoxMulti->setVisible(false);
    ui->winQuestGroupBoxSingle->setVisible(false);
    ui->winQuestCheckBoxQuest1->setVisible(false);
    ui->winQuestCheckBoxQuest2->setVisible(false);
    ui->winQuestCheckBoxQuest3->setVisible(false);
    ui->winQuestCheckBoxQuest4->setVisible(false);
    ui->winQuestRadioButtonQuest1->setVisible(false);
    ui->winQuestRadioButtonQuest2->setVisible(false);
    ui->winQuestRadioButtonQuest3->setVisible(false);
    ui->winQuestRadioButtonQuest4->setVisible(false);
    ui->winQuestLabelOkError->setVisible(false);
    ui->winQuestLabelOkErrorLable->setVisible(false);
    /* END скрыть с экрана элементы */

    ui->winQuestBtnEnd->setEnabled(false); // блокировка кнопки Закончить, ещё не начата работа с вопросами

}

void WinQuestion::ControlElementsAndWorkQuest() {

    bool nalError = false; // для фиксирования ошибок

    /* START отобразить на экране элементы */
    if (!numberPosInVector) { // после инициализации элементы скрыты, отобразить нужно только один раз, потом не скрываются
        ui->winQuestLabelOptionalInfo->setVisible(true);
        ui->winQuestGroupBoxMulti->setVisible(true);
        ui->winQuestGroupBoxSingle->setVisible(true);
        ui->winQuestCheckBoxQuest1->setVisible(true);
        ui->winQuestCheckBoxQuest2->setVisible(true);
        ui->winQuestCheckBoxQuest3->setVisible(true);
        ui->winQuestCheckBoxQuest4->setVisible(true);
        ui->winQuestRadioButtonQuest1->setVisible(true);
        ui->winQuestRadioButtonQuest2->setVisible(true);
        ui->winQuestRadioButtonQuest3->setVisible(true);
        ui->winQuestRadioButtonQuest4->setVisible(true);
        ui->winQuestLabelOkError->setVisible(true);
        ui->winQuestLabelOkErrorLable->setVisible(true);
    }
    /* END отобразить на экране элементы */

    ui->winQuestBtnEnd->setEnabled(true); // разблокировка кнопки Закончить


    /* START парсинг результатов, ответы заносятся в словарь */
    if (numberPosInVector) { //после первого нажатия на кнопку Далее тест запустится и сохранять ещё не чего
        if (ui->winQuestGroupBoxMulti->isVisible()) {
            if (ui->winQuestCheckBoxQuest1->isChecked()) {
                inputResponse.insert(numberQuesting,"+");
            }
            else {
                inputResponse.insert(numberQuesting,"-");
            }
            if (ui->winQuestCheckBoxQuest2->isChecked()) {
                inputResponse.insert(numberQuesting,"+");
            }
            else {
                inputResponse.insert(numberQuesting,"-");
            }
            if (ui->winQuestCheckBoxQuest3->isChecked()) {
                inputResponse.insert(numberQuesting,"+");
            }
            else {
                inputResponse.insert(numberQuesting,"-");
            }
            if (ui->winQuestCheckBoxQuest4->isChecked()) {
                inputResponse.insert(numberQuesting,"+");
            }
            else {
                inputResponse.insert(numberQuesting,"-");
            }
        }

        if (ui->winQuestGroupBoxSingle->isVisible()) {
            if (ui->winQuestRadioButtonQuest1->isChecked()) {
                inputResponse.insert(numberQuesting,"+");
            }
            else {
                inputResponse.insert(numberQuesting,"-");
            }
            if (ui->winQuestRadioButtonQuest2->isChecked()) {
                inputResponse.insert(numberQuesting,"+");
            }
            else {
                inputResponse.insert(numberQuesting,"-");
            }
            if (ui->winQuestRadioButtonQuest3->isChecked()) {
                inputResponse.insert(numberQuesting,"+");
            }
            else {
                inputResponse.insert(numberQuesting,"-");
            }
            if (ui->winQuestRadioButtonQuest4->isChecked()) {
                inputResponse.insert(numberQuesting,"+");
            }
            else {
                inputResponse.insert(numberQuesting,"-");
            }
        }
    }
    /* END парсинг результатов, ответы заносятся в словарь */

    if (numberPosInVector <= quint32(stringVector.size())) { // если номер позиции в векторе меньше чем кол-во элементов вектора, не выйти за пределы
        //заполнение окна данными

        /* START парсинг вопросов, вывод на окно элементов */
        if (numberPosInVector < quint32(stringVector.size())) { // дурацкое условие, пока не придумал как изменить на что то более "изящное",
            // оно нужно, что бы не было переполнения в векторе, поиск элементов закончен,
            // а обработать ответы последний раз ещё нужно
            ui->winQuestLabelTitleQuest->setText(stringVector[numberPosInVector+1]);
            ui->winQuestLabelTitleQuest->setStyleSheet("font-size:14pt");
            if (stringVector[numberPosInVector] == "1~") {
                ui->winQuestLabelOptionalInfo->setText("(один вариант ответа)");
                ui->winQuestLabelOptionalInfo->setStyleSheet("font-size: 9pt");
                ui->winQuestGroupBoxMulti->setVisible(false);
                ui->winQuestGroupBoxSingle->setVisible(true);

                if (stringVector[numberPosInVector+2].contains("+", Qt::CaseInsensitive)) {
                    QString changeStr;
                    changeStr = stringVector[numberPosInVector+2].mid(2, stringVector[numberPosInVector+2].size());
                    ui->winQuestRadioButtonQuest1->setText(changeStr);
                }
                else {
                    ui->winQuestRadioButtonQuest1->setText(stringVector[numberPosInVector+2]);
                }
                ui->winQuestRadioButtonQuest1->setStyleSheet("font-size: 11pt");

                if (stringVector[numberPosInVector+3].contains("+", Qt::CaseInsensitive)) {
                    QString changeStr;
                    changeStr = stringVector[numberPosInVector+3].mid(2, stringVector[numberPosInVector+3].size());
                    ui->winQuestRadioButtonQuest2->setText(changeStr);
                }
                else {
                    ui->winQuestRadioButtonQuest2->setText(stringVector[numberPosInVector+3]);
                }
                ui->winQuestRadioButtonQuest2->setStyleSheet("font-size: 11pt");

                if (stringVector[numberPosInVector+4].contains("+", Qt::CaseInsensitive)) {
                    QString changeStr;
                    changeStr = stringVector[numberPosInVector+4].mid(2, stringVector[numberPosInVector+4].size());
                    ui->winQuestRadioButtonQuest3->setText(changeStr);
                }
                else {
                    ui->winQuestRadioButtonQuest3->setText(stringVector[numberPosInVector+4]);
                }
                ui->winQuestRadioButtonQuest3->setStyleSheet("font-size: 11pt");

                if (stringVector[numberPosInVector+5].contains("+", Qt::CaseInsensitive)) {
                    QString changeStr;
                    changeStr = stringVector[numberPosInVector+5].mid(2, stringVector[numberPosInVector+5].size());
                    ui->winQuestRadioButtonQuest4->setText(changeStr);
                }
                else {
                    ui->winQuestRadioButtonQuest4->setText(stringVector[numberPosInVector+5]);
                }
                ui->winQuestRadioButtonQuest4->setStyleSheet("font-size: 11pt");
            }
            else {
                ui->winQuestLabelOptionalInfo->setText("(несколько вариантов ответа)");
                ui->winQuestLabelOptionalInfo->setStyleSheet("font-size: 9pt");
                ui->winQuestGroupBoxMulti->setVisible(true);
                ui->winQuestGroupBoxSingle->setVisible(false);

                if (stringVector[numberPosInVector+2].contains("+", Qt::CaseInsensitive)) {
                    QString changeStr;
                    changeStr = stringVector[numberPosInVector+2].mid(2, stringVector[numberPosInVector+2].size());
                    ui->winQuestCheckBoxQuest1->setText(changeStr);
                }
                else {
                    ui->winQuestCheckBoxQuest1->setText(stringVector[numberPosInVector+2]);
                }
                ui->winQuestCheckBoxQuest1->setStyleSheet("font-size: 11pt");

                if (stringVector[numberPosInVector+3].contains("+", Qt::CaseInsensitive)) {
                    QString changeStr;
                    changeStr = stringVector[numberPosInVector+3].mid(2, stringVector[numberPosInVector+3].size());
                    ui->winQuestCheckBoxQuest2->setText(changeStr);
                }
                else {
                    ui->winQuestCheckBoxQuest2->setText(stringVector[numberPosInVector+3]);
                }
                ui->winQuestCheckBoxQuest2->setStyleSheet("font-size: 11pt");

                if (stringVector[numberPosInVector+4].contains("+", Qt::CaseInsensitive)) {
                    QString changeStr;
                    changeStr = stringVector[numberPosInVector+4].mid(2, stringVector[numberPosInVector+4].size());
                    ui->winQuestCheckBoxQuest3->setText(changeStr);
                }
                else {
                    ui->winQuestCheckBoxQuest3->setText(stringVector[numberPosInVector+4]);
                }
                ui->winQuestCheckBoxQuest3->setStyleSheet("font-size: 11pt");

                if (stringVector[numberPosInVector+5].contains("+", Qt::CaseInsensitive)) {
                    QString changeStr;
                    changeStr = stringVector[numberPosInVector+5].mid(2, stringVector[numberPosInVector+5].size());
                    ui->winQuestCheckBoxQuest4->setText(changeStr);
                }
                else {
                    ui->winQuestCheckBoxQuest4->setText(stringVector[numberPosInVector+5]);
                }
                ui->winQuestCheckBoxQuest4->setStyleSheet("font-size: 11pt");

            }
        }
        /* END парсинг вопросов, вывод на окно элементов */

        if(numberPosInVector) { // проверка соответсвия ответов и результатов
            QList<QString> values = inputResponse.values(numberQuesting);
            QList<QString> values2 = correctResponse.values(numberQuesting);
            for (int i = 0; i < values.size(); ++i) {
                if (values.at(i) != values2.at(i))  {
                    nalError = true; // фиксирование ошибки
                }
            }
        }

        if (numberPosInVector) { // если вопросов ещё не было
            if (nalError) { // если была ошибка
                ui->winQuestLabelOkError->setStyleSheet("background-color: red");
            }
            else {
                ui->winQuestLabelOkError->setStyleSheet("background-color: green");
                amountCorrectResponce++;
            }
        }

        numberPosInVector+=6; // смещение в векторе с вопросами на 6 строк, как раз размер одного вопроса

        /* START сбросить выделенные элементы */
        ui->winQuestRadioButtonQuest1->setAutoExclusive(false);
        ui->winQuestRadioButtonQuest1->setChecked(false);
        ui->winQuestRadioButtonQuest1->setAutoExclusive(true);
        ui->winQuestRadioButtonQuest2->setAutoExclusive(false);
        ui->winQuestRadioButtonQuest2->setChecked(false);
        ui->winQuestRadioButtonQuest2->setAutoExclusive(true);
        ui->winQuestRadioButtonQuest3->setAutoExclusive(false);
        ui->winQuestRadioButtonQuest3->setChecked(false);
        ui->winQuestRadioButtonQuest3->setAutoExclusive(true);
        ui->winQuestRadioButtonQuest4->setAutoExclusive(false);
        ui->winQuestRadioButtonQuest4->setChecked(false);
        ui->winQuestRadioButtonQuest4->setAutoExclusive(true);
        ui->winQuestCheckBoxQuest1->setChecked(false);
        ui->winQuestCheckBoxQuest2->setChecked(false);
        ui->winQuestCheckBoxQuest3->setChecked(false);
        ui->winQuestCheckBoxQuest4->setChecked(false);
        /* END сбросить выделенные элементы */

        if (numberPosInVector > quint32(stringVector.size())) { // если вопросов больше нет
            Results(); // вывод рез-тов
        }

        numberQuesting++;

    }
}
void WinQuestion::clickedOnWinQuestBtnEnd() {
    int answer = QMessageBox::question(this, tr("MedicTesting"),
                                       tr("Вы точно хотите закончить?"),
                                       QMessageBox::Yes| QMessageBox::No,
                                       QMessageBox::Yes);
    if (answer == QMessageBox::Yes) {
        Results();
    }
}

void WinQuestion::Results() {

    /* START скрыть с экрана элементы */
    //ui->winQuestLabelTitleQuest->setVisible(false);
    //ui->winQuestLabelOptionalInfo->setVisible(false);
    ui->winQuestGroupBoxMulti->setVisible(false);
    ui->winQuestGroupBoxSingle->setVisible(false);
    ui->winQuestCheckBoxQuest1->setVisible(false);
    ui->winQuestCheckBoxQuest2->setVisible(false);
    ui->winQuestCheckBoxQuest3->setVisible(false);
    ui->winQuestCheckBoxQuest4->setVisible(false);
    ui->winQuestRadioButtonQuest1->setVisible(false);
    ui->winQuestRadioButtonQuest2->setVisible(false);
    ui->winQuestRadioButtonQuest3->setVisible(false);
    ui->winQuestRadioButtonQuest4->setVisible(false);
    //ui->winQuestLabelOkError->setVisible(false);
    //ui->winQuestLabelOkErrorLable->setVisible(false);
    /* END скрыть с экрана элементы */

    ui->winQuestBtnNext->setEnabled(false); // блокируется кнопка Далее
    ui->winQuestBtnEnd->setEnabled(false); // блокируется кнопка Закончить

    /* далее выводятся данные с результатами */
    ui->winQuestLabelTitleQuest->setText(QObject::trUtf8("Тест пройден!"));
    ui->winQuestLabelTitleQuest->setStyleSheet("font-size:14pt");

    QString str = QString(QObject::trUtf8("Всего вопросов:                        %1.\n"
                                          "Кол-во правильных ответов:      %2.\n"
                                          "Кол-во неправильных ответов:   %3.\n"
                                          "Тест пройден на                   %4%.")
                          )
            .arg(numberQuesting)
            .arg(amountCorrectResponce)
            .arg(numberQuesting-amountCorrectResponce)
            .arg((amountCorrectResponce * 100)/numberQuesting);

    ui->winQuestLabelOptionalInfo->setText(str);
    ui->winQuestLabelOptionalInfo->setStyleSheet("font-size:11pt");
    ui->winQuestLabelOptionalInfo->setGeometry(160,90,261,141);
}
