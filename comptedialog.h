#ifndef COMPTEDIALOG_H
#define COMPTEDIALOG_H

#include <QDialog>

namespace Ui {
class CompteDialog;
}

class CompteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CompteDialog(QWidget *parent = nullptr);
    ~CompteDialog();

private:
    Ui::CompteDialog *ui;
};

#endif // COMPTEDIALOG_H
