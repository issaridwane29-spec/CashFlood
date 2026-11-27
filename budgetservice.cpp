#include "budgetservice.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

BudgetService::BudgetService(const QString &connectionName)
    : m_connectionName(connectionName)
{
}

QString BudgetService::conditionPeriodeSql(const QString &periode) const
{
    if (periode == "hebdomadaire") {
        return "strftime('%Y-%W', o.dateope) = strftime('%Y-%W', 'now', 'localtime')";
    }
    if (periode == "annuel") {
        return "strftime('%Y', o.dateope) = strftime('%Y', 'now', 'localtime')";
    }
    return "strftime('%Y-%m', o.dateope) = strftime('%Y-%m', 'now', 'localtime')";
}

double BudgetService::calculerDepenseActuelle(const Budget &budget) const
{
    m_derniereErreur.clear();

    QSqlDatabase db = m_connectionName.isEmpty()
        ? QSqlDatabase::database()
        : QSqlDatabase::database(m_connectionName);

    if (!db.isOpen()) {
        m_derniereErreur = "La base de données n'est pas ouverte.";
        return 0.0;
    }

    const QString filtreDate = conditionPeriodeSql(budget.periode());

    QSqlQuery query(db);
    query.prepare(QString(
        "SELECT COALESCE(SUM(o.montant), 0.0) "
        "FROM operation o "
        "INNER JOIN budget_categorie bc ON o.idcat = bc.idcat "
        "WHERE bc.idbud = :idBudget "
        "  AND o.typeope = 'depense' "
        "  AND %1"
    ).arg(filtreDate));
    query.bindValue(":idBudget", budget.id());

    if (!query.exec()) {
        m_derniereErreur = query.lastError().text();
        return 0.0;
    }

    if (!query.next()) {
        return 0.0;
    }

    return query.value(0).toDouble();
}

SuiviBudget BudgetService::calculerSuivi(const Budget &budget) const
{
    SuiviBudget suivi;
    suivi.depense = calculerDepenseActuelle(budget);
    suivi.reste = budget.plafond() - suivi.depense;
    suivi.depasse = suivi.depense > budget.plafond();
    suivi.pourcentage = (budget.plafond() > 0.0)
        ? (suivi.depense / budget.plafond()) * 100.0
        : 0.0;
    return suivi;
}

double BudgetService::calculerPourcentage(const Budget &budget) const
{
    return calculerSuivi(budget).pourcentage;
}

double BudgetService::calculerResteDisponible(const Budget &budget) const
{
    return calculerSuivi(budget).reste;
}

bool BudgetService::estDepasse(const Budget &budget) const
{
    return calculerSuivi(budget).depasse;
}

QList<DepenseCategorie> BudgetService::depensesParCategorie(const QString &periode) const
{
    m_derniereErreur.clear();
    QList<DepenseCategorie> resultat;

    QSqlDatabase db = m_connectionName.isEmpty()
        ? QSqlDatabase::database()
        : QSqlDatabase::database(m_connectionName);

    if (!db.isOpen()) {
        m_derniereErreur = "La base de données n'est pas ouverte.";
        return resultat;
    }

    const QString filtreDate = conditionPeriodeSql(periode);

    QSqlQuery query(db);
    query.prepare(QString(
        "SELECT c.idcat, c.nomcat, c.couleur, COALESCE(SUM(o.montant), 0.0) AS total "
        "FROM categorie c "
        "LEFT JOIN operation o ON c.idcat = o.idcat "
        "  AND o.typeope = 'depense' "
        "  AND %1 "
        "GROUP BY c.idcat, c.nomcat, c.couleur "
        "HAVING total > 0 "
        "ORDER BY total DESC"
    ).arg(filtreDate));

    if (!query.exec()) {
        m_derniereErreur = query.lastError().text();
        return resultat;
    }

    while (query.next()) {
        DepenseCategorie item;
        item.idCategorie = query.value(0).toInt();
        item.nomCategorie = query.value(1).toString();
        item.couleur = query.value(2).toString();
        item.totalDepense = query.value(3).toDouble();
        resultat.append(item);
    }

    return resultat;
}

QString BudgetService::derniereErreur() const
{
    return m_derniereErreur;
}
