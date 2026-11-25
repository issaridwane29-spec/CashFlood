#ifndef VALIDATEUR_H
#define VALIDATEUR_H

#include <QString>
#include <compte.h>
#include <categorie.h>
#include <operation.h>
#include <budget.h>

class Validateur
{
public:
    Validateur();
    static QString validerCompte(const Compte &c);
    static QString validerCategorie(const Categorie &c);
    static QString validerOperation(const Operation &o);
    static QString validerBudget(const Budget &b);
};

#endif // VALIDATEUR_H
