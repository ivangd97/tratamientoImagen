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


#include "pracaoc.h"


pracAOC::pracAOC(): QWidget()
{
	mainWin=new QMainForm();
	mainWin->setupUi(this);

	ctable.resize(256);
	for(int i=0; i < 256; i++)
		ctable[i] = qRgb(i,i,i);

	imgO=new uchar[320*240];
	imgD=new uchar[320*240];

	qimgOrig = new QImage(imgO,320, 240, QImage::Format_Indexed8);
	qimgOrig->setColorTable(ctable);
	qimgOrig->setNumColors(256);

	qimgDest = new QImage(imgD,320, 240, QImage::Format_Indexed8);
	qimgDest->setColorTable(ctable);
	qimgOrig->setNumColors(256);


	for(int i=0; i<320*240;i++)
	{
		imgO[i]=0;
		imgD[i]=0;
	}

	seleccion=false;

	connect ( mainWin->pushButtonCargar, SIGNAL (clicked()), this, SLOT( cargar() ) );
	connect ( mainWin->pushButtonGuardar, SIGNAL (clicked()), this, SLOT( guardar() ) );
	connect ( mainWin->pushButtonCopiarO, SIGNAL (clicked()), this, SLOT( copiarOrigen() ) );
	connect ( mainWin->pushButtonCopiarD, SIGNAL (clicked()), this, SLOT( copiarDestino() ) );
    connect ( mainWin->pushButtonNegar, SIGNAL (clicked()), this, SLOT( negar() ) );
    connect ( mainWin->pushButtonRecortar, SIGNAL (clicked()), this, SLOT( recortar() ) );
	connect ( mainWin->pushButtonVoltearH, SIGNAL (clicked()), this, SLOT( voltearHorizontal() ) );
	connect ( mainWin->pushButtonVoltearV, SIGNAL (clicked()), this, SLOT( voltearVertical() ) );
	connect ( mainWin->pushButtonGirarI, SIGNAL (clicked()), this, SLOT( girarIzquierda() ) );
	connect ( mainWin->pushButtonGirarD, SIGNAL (clicked()), this, SLOT( girarDerecha() ) );
    connect ( mainWin->pushButtonUmbralizar, SIGNAL (clicked()), this, SLOT( umbralizar() ) );
	connect ( mainWin->pushButtonSalir, SIGNAL (clicked()), this, SLOT( close() ) );

}


pracAOC::~pracAOC()
{
}


void pracAOC::paintEvent(QPaintEvent *)
{
	QPainter painter(this);

	painter.drawImage(QPoint(mainWin->frameOrig->x(), mainWin->frameOrig->y()), *qimgOrig);
	painter.drawImage(QPoint(mainWin->frameDest->x(), mainWin->frameDest->y()), *qimgDest);

	painter.end();
}

void pracAOC::cargar()
{
	QImage qimgRead(320,240,QImage::Format_Indexed8);
	QString fn = QFileDialog::getOpenFileName(this,"Elija la imagen a cargar","/home/pilar/asignaturas/OC/curso08-09/prac2/pracoc/bin", "Images (*.png *.xpm *.jpg)");
	if(qimgRead.load(fn))
	{

        QImage qimgCRead(qimgRead.width(),qimgRead.height(),QImage::Format_Indexed8);
        qimgCRead=qimgRead.convertToFormat(QImage::Format_Indexed8, ctable);

        int width = ((qimgCRead.width()+3)/4)*4;

		for(int y=0; y<240 && y<qimgRead.height();y++)
			for(int x=0; x<320 && x<qimgRead.width(); x++)
                imgO[y*320+x]=(qimgCRead.bits())[(y*width+x)];

		update();
	}
}

void pracAOC::guardar()
{
	QImage qimgWrite(320,240,QImage::Format_Indexed8);
	QString fn = QFileDialog::getSaveFileName(this,"Elija la imagen a cargar","/home/pilar/asignaturas/OC/curso08-09/prac2/pracoc/bin", "Images (*.png *.xpm *.jpg)");
	qimgWrite.setColorTable(ctable);
	for(int y=0; y<240;y++)
		for(int x=0; x<320; x++)
			(qimgWrite.bits())[(y*320+x)]=imgD[y*320+x];

	qimgWrite.save(fn);
}


void pracAOC::copiarOrigen()
{
	imageprocess::copiar(imgO,imgD);	
	update();
}

void pracAOC::copiarDestino()
{
	imageprocess::copiar(imgD,imgO);	
	update();
}

void pracAOC::negar()
{
	imageprocess::negar(imgO,imgD);	
	update();
}

void pracAOC::recortar()
{
    if(mainWin->pushButtonRecortar->isChecked())
        mainWin->frameOrig->setCursor(Qt::CrossCursor);
    else{
        mainWin->frameOrig->setCursor(Qt::ArrowCursor);
        seleccion=false;
    }
}


void pracAOC::voltearHorizontal()
{
	imageprocess::voltearHorizontal(imgO,imgD);	
	update();
}

void pracAOC::voltearVertical()
{
	imageprocess::voltearVertical(imgO,imgD);	
	update();


}

void pracAOC::girarIzquierda()
{

//	imageprocess::borrar(imgD);
	imageprocess::girarIzquierda(imgO,imgD);	
	update();

}

void pracAOC::girarDerecha()
{
//	imageprocess::borrar(imgD);
	imageprocess::girarDerecha(imgO,imgD);	
	update();

}

void pracAOC::umbralizar()
{
    int histo[256];
    uchar umbral;

    imageprocess::calcularHistograma(imgO, histo);
    imageprocess::calcularUmbral(histo, umbral);
    imageprocess::umbralizar(imgO, umbral, imgD);
    update();
}


void pracAOC::mousePressEvent(QMouseEvent *event)
{

    if (event->button() == Qt::LeftButton
            && mainWin->frameOrig->geometry().contains(event->pos()) && mainWin->pushButtonRecortar->isChecked()) {
			
			seleccion=true;
			xFin=xIni=event->pos().x()-mainWin->frameOrig->geometry().x();
			yFin=yIni=event->pos().y()-mainWin->frameOrig->geometry().y();
			imgO[yIni*320+xIni]=255-imgO[yIni*320+xIni];
    }
}

void pracAOC::mouseMoveEvent(QMouseEvent *event)
{
	int xInter, yInter;
	int x, y;

    if (mainWin->frameOrig->geometry().contains(event->pos()) && mainWin->pushButtonRecortar->isChecked()) {
			if(seleccion){
				xInter=event->pos().x()-mainWin->frameOrig->geometry().x();
				yInter=event->pos().y()-mainWin->frameOrig->geometry().y();
				if(xInter!=xFin || yInter!=yFin)
				{
					if(xIni<xFin){
						for(x=xIni;x<xFin;x++){
							imgO[yIni*320+x]=255-imgO[yIni*320+x];
							imgO[yFin*320+x]=255-imgO[yFin*320+x];
						}
					}
					else{
						for(x=xIni;x>xFin;x--){
							imgO[yIni*320+x]=255-imgO[yIni*320+x];
							imgO[yFin*320+x]=255-imgO[yFin*320+x];
						}
					}
					if(yIni<yFin){
						for(y=yIni;y<yFin;y++){
							imgO[y*320+xIni]=255-imgO[y*320+xIni];
							imgO[y*320+xFin]=255-imgO[y*320+xFin];
						}

					}
					else{
						for(y=yIni;y>yFin;y--){
							imgO[y*320+xIni]=255-imgO[y*320+xIni];
							imgO[y*320+xFin]=255-imgO[y*320+xFin];
						}
					}
					if(xIni<xInter){
						for(x=xIni;x<xInter;x++){
							imgO[yIni*320+x]=255-imgO[yIni*320+x];
							imgO[yInter*320+x]=255-imgO[yInter*320+x];
						}
					}
					else{
						for(x=xIni;x>xInter;x--){
							imgO[yIni*320+x]=255-imgO[yIni*320+x];
							imgO[yInter*320+x]=255-imgO[yInter*320+x];
						}
					}
					if(yIni<yInter){
						for(y=yIni;y<yInter;y++){
							imgO[y*320+xIni]=255-imgO[y*320+xIni];
							imgO[y*320+xInter]=255-imgO[y*320+xInter];
						}
					}
					else{
						for(y=yIni;y>yInter;y--){
							imgO[y*320+xIni]=255-imgO[y*320+xIni];
							imgO[y*320+xInter]=255-imgO[y*320+xInter];
						}
					}
					xFin=xInter;
					yFin=yInter;
					update();


				}

			}
    }
}
void pracAOC::mouseReleaseEvent(QMouseEvent *event)
{
	int x, y;

	if(event->button()!=Qt::LeftButton) return;

    if (seleccion && mainWin->pushButtonRecortar->isChecked()) {
			
			seleccion=false;

			if(xIni<xFin){
				for(x=xIni;x<xFin;x++){
					imgO[yIni*320+x]=255-imgO[yIni*320+x];
					imgO[yFin*320+x]=255-imgO[yFin*320+x];
				}
			}
			else{
				for(x=xIni;x>xFin;x--){
					imgO[yIni*320+x]=255-imgO[yIni*320+x];
					imgO[yFin*320+x]=255-imgO[yFin*320+x];
				}
			}
			if(yIni<yFin){
				for(y=yIni;y<yFin;y++){
					imgO[y*320+xIni]=255-imgO[y*320+xIni];
					imgO[y*320+xFin]=255-imgO[y*320+xFin];
				}
			}
			else{
				for(y=yIni;y>yFin;y--){
					imgO[y*320+xIni]=255-imgO[y*320+xIni];
					imgO[y*320+xFin]=255-imgO[y*320+xFin];
				}
			}

			imgO[yIni*320+xIni]=255-imgO[yIni*320+xIni];

			if(xIni>xFin){
				x=xIni;
				xIni=xFin;
				xFin=x;
			}
	
			if(yIni>yFin){
				y=yIni;
				yIni=yFin;
				yFin=y;
			}
			mainWin->pushButtonRecortar->setChecked(false);
			mainWin->frameOrig->setCursor(Qt::ArrowCursor);
            //imageprocess::borrar(imgD);
			imageprocess::recortar(imgO,imgD, xIni, yIni, xFin-xIni+1, yFin-yIni+1);	
			update();
    }
}

void pracAOC::closeEvent(QCloseEvent *event)
{
	event->accept();
	delete mainWin;

}
