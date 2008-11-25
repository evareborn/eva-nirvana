/***************************************************************************
 *   Copyright (C) 2004 by yunfan                                          *
 *   yunfan_zg@163.com                                                     *
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
#include "evatextedit.h"
#include <qevent.h>
//Added by qt3to4:
#include <QKeyEvent>

EvaTextEdit::EvaTextEdit(const QString &text, const QString &context, QWidget *parent, const char *name):
		Q3TextEdit(text, context, parent, name), isEnterSend(false)
{
	setAutoFormatting(Q3TextEdit::AutoNone);
	setWrapPolicy(Q3TextEdit::AtWordOrDocumentBoundary);
}

EvaTextEdit::EvaTextEdit(QWidget *parent, const char * name):
		Q3TextEdit(parent, name), isEnterSend(false)
{
	setAutoFormatting(Q3TextEdit::AutoNone);
	setWrapPolicy(Q3TextEdit::AtWordOrDocumentBoundary);
}

void EvaTextEdit::keyPressEvent(QKeyEvent *e)
{

    int para;
    int index;	
    getCursorPosition(&para,&index);
    if ( (e->key() == Qt::Key_Enter) || (e->key() == Qt::Key_Return) ) {
	if ( (e->state() & Qt::ControlButton)==Qt::ControlButton)
        {
    	    if ( !isEnterSend )
    	    {
        	emit keyPressed(e);
        	return;
    	    }
    	}
	else if ( (e->state() | Qt::KeyButtonMask) ) {
    	    if (isEnterSend )
    	    {
        	emit keyPressed(e);
        	return;
    	    }
	}
    }
    Q3TextEdit::keyPressEvent(e);
    if((e->key() == Qt::Key_Enter) || (e->key() == Qt::Key_Return) ){	
        QString txt = text();
        txt.replace("</p>\n<p>", "<br />");
        setText(txt);
        setCursorPosition(para, index + 1);
    }
    emit keyPressed(e);
}
