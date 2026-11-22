#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include<QString>


class DatabaseManager
{
public:
    DatabaseManager();
    bool ouvrir(const QString &chemin);
    bool reinitialiser();
    QString derniereErreur() const;

private:
    bool creerTables();
    bool executer(const QString &sql);
    QString m_erreur;
};

#endif // DATABASEMANAGER_H
