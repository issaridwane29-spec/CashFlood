#ifndef BUDGETDIALOG_H
#define BUDGETDIALOG_H

#include <QDialog>
#include <QList>
#include "budget.h"
#include "categorie.h"

namespace Ui {
class BudgetDialog;
}

class BudgetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BudgetDialog(QWidget *parent = nullptr);
    ~BudgetDialog();

    void chargerCategoriesDisponibles(const QList<Categorie> &categories);
    void chargerBudget(const Budget &budget);
    Budget budgetResultat() const;

private slots:
    void onAccepted();

private:
    Ui::BudgetDialog *ui;
    int m_idBudget = 0;
};

#endif // BUDGETDIALOG_H
