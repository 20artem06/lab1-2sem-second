#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtCore/QProcess>
#include <iostream>
#include "shared_ptr.h"
#include "unique_ptr.h"
#include "weak_ptr.h"


struct TestObject {
    int id;
    TestObject(int i) : id(i) { std::cout << "TestObject(" << id << ") ñîçäàí.\n"; }
    ~TestObject() { std::cout << "TestObject(" << id << ") óíè÷òîæåí.\n"; }
    void greet() { std::cout << "Ïðèâåò îò TestObject(" << id << ")!\n"; }
};

class MainWindow : public QMainWindow
{
    //Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnCreateShared_clicked();
    void on_btnCreateUnique_clicked();
    void on_btnCreateWeak_clicked();
    void on_btnRunFunctionalTests_clicked();
    void on_btnRunStressTests_clicked();

    void functionalTestsFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void stressTestsFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:

    QPushButton* btnCreateShared;
    QPushButton* btnCreateUnique;
    QPushButton* btnCreateWeak;
    QPushButton* btnRunFunctionalTests;
    QPushButton* btnRunStressTests;
    QTextEdit* textEditLog;

    shared_ptr<TestObject> sp;
    unique_ptr<TestObject> up;
    weak_ptr<TestObject> wp;


    QProcess* functionalTestProcess;
    QProcess* stressTestProcess;

    void logMessage(const QString& message);
};

#endif
