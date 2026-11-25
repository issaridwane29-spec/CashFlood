// compteoperationservice.cpp
#include "compteoperationservice.h"

QHash<int, QString> CompteOperationService::construireMapComptes(const QList<Compte> &comptes) const
{
    QHash<int, QString> map;
    for (const Compte &cpt : comptes) {
        map.insert(cpt.id(), cpt.nom());
    }
    return map;
}

QHash<int, QString> CompteOperationService::construireMapCategories(const QList<Categorie> &categories) const
{
    QHash<int, QString> map;
    for (const Categorie &cat : categories) {
        map.insert(cat.id(), cat.nom());
    }
    return map;
}

QList<Operation> CompteOperationService::filtrerOperations(const QList<Operation> &operations,
                                                          const FiltreTransaction &filtre) const
{
    QList<Operation> resultat;
    const QString motCleTrim = filtre.motCle.trimmed();

    for (const Operation &op : operations) {
        if (!filtre.typeNormalise.isEmpty() && op.type() != filtre.typeNormalise) {
            continue;
        }
        if (filtre.idCompte > 0 && op.idCompte() != filtre.idCompte) {
            continue;
        }
        if (filtre.idCategorie == 0 && op.idCategorie() > 0) {
            continue;
        }
        if (filtre.idCategorie > 0 && op.idCategorie() != filtre.idCategorie) {
            continue;
        }
        if (!motCleTrim.isEmpty() && !op.description().contains(motCleTrim, Qt::CaseInsensitive)) {
            continue;
        }
        if (filtre.filtrerParDate && op.date().isValid()) {
            if (filtre.dateDebut.isValid() && op.date() < filtre.dateDebut) {
                continue;
            }
            if (filtre.dateFin.isValid() && op.date() > filtre.dateFin) {
                continue;
            }
        }
        resultat.append(op);
    }

    return resultat;
}

double CompteOperationService::calculerSoldeCompte(const Compte &compte, const QList<Operation> &operations) const
{
    double solde = compte.soldeInitial();
    for (const Operation &op : operations) {
        if (op.idCompte() != compte.id()) {
            continue;
        }
        const QString typeNorm = op.type().trimmed().toLower();
        if (typeNorm == "revenu") {
            solde += op.montant();
        } else if (typeNorm == "depense") {
            solde -= op.montant();
        }
    }
    return solde;
}
