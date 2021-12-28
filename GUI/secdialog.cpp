#include "secdialog.hpp"
#include "ui_secdialog.h"
#include <iostream>
#include <QMessageBox>
#include "Exception/Error.hpp"

SecDialog::SecDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SecDialog)
{
    ui->setupUi(this);
    this->_network = std::make_unique<networkCli>();
}

SecDialog::~SecDialog()
{
    delete ui;
}


void SecDialog::on_callBtn_clicked()
{
    QString itemSelected = ui->comboBox->currentText();
    if (itemSelected.isEmpty())
        return;
    this->_network->callOtherUser(itemSelected.toInt());
}

void SecDialog::on_ConnectToServer_clicked()
{
    QString addressIp = ui->address_line->text();
    QString port = ui->port_line->text();

    try {
        this->_network->connectToServer(addressIp.toStdString(), port.toInt());
    } catch (ConnectionError const &err) {
        QMessageBox::warning(this, "Error",  err.what());
        return;
    }
    catch (const TorchBearerError& err) {
        QMessageBox::warning(this, "Error",  err.what());
        return;
    }

    QMessageBox::information(this, "user connected", "You are connected");
    QObject::connect(this->_network.get(), &networkCli::signalUserList,
                     this, &SecDialog::addUsersToScroller);
    this->_network->askForUserList();
    QObject::connect(ui->hangUp, SIGNAL(clicked()), this, SLOT(this->on_hangUp_clicked()));
}

void SecDialog::on_refreshBtn_clicked()
{
    this->_network->askForUserList();
}

void SecDialog::addUsersToScroller()
{
    std::vector<int> userList = this->_network->getUserList();

    for (const auto &elem: userList)
        ui->comboBox->addItem(std::to_string(elem).c_str());
}


void SecDialog::on_hangUp_clicked()
{
    this->_network->hangUp();
}
