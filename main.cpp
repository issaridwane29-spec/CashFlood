#include "mainwindow.h"
#include "databasemanager.h"
#include "comptedao.h"
#include "compte.h"
#include "operationdao.h"
#include "operation.h"

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

    CompteDao compteDao;
    Compte cpt(0, "Compte test op", "courant", 10000, 9);
    if (!compteDao.inserer(cpt))
        qDebug() << "Erreur compte :" << compteDao.derniereErreur();

    OperationDao opDao;
    Operation op(0, "depense", 2500, QDate::currentDate(), "Courses", cpt.id(), 0);
    if (!opDao.inserer(op)) {
        qDebug() << "Erreur operation :" << opDao.derniereErreur();
    } else {
        qDebug() << "Operation inseree, id =" << op.id();

        op.setMontant(3000);
        opDao.modifier(op);

        Operation trouvee;
        if (opDao.trouver(op.id(), trouvee))
            qDebug() << "Trouvee :" << trouvee.type() << trouvee.montant() << trouvee.date();

        qDebug() << "Operations du compte :" << opDao.listerParCompte(cpt.id()).size();
        opDao.supprimer(op.id());
    }
    compteDao.supprimer(cpt.id());

    MainWindow w;
    w.show();
    return a.exec();
}
