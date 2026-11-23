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

    Compte c(0, "Compte test", "courant", 20000, 2);
    if (!dao.inserer(c)) {
        qDebug() << "Erreur insertion :" << dao.derniereErreur();
    } else {
        qDebug() << "Insere, id =" << c.id();

        c.setNom("Compte modifie");
        dao.modifier(c);

        Compte trouve;
        if (dao.trouver(c.id(), trouve))
            qDebug() << "Trouve :" << trouve.id() << trouve.nom();

        dao.supprimer(c.id());
    }

    for (const Compte &x : dao.lister())
        qDebug() << "Liste :" << x.id() << x.nom() << x.soldeInitial();

    MainWindow w;
    w.show();
    return a.exec();

}






