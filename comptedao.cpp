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
