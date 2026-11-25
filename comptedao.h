#ifndef COMPTEDAO_H
#define COMPTEDAO_H

#include <QList>
#include <QString>
#include "compte.h"

class CompteDao
{
public:
    CompteDao();

    bool inserer(Compte &compte);
    bool modifier(const Compte &compte);
    bool supprimer(int id);
    QList<Compte> lister();
    bool trouver(int id, Compte &resultat);

    QString derniereErreur() const;

private:
    QString m_erreur;
};

#endif // COMPTEDAO_H
