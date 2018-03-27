#ifndef VOIEBUTTOIR_H
#define VOIEBUTTOIR_H

#include <voie.h>
#include <loco.h>

class VoieButtoir : public Voie
{
    Q_OBJECT

public:
    VoieButtoir(qreal longueur);
    void calculerAnglesEtCoordonnees(Voie *v);
    void calculerPositionContact();
    QList<QList<Voie*>*> explorationContactAContact(Voie *);
    qreal getLongueurAParcourir();
    Voie* getVoieSuivante(Voie*);
    void avanceLoco(qreal &dist, qreal &angle, qreal &rayon, qreal, QPointF posActuelle, Voie *voieSuivante);
    void correctionPosition(qreal deltaX, qreal deltaY, Voie *);
    void correctionPositionLoco(qreal &, qreal &);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    void setEtat(int);

private:
    qreal longueur;
};

#endif // VOIEBUTTOIR_H
