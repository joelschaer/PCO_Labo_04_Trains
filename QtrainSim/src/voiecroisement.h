#ifndef VOIECROISEMENT_H
#define VOIECROISEMENT_H

#include <voie.h>
#include <loco.h>

class VoieCroisement : public Voie
{
    Q_OBJECT

public:
    VoieCroisement(qreal angle, qreal longueur);
    void calculerAnglesEtCoordonnees(Voie *v);
    void calculerPositionContact();
    QList<QList<Voie*>*> explorationContactAContact(Voie* voieAppelante);
    qreal getLongueurAParcourir();
    Voie* getVoieSuivante(Voie* voieArrivee);
    void avanceLoco(qreal &dist, qreal &angle, qreal &rayon, qreal, QPointF posActuelle, Voie *voieSuivante);
    void correctionPosition(qreal deltaX, qreal deltaY, Voie *v);
    void correctionPositionLoco(qreal &, qreal &);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    void setEtat(int);
private:
    qreal angle, longueur;
    qreal lastDistDel;
};

#endif // VOIECROISEMENT_H
