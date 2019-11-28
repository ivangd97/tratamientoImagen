/***************************************************************************
 *   Copyright (C) 2011 by pilar   *
 *   pilarb@unex.es   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#ifndef PRACAOC_H
#define PRACAOC_H

#include "ui_mainForm.h"
#include <QtCore>
#include <QPainter>
#include <QFileDialog>
#include <QMouseEvent>
#include <iostream>
#include "imageprocess.h"

using namespace Ui;


class pracAOC:public QWidget
{
    Q_OBJECT

public:
    pracAOC();
    ~pracAOC();
		
		

private:

		QMainForm * mainWin;
		QImage * qimgOrig, * qimgDest;
		uchar * imgO, * imgD;
		int mascara[9];

		QVector<QRgb> ctable;

		bool seleccion;
		int xIni, yIni, xFin, yFin;

protected:

		void mousePressEvent(QMouseEvent *event);
		void mouseReleaseEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);
		void closeEvent(QCloseEvent *event);

public slots:

		void paintEvent(QPaintEvent *);
		void cargar();
		void guardar();
		void copiarOrigen();
		void copiarDestino();
		void negar();
        void recortar();
		void voltearHorizontal();
		void voltearVertical();
		void girarIzquierda();
		void girarDerecha();
        void umbralizar();

};




#endif
