#include "databasemanager.h"

#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlError>
#include<QFile>

DatabaseManager::DatabaseManager()
{
}

QString DatabaseManager::derniereErreur() const
{
    return m_erreur;

}

bool DatabaseManager::executer(const QString &sql){
    QSqlQuery requete;
    if (!requete.exec(sql)){
        m_erreur = requete.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::ouvrir(const QString &chemin){
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(chemin);

    if (!db.open()) {
        m_erreur = db.lastError().text();
        return false;
    }

    if (!executer("PRAGMA foreign_keys = ON")) {
        return false;
    }
    return creerTables();
}

bool DatabaseManager::creerTables(){
    QFile fichier(":/schema.sql");
    if(!fichier.open(QIODevice::ReadOnly | QIODevice::Text)){
        m_erreur = "Impossible de lire schema.sql";
        return false;
    }

    QString contenu =QString::fromUtf8(fichier.readAll());
    const QStringList instructions = contenu.split(';', QString::SkipEmptyParts);

        for (const QString &instruction : instructions) {
            if (instruction.trimmed().isEmpty())
                continue;
            if (!executer(instruction))
                return false;
        }
        return true;
    }

    bool DatabaseManager::reinitialiser(){
        const QStringList tables = {"budget_categorie", "budget", "operation", "categorie", "compte"};
        for (const QString &table : tables) {
            if (!executer("DROP TABLE IF EXISTS " + table))
                 return false;
            }
        return creerTables();
    }
