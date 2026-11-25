#include "mainwindow.h"
#include "databasemanager.h"


#include <QApplication>
#include <QDebug>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DatabaseManager gestionnaire;
    QString chemin = QCoreApplication::applicationDirPath() + "/cashflood.db";

    if (!gestionnaire.ouvrir(chemin)) {
        qDebug() << "Erreur ouverture :" << gestionnaire.derniereErreur();
        return 1;
    }

    MainWindow w;
    w.show();
    return a.exec();
}
