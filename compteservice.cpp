#include "compteservice.h"
#include "comptedao.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

CompteService::CompteService(const QString &connectionName)
    : m_connectionName(connectionName)
{
}

double CompteService::calculerSoldeActuel(const Compte &compte) const
{
    m_derniereErreur.clear();

    QSqlDatabase db = m_connectionName.isEmpty()
        ? QSqlDatabase::database()
        : QSqlDatabase::database(m_connectionName);

    if (!db.isOpen()) {
        m_derniereErreur = "La base de données n'est pas ouverte.";
        return compte.soldeInitial();
    }

    QSqlQuery query(db);
    query.prepare(
        "SELECT COALESCE(SUM(CASE "
        "WHEN typeope = 'revenu' THEN montant "
        "WHEN typeope = 'depense' THEN -montant "
        "ELSE 0 END), 0.0) "
        "FROM operation WHERE idcmpt = :idCompte"
    );
    query.bindValue(":idCompte", compte.id());

    if (!query.exec() || !query.next()) {
        m_derniereErreur = query.lastError().text();
        return compte.soldeInitial();
    }

    const double mouvementNet = query.value(0).toDouble();
    return compte.soldeInitial() + mouvementNet;
}

double CompteService::calculerSoldeTotal(const QList<Compte> &comptes) const
{
    double total = 0.0;
    for (const Compte &cpt : comptes) {
        total += calculerSoldeActuel(cpt);
    }
    return total;
}

bool CompteService::echangerOrdre(Compte &compteA, Compte &compteB)
{
    m_derniereErreur.clear();

    QSqlDatabase db = m_connectionName.isEmpty()
        ? QSqlDatabase::database()
        : QSqlDatabase::database(m_connectionName);

    if (!db.isOpen()) {
        m_derniereErreur = "La base de données n'est pas ouverte.";
        return false;
    }

    const int ancienOrdreA = compteA.ordre();
    const int ancienOrdreB = compteB.ordre();

    if (ancienOrdreA == ancienOrdreB) {
        return true;
    }

    CompteDao dao;

    if (!db.transaction()) {
        m_derniereErreur = db.lastError().text();
        return false;
    }

    compteA.setOrdre(ancienOrdreB);
    compteB.setOrdre(ancienOrdreA);

    if (!dao.modifier(compteA) || !dao.modifier(compteB)) {
        m_derniereErreur = dao.derniereErreur().isEmpty()
            ? db.lastError().text()
            : dao.derniereErreur();
        compteA.setOrdre(ancienOrdreA);
        compteB.setOrdre(ancienOrdreB);
        db.rollback();
        return false;
    }

    if (!db.commit()) {
        m_derniereErreur = db.lastError().text();
        compteA.setOrdre(ancienOrdreA);
        compteB.setOrdre(ancienOrdreB);
        db.rollback();
        return false;
    }

    return true;
}

QString CompteService::derniereErreur() const
{
    return m_derniereErreur;
}
