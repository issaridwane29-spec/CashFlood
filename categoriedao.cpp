#include "categoriedao.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

CategorieDao::CategorieDao(const QString &connectionName)
    : m_connectionName(connectionName)
{
}

QString CategorieDao::derniereErreur() const { return m_erreur; }

bool CategorieDao::inserer(Categorie &categorie)
{
    m_erreur.clear();
    QSqlDatabase db = m_connectionName.isEmpty()
        ? QSqlDatabase::database()
        : QSqlDatabase::database(m_connectionName);

    QSqlQuery q(db);
    q.prepare("INSERT INTO categorie (nomcat, couleur) VALUES (:nom, :couleur)");
    q.bindValue(":nom", categorie.nom());
    q.bindValue(":couleur", categorie.couleur());

    if (!q.exec()) {
        m_erreur = q.lastError().text();
        return false;
    }

    const int nouvelId = q.lastInsertId().toInt();
    if (nouvelId <= 0) {
        m_erreur = "Impossible de récupérer l'identifiant de la catégorie insérée.";
        return false;
    }
    categorie.setId(nouvelId);
    return true;
}

bool CategorieDao::modifier(const Categorie &categorie)
{
    m_erreur.clear();
    QSqlDatabase db = m_connectionName.isEmpty()
        ? QSqlDatabase::database()
        : QSqlDatabase::database(m_connectionName);

    QSqlQuery q(db);
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
    m_erreur.clear();
    QSqlDatabase db = m_connectionName.isEmpty()
        ? QSqlDatabase::database()
        : QSqlDatabase::database(m_connectionName);

    QSqlQuery q(db);
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
    m_erreur.clear();
    QList<Categorie> liste;
    QSqlDatabase db = m_connectionName.isEmpty()
        ? QSqlDatabase::database()
        : QSqlDatabase::database(m_connectionName);

    QSqlQuery q(db);
    if (!q.exec("SELECT idcat, nomcat, couleur FROM categorie ORDER BY nomcat")) {
        m_erreur = q.lastError().text();
        return liste;
    }

    while (q.next()) {
        liste.append(Categorie(q.value(0).toInt(),
                               q.value(1).toString(),
                               q.value(2).toString()));
    }
    return liste;
}

bool CategorieDao::trouver(int id, Categorie &resultat)
{
    m_erreur.clear();
    QSqlDatabase db = m_connectionName.isEmpty()
        ? QSqlDatabase::database()
        : QSqlDatabase::database(m_connectionName);

    QSqlQuery q(db);
    q.prepare("SELECT idcat, nomcat, couleur FROM categorie WHERE idcat = :id");
    q.bindValue(":id", id);

    if (!q.exec()) {
        m_erreur = q.lastError().text();
        return false;
    }
    if (!q.next()) {
        m_erreur = "Catégorie introuvable";
        return false;
    }
    resultat = Categorie(q.value(0).toInt(),
                         q.value(1).toString(),
                         q.value(2).toString());
    return true;
}
