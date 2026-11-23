#include "mainwindow.h"
#include "databasemanager.h"
#include "comptedao.h"
#include "compte.h"
#include "operationdao.h"
#include "operation.h"
#include "budgetdao.h"
#include "categoriedao.h"
#include "categorie.h"

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

    CategorieDao catDao;
    Categorie cat(0, "Cat test budget", "#00AAFF");
    catDao.inserer(cat);

    BudgetDao budDao;
    Budget bud(0, "Budget test", 50000, "mensuel", QList<int>() << cat.id());
    if (!budDao.inserer(bud)) {
        qDebug() << "Erreur budget :" << budDao.derniereErreur();
    } else {
        qDebug() << "Budget insere, id =" << bud.id();

        bud.setPlafond(60000);
        budDao.modifier(bud);

        Budget trouve;
        if (budDao.trouver(bud.id(), trouve))
            qDebug() << "Trouve :" << trouve.nom() << trouve.plafond()
                     << "categories :" << trouve.categories().size();

        budDao.supprimer(bud.id());
    }
    catDao.supprimer(cat.id());

    MainWindow w;
    w.show();
    return a.exec();
}
