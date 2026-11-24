#ifndef COMPTEDIALOG_H
#define COMPTEDIALOG_H

#include <QDialog>
#include "compte.h"

namespace Ui {
class CompteDialog;
}

class CompteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CompteDialog(QWidget *parent = nullptr);
    ~CompteDialog();

    void chargerCompte(const Compte &compte);
    Compte compteResultat() const;

private slots:
    void onAccepted();

private:
    Ui::CompteDialog *ui;
    int m_idCompte = 0;
    int m_ordre = 0;
};

#endif // COMPTEDIALOG_H
