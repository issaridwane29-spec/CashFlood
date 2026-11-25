#include "transactiondialog.h"
#include "ui_transactiondialog.h"

TransactionDialog::TransactionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TransactionDialog)
{
    ui->setupUi(this);
}

TransactionDialog::~TransactionDialog()
{
    delete ui;
}
