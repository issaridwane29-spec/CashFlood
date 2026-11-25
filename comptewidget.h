#ifndef COMPTEWIDGET_H
#define COMPTEWIDGET_H

#include <QWidget>
#include "compte.h"
#include "comptedao.h"
#include "compteservice.h"

namespace Ui {
class CompteWidget;
}

class CompteWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CompteWidget(QWidget *parent = nullptr);
    ~CompteWidget();

    void setCompteDao(CompteDao *compteDao);

public slots:
    void rafraichirComptes();

private slots:
    void onNouveauCompte();
    void onModifierCompte();
    void onSupprimerCompte();
    void onMonterCompte();
    void onDescendreCompte();

private:
    int idCompteSelectionne(int *ligneCourante = nullptr) const;

    Ui::CompteWidget *ui;

    CompteDao *m_compteDao = nullptr;
    CompteService m_compteService;
};

#endif // COMPTEWIDGET_H
