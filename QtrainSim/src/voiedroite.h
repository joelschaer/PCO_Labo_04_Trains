#ifndef VOIEDROITE_H
#define VOIEDROITE_H

#include <voie.h>
#include <loco.h>

class VoieDroite : public Voie
{
    Q_OBJECT

public:
    VoieDroite(qreal longueur);
    void calculerAnglesEtCoordonnees(Voie *v=0);
    void calculerPositionContact();
    QList<QList<Voie*>*> explorationContactAContact(Voie* voieAppelante);
    qreal getLongueurAParcourir();
    Voie* getVoieSuivante(Voie* voieArrivee);
    void avanceLoco(qreal &dist, qreal &, qreal &, qreal, QPointF posActuelle, Voie *voieSuivante);
    void correctionPosition(qreal deltaX, qreal deltaY, Voie *v);
    void correctionPositionLoco(qreal &x, qreal &y);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    void setEtat(int);
private:
    qreal longueur;
    qreal lastDistDel;
};

#endif // VOIEDROITE_H
