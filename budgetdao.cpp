#include "budgetdao.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

BudgetDao::BudgetDao() {}

QString BudgetDao::derniereErreur() const { return m_erreur; }

bool BudgetDao::enregistrerCategories(int idBudget, const QList<int> &categories)
{
    QSqlQuery del;
    del.prepare("DELETE FROM budget_categorie WHERE idbud = :id");
    del.bindValue(":id", idBudget);
    if (!del.exec()) {
        m_erreur = del.lastError().text();
        return false;
    }

    for (int idCat : categories) {
        QSqlQuery ins;
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
    QSqlQuery q;
    q.prepare("SELECT idcat FROM budget_categorie WHERE idbud = :id");
    q.bindValue(":id", idBudget);
    if (q.exec()) {
        while (q.next())
            ids.append(q.value(0).toInt());
    }
    return ids;
}

bool BudgetDao::inserer(Budget &b)
{
    QSqlDatabase db = QSqlDatabase::database();
    db.transaction();

    QSqlQuery q;
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
    b.setId(q.lastInsertId().toInt());

    if (!enregistrerCategories(b.id(), b.categories())) {
        db.rollback();
        return false;
    }
    return db.commit();
}

bool BudgetDao::modifier(const Budget &b)
{
    QSqlDatabase db = QSqlDatabase::database();
    db.transaction();

    QSqlQuery q;
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
    return db.commit();
}

bool BudgetDao::supprimer(int id)
{
    QSqlQuery q;
    q.prepare("DELETE FROM budget WHERE idbud = :id");   // CASCADE nettoie budget_categorie
    q.bindValue(":id", id);

    if (!q.exec()) {
        m_erreur = q.lastError().text();
        return false;
    }
    return true;
}

QList<Budget> BudgetDao::lister()
{
    QList<Budget> liste;
    QSqlQuery q("SELECT idbud, nombud, plafond, periode FROM budget ORDER BY nombud");
    while (q.next()) {
        liste.append(Budget(q.value(0).toInt(),
                            q.value(1).toString(),
                            q.value(2).toDouble(),
                            q.value(3).toString(),
                            QList<int>()));
    }
    // on charge les categories apres la boucle, pas pendant
    for (Budget &b : liste)
        b.setCategories(chargerCategories(b.id()));
    return liste;
}

bool BudgetDao::trouver(int id, Budget &resultat)
{
    QSqlQuery q;
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
