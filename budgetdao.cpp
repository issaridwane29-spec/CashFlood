#include "budgetdao.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

BudgetDao::BudgetDao(const QString &connectionName)
    : m_connectionName(connectionName)
{
}

QString BudgetDao::derniereErreur() const { return m_erreur; }

bool BudgetDao::enregistrerCategories(int idBudget, const QList<int> &categories)
{
    QSqlDatabase db = m_connectionName.isEmpty()
        ? QSqlDatabase::database()
        : QSqlDatabase::database(m_connectionName);

    QSqlQuery del(db);
    del.prepare("DELETE FROM budget_categorie WHERE idbud = :id");
    del.bindValue(":id", idBudget);
    if (!del.exec()) {
        m_erreur = del.lastError().text();
        return false;
    }

    for (int idCat : categories) {
        QSqlQuery ins(db);
        ins.prepare("INSERT INTO budget_categorie (idbud, idcat) VALUES (:bud, :cat)");
        ins.bindValue(":bud", idBudget);
        ins.bindValue(":cat", idCat);
        if (!ins.exec()) {
            m_erreur = ins.lastError().text();
            return false;
        }
    }
    return true;
}

QList<int> BudgetDao::chargerCategories(int idBudget)
{
    QList<int> ids;
    QSqlDatabase db = m_connectionName.isEmpty()
        ? QSqlDatabase::database()
        : QSqlDatabase::database(m_connectionName);

    QSqlQuery q(db);
    q.prepare("SELECT idcat FROM budget_categorie WHERE idbud = :id");
    q.bindValue(":id", idBudget);
    if (!q.exec()) {
        m_erreur = q.lastError().text();
        return ids;
    }

    while (q.next()) {
        ids.append(q.value(0).toInt());
    }
    return ids;
}

bool BudgetDao::inserer(Budget &b)
{
    m_erreur.clear();
    QSqlDatabase db = m_connectionName.isEmpty()
        ? QSqlDatabase::database()
        : QSqlDatabase::database(m_connectionName);

    if (!db.transaction()) {
        m_erreur = db.lastError().text();
        return false;
    }

    QSqlQuery q(db);
    q.prepare("INSERT INTO budget (nombud, plafond, periode) "
              "VALUES (:nom, :plafond, :periode)");
    q.bindValue(":nom", b.nom());
    q.bindValue(":plafond", b.plafond());
    q.bindValue(":periode", b.periode());

    if (!q.exec()) {
        m_erreur = q.lastError().text();
        db.rollback();
        return false;
    }

    const int nouvelId = q.lastInsertId().toInt();
    if (nouvelId <= 0) {
        m_erreur = "Impossible de récupérer l'identifiant du budget inséré.";
        db.rollback();
        return false;
    }
    b.setId(nouvelId);

    if (!enregistrerCategories(b.id(), b.categories())) {
        db.rollback();
        return false;
    }
    if (!db.commit()) {
        m_erreur = db.lastError().text();
        db.rollback();
        return false;
    }
    return true;
}

bool BudgetDao::modifier(const Budget &b)
{
    m_erreur.clear();
    QSqlDatabase db = m_connectionName.isEmpty()
        ? QSqlDatabase::database()
        : QSqlDatabase::database(m_connectionName);

    if (!db.transaction()) {
        m_erreur = db.lastError().text();
        return false;
    }

    QSqlQuery q(db);
    q.prepare("UPDATE budget SET nombud = :nom, plafond = :plafond, periode = :periode "
              "WHERE idbud = :id");
    q.bindValue(":nom", b.nom());
    q.bindValue(":plafond", b.plafond());
    q.bindValue(":periode", b.periode());
    q.bindValue(":id", b.id());

    if (!q.exec()) {
        m_erreur = q.lastError().text();
        db.rollback();
        return false;
    }
    if (!enregistrerCategories(b.id(), b.categories())) {
        db.rollback();
        return false;
    }
    if (!db.commit()) {
        m_erreur = db.lastError().text();
        db.rollback();
        return false;
    }
    return true;
}

bool BudgetDao::supprimer(int id)
{
    m_erreur.clear();
    QSqlDatabase db = m_connectionName.isEmpty()
        ? QSqlDatabase::database()
        : QSqlDatabase::database(m_connectionName);

    QSqlQuery q(db);
    q.prepare("DELETE FROM budget WHERE idbud = :id");
    q.bindValue(":id", id);

    if (!q.exec()) {
        m_erreur = q.lastError().text();
        return false;
    }
    return true;
}

QList<Budget> BudgetDao::lister()
{
    m_erreur.clear();
    QList<Budget> liste;
    QSqlDatabase db = m_connectionName.isEmpty()
        ? QSqlDatabase::database()
        : QSqlDatabase::database(m_connectionName);

    QSqlQuery q(db);
    if (!q.exec("SELECT idbud, nombud, plafond, periode FROM budget ORDER BY nombud")) {
        m_erreur = q.lastError().text();
        return liste;
    }

    while (q.next()) {
        liste.append(Budget(q.value(0).toInt(),
                            q.value(1).toString(),
                            q.value(2).toDouble(),
                            q.value(3).toString(),
                            QList<int>()));
    }

    for (Budget &b : liste) {
        b.setCategories(chargerCategories(b.id()));
    }
    return liste;
}

bool BudgetDao::trouver(int id, Budget &resultat)
{
    m_erreur.clear();
    QSqlDatabase db = m_connectionName.isEmpty()
        ? QSqlDatabase::database()
        : QSqlDatabase::database(m_connectionName);

    QSqlQuery q(db);
    q.prepare("SELECT idbud, nombud, plafond, periode FROM budget WHERE idbud = :id");
    q.bindValue(":id", id);

    if (!q.exec()) {
        m_erreur = q.lastError().text();
        return false;
    }
    if (!q.next()) {
        m_erreur = "Budget introuvable";
        return false;
    }
    resultat = Budget(q.value(0).toInt(),
                      q.value(1).toString(),
                      q.value(2).toDouble(),
                      q.value(3).toString(),
                      QList<int>());
    resultat.setCategories(chargerCategories(resultat.id()));
    return true;
}
