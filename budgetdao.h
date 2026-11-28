#ifndef BUDGETDAO_H
#define BUDGETDAO_H

#include <QList>
#include <QString>
#include "budget.h"

class BudgetDao
{
public:
    BudgetDao();

    bool inserer(Budget &budget);
    bool modifier(const Budget &budget);
    bool supprimer(int id);
    QList<Budget> lister();
    bool trouver(int id, Budget &resultat);

    QString derniereErreur() const;

private:
    bool enregistrerCategories(int idBudget, const QList<int> &categories);
    QList<int> chargerCategories(int idBudget);

    QString m_erreur;
};

#endif // BUDGETDAO_H
