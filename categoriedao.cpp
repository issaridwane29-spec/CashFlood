#include "categoriedao.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

CategorieDao::CategorieDao() {}

QString CategorieDao::derniereErreur() const { return m_erreur; }

bool CategorieDao::inserer(Categorie &categorie)
{
    QSqlQuery q;
    q.prepare("INSERT INTO categorie (nomcat, couleur) VALUES (:nom, :couleur)");
    q.bindValue(":nom", categorie.nom());
    q.bindValue(":couleur", categorie.couleur());

    if (!q.exec()) {
        m_erreur = q.lastError().text();
        return false;
    }
    categorie.setId(q.lastInsertId().toInt());
    return true;
}

bool CategorieDao::modifier(const Categorie &categorie)
{
    QSqlQuery q;
    q.prepare("UPDATE categorie SET nomcat = :nom, couleur = :couleur "
              "WHERE idcat = :id");
    q.bindValue(":nom", categorie.nom());
    q.bindValue(":couleur", categorie.couleur());
    q.bindValue(":id", categorie.id());

    if (!q.exec()) {
        m_erreur = q.lastError().text();
        return false;
    }
    return true;
}

bool CategorieDao::supprimer(int id)
{
    QSqlQuery q;
    q.prepare("DELETE FROM categorie WHERE idcat = :id");
    q.bindValue(":id", id);

    if (!q.exec()) {
        m_erreur = q.lastError().text();
        return false;
    }
    return true;
}

QList<Categorie> CategorieDao::lister()
{
    QList<Categorie> liste;
    QSqlQuery q("SELECT idcat, nomcat, couleur FROM categorie ORDER BY nomcat");

    while (q.next()) {
        liste.append(Categorie(q.value(0).toInt(),
                               q.value(1).toString(),
                               q.value(2).toString()));
    }
    return liste;
}

bool CategorieDao::trouver(int id, Categorie &resultat)
{
    QSqlQuery q;
    q.prepare("SELECT idcat, nomcat, couleur FROM categorie WHERE idcat = :id");
    q.bindValue(":id", id);

    if (!q.exec()) {
        m_erreur = q.lastError().text();
        return false;
    }
    if (!q.next()) {
        m_erreur = "Categorie introuvable";
        return false;
    }
    resultat = Categorie(q.value(0).toInt(),
                         q.value(1).toString(),
                         q.value(2).toString());
    return true;
}
