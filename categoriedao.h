#ifndef CATEGORIEDAO_H
#define CATEGORIEDAO_H

#include <QList>
#include <QString>
#include "categorie.h"

class CategorieDao
{
public:
    explicit CategorieDao(const QString &connectionName = QString());

    bool inserer(Categorie &categorie);
    bool modifier(const Categorie &categorie);
    bool supprimer(int id);
    QList<Categorie> lister();
    bool trouver(int id, Categorie &resultat);

    QString derniereErreur() const;

private:
    QString m_connectionName;
    QString m_erreur;
};

#endif // CATEGORIEDAO_H
