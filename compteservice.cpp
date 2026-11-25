//compteservice.cpp
#include "compteservice.h"
#include "comptedao.h"
#include "operationdao.h"
#include "validateur.h"
#include <QSqlDatabase>
#include <QSqlError>

CompteService::CompteService(const QString &connectionName)
    : m_connectionName(connectionName)
{
}

double CompteService::calculerSoldeActuel(const Compte &compte) const
{
    m_derniereErreur.clear();

    OperationDao operationDao;
    const QList<Operation> operations = operationDao.lister();
    if (!operationDao.derniereErreur().isEmpty()) {
        m_derniereErreur = operationDao.derniereErreur();
        return compte.soldeInitial();
    }

    double mouvementNet = 0.0;
    for (const Operation &op : operations) {
        if (op.idCompte() == compte.id()) {
            const QString typeNorm = op.type().trimmed().toLower();
            if (typeNorm == "revenu") {
                mouvementNet += op.montant();
            } else if (typeNorm == "depense") {
                mouvementNet -= op.montant();
            }
        }
    }

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

    compteA.setOrdre(ancienOrdreB);
    compteB.setOrdre(ancienOrdreA);

    const QString errA = Validateur::validerCompte(compteA);
    const QString errB = Validateur::validerCompte(compteB);
    if (!errA.isEmpty() || !errB.isEmpty()) {
        m_derniereErreur = !errA.isEmpty() ? errA : errB;
        compteA.setOrdre(ancienOrdreA);
        compteB.setOrdre(ancienOrdreB);
        return false;
    }

    CompteDao dao;

    if (!db.transaction()) {
        m_derniereErreur = db.lastError().text();
        compteA.setOrdre(ancienOrdreA);
        compteB.setOrdre(ancienOrdreB);
        return false;
    }

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
