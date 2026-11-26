#include "comptedao.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

CompteDao::CompteDao() {}

QString CompteDao::derniereErreur() const { return m_erreur; }

bool CompteDao::inserer(Compte &compte)
{
    QSqlQuery q;
    q.prepare("INSERT INTO compte (nomcmpt, typecmpt, soldeinitial, ordreaffichage) "
              "VALUES (:nom, :type, :solde, :ordre)");
    q.bindValue(":nom", compte.nom());
    q.bindValue(":type", compte.type());
    q.bindValue(":solde", compte.soldeInitial());
    q.bindValue(":ordre", compte.ordre());

    if (!q.exec()) {
        m_erreur = q.lastError().text();
        return false;
    }
    compte.setId(q.lastInsertId().toInt());
    return true;
}

bool CompteDao::modifier(const Compte &compte)
{
    QSqlQuery q;
    q.prepare("UPDATE compte SET nomcmpt = :nom, typecmpt = :type, "
              "soldeinitial = :solde, ordreaffichage = :ordre "
              "WHERE idcmpt = :id");
    q.bindValue(":nom", compte.nom());
    q.bindValue(":type", compte.type());
    q.bindValue(":solde", compte.soldeInitial());
    q.bindValue(":ordre", compte.ordre());
    q.bindValue(":id", compte.id());

    if (!q.exec()) {
        m_erreur = q.lastError().text();
        return false;
    }
    return true;
}

bool CompteDao::supprimer(int id)
{
    QSqlQuery q;
    q.prepare("DELETE FROM compte WHERE idcmpt = :id");
    q.bindValue(":id", id);

    if (!q.exec()) {
        m_erreur = q.lastError().text();
        return false;
    }
    return true;
}

QList<Compte> CompteDao::lister()
{
    QList<Compte> liste;
    QSqlQuery q("SELECT idcmpt, nomcmpt, typecmpt, soldeinitial, ordreaffichage "
                "FROM compte ORDER BY ordreaffichage");

    while (q.next()) {
        Compte c(q.value(0).toInt(),
                 q.value(1).toString(),
                 q.value(2).toString(),
                 q.value(3).toDouble(),
                 q.value(4).toInt());
        liste.append(c);
    }
    return liste;
}

bool CompteDao::trouver(int id, Compte &resultat)
{
    QSqlQuery q;
    q.prepare("SELECT idcmpt, nomcmpt, typecmpt, soldeinitial, ordreaffichage "
              "FROM compte WHERE idcmpt = :id");
    q.bindValue(":id", id);

    if (!q.exec()) {
        m_erreur = q.lastError().text();
        return false;
    }
    if (!q.next()) {
        m_erreur = "Compte introuvable";
        return false;
    }
    resultat = Compte(q.value(0).toInt(),
                      q.value(1).toString(),
                      q.value(2).toString(),
                      q.value(3).toDouble(),
                      q.value(4).toInt());
    return true;
}

double CompteDao::solde(int idCompte, bool *ok)
{
    if (ok) *ok = false;

    QSqlQuery q;
    q.prepare(
        "SELECT c.soldeinitial + COALESCE(SUM(CASE WHEN o.typeope = 'revenu' "
        "THEN o.montant ELSE -o.montant END), 0) "
        "FROM compte c LEFT JOIN operation o ON o.idcmpt = c.idcmpt "
        "WHERE c.idcmpt = :id GROUP BY c.idcmpt");
    q.bindValue(":id", idCompte);

    if (!q.exec()) {
        m_erreur = q.lastError().text();
        return 0.0;
    }
    if (!q.next()) {
        m_erreur = "Compte introuvable";
        return 0.0;
    }

    if (ok) *ok = true;
    return q.value(0).toDouble();
}
