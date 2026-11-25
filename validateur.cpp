#include "validateur.h"
#include "QRegularExpression"


Validateur::Validateur()
{

}

QString Validateur::validerCompte(const Compte &c)
{
    if (c.nom().trimmed().isEmpty())
        return "Le nom du compte est obligatoire.";
    if (c.type().trimmed().isEmpty())
        return "Le type du compte est obligatoire.";
    return QString();
}

QString Validateur::validerCategorie(const Categorie &c)
{
    if (c.nom().trimmed().isEmpty())
        return "Le nom de la categorie est obligatoire.";
    QRegularExpression couleur("^#[0-9A-Fa-f]{6}$");
    if (!couleur.match(c.couleur()).hasMatch())
        return "La couleur doit etre au format #RRGGBB.";
    return QString();
}

QString Validateur::validerOperation(const Operation &o)
{
    if (o.type() != "revenu" && o.type() != "depense")
        return "Le type doit etre revenu ou depense.";
    if (o.montant() <= 0)
        return "Le montant doit etre superieur a 0.";
    if (!o.date().isValid())
        return "La date est invalide.";
    if (o.idCompte() <= 0)
        return "Un compte doit etre selectionne.";
    return QString();
}

QString Validateur::validerBudget(const Budget &b)
{
    if (b.nom().trimmed().isEmpty())
        return "Le nom du budget est obligatoire.";
    if (b.plafond() <= 0)
        return "Le plafond doit etre superieur a 0.";
    if (b.periode() != "hebdomadaire" && b.periode() != "mensuel"
        && b.periode() != "annuel")
        return "La periode doit etre hebdomadaire, mensuelle ou annuelle.";
    return QString();
}
