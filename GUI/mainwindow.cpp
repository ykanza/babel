#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
}


void MainWindow::on_pushButton_clicked()
{
    QString username = ui->username_line->text();
    QString password = ui->password_line->text();


     if (username == "admin" && password == "admin") {
         QMessageBox::information(this, "Login", "username password is correct");
         hide();
         _sec = std::make_unique<SecDialog>(this);
         _sec->show();
     } else {
        QMessageBox::warning(this, "login", "bad password");
     }

}
