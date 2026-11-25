#ifndef OPERATIONDAO_H
#define OPERATIONDAO_H

#include <QList>
#include <QString>
#include <QSqlQuery>
#include "operation.h"

class OperationDao
{
public:
    OperationDao();

    bool inserer(Operation &operation);
    bool modifier(const Operation &operation);
    bool supprimer(int id);
    QList<Operation> lister();
    QList<Operation> listerParCompte(int idCompte);
    bool trouver(int id, Operation &resultat);

    QString derniereErreur() const;

private:
    Operation depuisRequete(const QSqlQuery &q) const;
    QString m_erreur;
};

#endif // OPERATIONDAO_H
