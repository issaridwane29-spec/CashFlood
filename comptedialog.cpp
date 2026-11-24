#include "comptedialog.h"
#include "ui_comptedialog.h"

CompteDialog::CompteDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CompteDialog)
{
    ui->setupUi(this);
}

CompteDialog::~CompteDialog()
{
    delete ui;
}
