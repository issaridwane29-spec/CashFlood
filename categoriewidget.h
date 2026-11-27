#ifndef CATEGORIEWIDGET_H
#define CATEGORIEWIDGET_H

#include <QWidget>
#include <QString>
#include "categorie.h"
#include "categoriedao.h"

namespace Ui {
class CategorieWidget;
}

class CategorieWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CategorieWidget(QWidget *parent = nullptr);
    ~CategorieWidget();

    void setCategorieDao(CategorieDao *categorieDao);

signals:
    void categoriesModifiees();

public slots:
    void rafraichirCategories();

private slots:
    void onAjouterCategorie();
    void onModifierCategorie();
    void onSupprimerCategorie();
    void onChoisirCouleur();
    void onReinitialiser();
    void onSelectionChanged();

private:
    int idCategorieSelectionnee(int *ligneCourante = nullptr) const;
    void mettreAJourApercuCouleur();

    Ui::CategorieWidget *ui;
    CategorieDao *m_categorieDao = nullptr;
    QString m_couleurSelectionnee = "#3498db";
};

#endif // CATEGORIEWIDGET_H
