#include "mainwindow.h"
#include "databasemanager.h"
#include "comptedao.h"
#include "compte.h"

#include <QApplication>
#include <QDebug>
#include <QSqlQuery>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DatabaseManager gestionnaire;
    QString chemin = QCoreApplication::applicationDirPath() + "/cashflood.db";

    if (!gestionnaire.ouvrir(chemin)) {
        qDebug() << "Erreur ouverture :" << gestionnaire.derniereErreur();
        return 1;
    }
    qDebug() << "Base ouverte :" << chemin;

    QSqlQuery q("SELECT name FROM sqlite_master WHERE type = 'table'");
    while (q.next())
        qDebug() << "Table :" << q.value(0).toString();

    CompteDao dao;
    Compte c(0, "Compte courant", "courant", 50000, 1);
    if (dao.inserer(c))
        qDebug() << "Compte insere, id =" << c.id();
    else
        qDebug() << "Erreur :" << dao.derniereErreur();

    MainWindow w;
    w.show();
    return a.exec();

}






