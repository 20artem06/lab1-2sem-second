#include "mainwindow.h"
#include <QtWidgets/QMessageBox>
#include <QtCore/QFile>
#include <QtCore/QDir>
//#include <QtCore/QTextStream>




MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , btnCreateShared(new QPushButton("Ñîçäàòü shared_ptr", this))
    , btnCreateUnique(new QPushButton("Ñîçäàòü unique_ptr", this))
    , btnCreateWeak(new QPushButton("Ñîçäàòü weak_ptr", this))
    , btnRunFunctionalTests(new QPushButton("Çàïóñòèòü Ôóíêöèîíàëüíûå Òåñòû", this))
    , btnRunStressTests(new QPushButton("Çàïóñòèòü Íàãðóçî÷íûå Òåñòû", this))
    , textEditLog(new QTextEdit(this))
    , functionalTestProcess(new QProcess(this))
    , stressTestProcess(new QProcess(this))
{
    // Íàñòðîéêà UI
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    // Ïåðâàÿ ñòðîêà êíîïîê
    QHBoxLayout* layout1 = new QHBoxLayout();
    layout1->addWidget(btnCreateShared);
    layout1->addWidget(btnCreateUnique);
    layout1->addWidget(btnCreateWeak);
    mainLayout->addLayout(layout1);

    // Âòîðàÿ ñòðîêà êíîïîê
    QHBoxLayout* layout2 = new QHBoxLayout();
    layout2->addWidget(btnRunFunctionalTests);
    layout2->addWidget(btnRunStressTests);
    mainLayout->addLayout(layout2);

    // Ëîã òåêñòîâîãî ðåäàêòîðà
    textEditLog->setReadOnly(true);
    mainLayout->addWidget(textEditLog);

    // Ïîäêëþ÷åíèå ñèãíàëîâ ê ñëîòàì
    connect(btnCreateShared, &QPushButton::clicked, this, &MainWindow::on_btnCreateShared_clicked);
    connect(btnCreateUnique, &QPushButton::clicked, this, &MainWindow::on_btnCreateUnique_clicked);
    connect(btnCreateWeak, &QPushButton::clicked, this, &MainWindow::on_btnCreateWeak_clicked);
    connect(btnRunFunctionalTests, &QPushButton::clicked, this, &MainWindow::on_btnRunFunctionalTests_clicked);
    connect(btnRunStressTests, &QPushButton::clicked, this, &MainWindow::on_btnRunStressTests_clicked);

    // Ïîäêëþ÷åíèå ñèãíàëîâ çàâåðøåíèÿ ïðîöåññîâ ê ñëîòàì
    connect(functionalTestProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &MainWindow::functionalTestsFinished);
    connect(stressTestProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &MainWindow::stressTestsFinished);

    // Ïîäêëþ÷åíèå ñèãíàëîâ ãîòîâíîñòè äàííûõ
    connect(functionalTestProcess, &QProcess::readyReadStandardOutput, this, [this]() {
        QString output = functionalTestProcess->readAllStandardOutput();
        logMessage(output);
    });
    connect(functionalTestProcess, &QProcess::readyReadStandardError, this, [this]() {
        QString output = functionalTestProcess->readAllStandardError();
        logMessage(output);
    });

    connect(stressTestProcess, &QProcess::readyReadStandardOutput, this, [this]() {
        QString output = stressTestProcess->readAllStandardOutput();
        logMessage(output);
    });
    connect(stressTestProcess, &QProcess::readyReadStandardError, this, [this]() {
        QString output = stressTestProcess->readAllStandardError();
        logMessage(output);
    });
}

MainWindow::~MainWindow()
{

}

void MainWindow::logMessage(const QString& message)
{
    textEditLog->append(message);
}

void MainWindow::on_btnCreateShared_clicked()
{
    sp = shared_ptr<TestObject>(new TestObject(100));
    QString msg = QString("Ñîçäàí shared_ptr ñ use_count = %1").arg(sp.use_count());
    logMessage(msg);
}

void MainWindow::on_btnCreateUnique_clicked()
{
    up = unique_ptr<TestObject>(new TestObject(200));
    if (up) {
        QString msg = QString("Ñîçäàí unique_ptr ñ îáúåêòîì ID = %1").arg(up->id);
        logMessage(msg);
    }
}

void MainWindow::on_btnCreateWeak_clicked()
{
    wp = weak_ptr<TestObject>(sp);
    if (!wp.expired()) {
        shared_ptr<TestObject> sp_locked = wp.lock();
        QString msg = QString("Ñîçäàí weak_ptr. use_count = %1").arg(sp_locked.use_count());
        logMessage(msg);
    }
    else {
        logMessage("weak_ptr èñòåê. Îáúåêò óíè÷òîæåí.");
    }
}

void MainWindow::on_btnRunFunctionalTests_clicked()
{
    if (functionalTestProcess->state() != QProcess::NotRunning) {
        QMessageBox::information(this, "Èíôîðìàöèÿ", "Ôóíêöèîíàëüíûå òåñòû óæå çàïóñêàþòñÿ.");
        return;
    }

    QString testPath = QDir::currentPath() + "/functional_tests";

#ifdef Q_OS_WIN
    testPath += ".exe";
#endif

    if (!QFile::exists(testPath)) {
        QMessageBox::warning(this, "Îøèáêà", "Íå íàéäåí èñïîëíÿåìûé ôàéë functional_tests.");
        return;
    }

    logMessage("Çàïóñê ôóíêöèîíàëüíûõ òåñòîâ...");
    functionalTestProcess->start(testPath);
}

void MainWindow::on_btnRunStressTests_clicked()
{
    if (stressTestProcess->state() != QProcess::NotRunning) {
        QMessageBox::information(this, "Èíôîðìàöèÿ", "Íàãðóçî÷íûå òåñòû óæå çàïóñêàþòñÿ.");
        return;
    }

    QString testPath = QDir::currentPath() + "/stress_tests";

#ifdef Q_OS_WIN
    testPath += ".exe";
#endif

    if (!QFile::exists(testPath)) {
        QMessageBox::warning(this, "Îøèáêà", "Íå íàéäåí èñïîëíÿåìûé ôàéë stress_tests.");
        return;
    }

    logMessage("Çàïóñê íàãðóçî÷íûõ òåñòîâ...");
    stressTestProcess->start(testPath);
}

void MainWindow::functionalTestsFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitStatus == QProcess::NormalExit && exitCode == 0) {
        logMessage("Ôóíêöèîíàëüíûå òåñòû çàâåðøåíû óñïåøíî.");
    }
    else {
        logMessage("Ôóíêöèîíàëüíûå òåñòû çàâåðøèëèñü ñ îøèáêîé.");
    }
}

void MainWindow::stressTestsFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitStatus == QProcess::NormalExit && exitCode == 0) {
        logMessage("Íàãðóçî÷íûå òåñòû çàâåðøåíû óñïåøíî.");
    }
    else {
        logMessage("Íàãðóçî÷íûå òåñòû çàâåðøèëèñü ñ îøèáêîé.");
    }
}
