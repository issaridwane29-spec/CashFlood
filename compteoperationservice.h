// compteoperationservice.h
#ifndef COMPTEOPERATIONSERVICE_H
#define COMPTEOPERATIONSERVICE_H

#include <QDate>
#include <QHash>
#include <QList>
#include <QString>
#include "compte.h"
#include "categorie.h"
#include "operation.h"

struct FiltreTransaction
{
    QString motCle;
    QString typeNormalise;     // "" (tous), "depense", ou "revenu"
    int idCompte = -1;         // -1 = tous les comptes
    int idCategorie = -1;      // -1 = toutes, 0 = sans catégorie, >0 = catégorie précise
    QDate dateDebut;
    QDate dateFin;
    bool filtrerParDate = true;
};

class CompteOperationService
{
public:
    CompteOperationService() = default;

    QHash<int, QString> construireMapComptes(const QList<Compte> &comptes) const;
    QHash<int, QString> construireMapCategories(const QList<Categorie> &categories) const;

    QList<Operation> filtrerOperations(const QList<Operation> &operations,
                                       const FiltreTransaction &filtre) const;

    double calculerSoldeCompte(const Compte &compte,
                              const QList<Operation> &operations) const;
};

#endif // COMPTEOPERATIONSERVICE_H
