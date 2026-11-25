#ifndef TRANSACTIONDIALOG_H
#define TRANSACTIONDIALOG_H

#include <QDialog>
#include <QList>
#include "operation.h"
#include "compte.h"
#include "categorie.h"

namespace Ui {
class TransactionDialog;
}

class TransactionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TransactionDialog(QWidget *parent = nullptr);
    ~TransactionDialog();

    void setComptesEtCategories(const QList<Compte> &comptes,
                                const QList<Categorie> &categories);
    void chargerOperation(const Operation &operation);
    Operation operationResultat() const;

private slots:
    void onAccepted();

private:
    Ui::TransactionDialog *ui;
    int m_idOperation = 0;
};

#endif // TRANSACTIONDIALOG_H
