#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "secdialog.hpp"
#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:

    std::unique_ptr<Ui::MainWindow> ui;
    std::unique_ptr<SecDialog> _sec;
};
#endif // MAINWINDOW_H
