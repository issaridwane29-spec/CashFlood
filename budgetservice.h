#ifndef BUDGETSERVICE_H
#define BUDGETSERVICE_H

#include <QList>
#include <QString>
#include "budget.h"

struct DepenseCategorie {
    int idCategorie;
    QString nomCategorie;
    QString couleur;
    double totalDepense;
};

struct SuiviBudget {
    double depense = 0.0;
    double pourcentage = 0.0;
    double reste = 0.0;
    bool depasse = false;
};

class BudgetService
{
public:
    explicit BudgetService(const QString &connectionName = QString());

    double calculerDepenseActuelle(const Budget &budget) const;
    double calculerPourcentage(const Budget &budget) const;
    double calculerResteDisponible(const Budget &budget) const;
    bool estDepasse(const Budget &budget) const;
    SuiviBudget calculerSuivi(const Budget &budget) const;

    QList<DepenseCategorie> depensesParCategorie(const QString &periode = "mensuel") const;

    QString derniereErreur() const;

private:
    QString conditionPeriodeSql(const QString &periode) const;

    QString m_connectionName;
    mutable QString m_derniereErreur;
};

#endif // BUDGETSERVICE_H
