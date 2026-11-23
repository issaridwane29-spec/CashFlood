#include "mainwindow.h"
#include "databasemanager.h"
#include "comptedao.h"
#include "compte.h"
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

    QSqlQuery q("SELECT name FROM sqlite_master WHERE type = 'table'");
    while (q.next())
        qDebug() << "Table :" << q.value(0).toString();

    CategorieDao catDao;
    Categorie cat(0, "Alimentation", "#FF5733");
    if (!catDao.inserer(cat)) {
        qDebug() << "Erreur categorie :" << catDao.derniereErreur();
    } else {
        qDebug() << "Categorie inseree, id =" << cat.id();
        cat.setNom("Alimentation modifiee");
        catDao.modifier(cat);

        Categorie trouvee;
        if (catDao.trouver(cat.id(), trouvee))
            qDebug() << "Trouvee :" << trouvee.nom() << trouvee.couleur();

        catDao.supprimer(cat.id());
    }
    qDebug() << "Categories restantes :" << catDao.lister().size();

    MainWindow w;
    w.show();
    return a.exec();

}






