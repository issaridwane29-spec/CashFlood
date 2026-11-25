#include "operationdao.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

OperationDao::OperationDao() {}

QString OperationDao::derniereErreur() const { return m_erreur; }

static const char *COLONNES =
    "idope, typeope, montant, dateope, description, idcmpt, idcat";

Operation OperationDao::depuisRequete(const QSqlQuery &q) const
{
    return Operation(q.value(0).toInt(),
                     q.value(1).toString(),
                     q.value(2).toDouble(),
                     QDate::fromString(q.value(3).toString(), Qt::ISODate),
                     q.value(4).toString(),
                     q.value(5).toInt(),
                     q.value(6).toInt());   // 0 si la categorie est NULL
}

bool OperationDao::inserer(Operation &op)
{
    QSqlQuery q;
    q.prepare("INSERT INTO operation (typeope, montant, dateope, description, idcmpt, idcat) "
              "VALUES (:type, :montant, :date, :desc, :cmpt, :cat)");
    q.bindValue(":type", op.type());
    q.bindValue(":montant", op.montant());
    q.bindValue(":date", op.date().toString(Qt::ISODate));
    q.bindValue(":desc", op.description().isNull() ? QString("") : op.description());
    q.bindValue(":cmpt", op.idCompte());
    // categorie optionnelle : NULL si aucune
    q.bindValue(":cat", op.idCategorie() > 0 ? QVariant(op.idCategorie()) : QVariant());

    if (!q.exec()) {
        m_erreur = q.lastError().text();
        return false;
    }
    op.setId(q.lastInsertId().toInt());
    return true;
}

bool OperationDao::modifier(const Operation &op)
{
    QSqlQuery q;
    q.prepare("UPDATE operation SET typeope = :type, montant = :montant, "
              "dateope = :date, description = :desc, idcmpt = :cmpt, idcat = :cat "
              "WHERE idope = :id");
    q.bindValue(":type", op.type());
    q.bindValue(":montant", op.montant());
    q.bindValue(":date", op.date().toString(Qt::ISODate));
    q.bindValue(":desc", op.description().isNull() ? QString("") : op.description());
    q.bindValue(":cmpt", op.idCompte());
    q.bindValue(":cat", op.idCategorie() > 0 ? QVariant(op.idCategorie()) : QVariant());
    q.bindValue(":id", op.id());

    if (!q.exec()) {
        m_erreur = q.lastError().text();
        return false;
    }
    return true;
}

bool OperationDao::supprimer(int id)
{
    QSqlQuery q;
    q.prepare("DELETE FROM operation WHERE idope = :id");
    q.bindValue(":id", id);

    if (!q.exec()) {
        m_erreur = q.lastError().text();
        return false;
    }
    return true;
}

QList<Operation> OperationDao::lister()
{
    QList<Operation> liste;
    QSqlQuery q(QString("SELECT %1 FROM operation ORDER BY dateope DESC, idope DESC")
                    .arg(COLONNES));
    while (q.next())
        liste.append(depuisRequete(q));
    return liste;
}

QList<Operation> OperationDao::listerParCompte(int idCompte)
{
    QList<Operation> liste;
    QSqlQuery q;
    q.prepare(QString("SELECT %1 FROM operation WHERE idcmpt = :id "
                      "ORDER BY dateope DESC, idope DESC").arg(COLONNES));
    q.bindValue(":id", idCompte);

    if (!q.exec()) {
        m_erreur = q.lastError().text();
        return liste;
    }
    while (q.next())
        liste.append(depuisRequete(q));
    return liste;
}

bool OperationDao::trouver(int id, Operation &resultat)
{
    QSqlQuery q;
    q.prepare(QString("SELECT %1 FROM operation WHERE idope = :id").arg(COLONNES));
    q.bindValue(":id", id);

    if (!q.exec()) {
        m_erreur = q.lastError().text();
        return false;
    }
    if (!q.next()) {
        m_erreur = "Operation introuvable";
        return false;
    }
    resultat = depuisRequete(q);
    return true;
}
