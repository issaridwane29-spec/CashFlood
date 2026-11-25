#ifndef COMPTESERVICE_H
#define COMPTESERVICE_H

#include <QList>
#include <QString>
#include "compte.h"


class CompteService
{
public:
    explicit CompteService(const QString &connectionName = QString());

    double calculerSoldeActuel(const Compte &compte) const;
    double calculerSoldeTotal(const QList<Compte> &comptes) const;
    bool echangerOrdre(Compte &compteA, Compte &compteB);

    QString derniereErreur() const;

private:
    QString m_connectionName;
    mutable QString m_derniereErreur;
};

#endif // COMPTESERVICE_H
