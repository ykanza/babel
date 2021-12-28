#ifndef SECDIALOG_HPP
#define SECDIALOG_HPP
#include <QDialog>
#include "../client/include/network.hpp"

namespace Ui {
class SecDialog;
}

class SecDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SecDialog(QWidget *parent = nullptr);
    ~SecDialog();

private slots:
    void on_hangUp_clicked();
    void addUsersToScroller();
    void on_refreshBtn_clicked();
    void on_callBtn_clicked();
    void on_ConnectToServer_clicked();


private:
    Ui::SecDialog *ui;
    std::unique_ptr<networkCli> _network;

};

#endif // SECDIALOG_HPP
