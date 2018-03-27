#ifndef VOIETRAVERSEEJONCTION_H
#define VOIETRAVERSEEJONCTION_H

#include <voievariable.h>
#include <loco.h>
#include "trainsimsettings.h"

class VoieTraverseeJonction : public VoieVariable
{
    Q_OBJECT

public:
    VoieTraverseeJonction(qreal angle, qreal rayon, qreal longueur);
    void calculerAnglesEtCoordonnees(Voie *v);
    void calculerPositionContact();
    QList<QList<Voie*>*> explorationContactAContact(Voie* voieAppelante);
    qreal getLongueurAParcourir();
    Voie* getVoieSuivante(Voie* voieArrivee);
    void avanceLoco(qreal &dist, qreal &angle, qreal &rayon, qreal angleCumule, QPointF posActuelle, Voie *voieSuivante);
    void correctionPosition(qreal deltaX, qreal deltaY, Voie *v);
    void correctionPositionLoco(qreal &, qreal &);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    virtual void setNumVoieVariable(int numVoieVariable);

    virtual void mousePressEvent (QGraphicsSceneMouseEvent *);
private:
    qreal rayon03, rayon12, angle, longueur;
    QPointF* centre03;
    QPointF* centre12;
    qreal lastDistDel;
};

#endif // VOIETRAVERSEEJONCTION_H
