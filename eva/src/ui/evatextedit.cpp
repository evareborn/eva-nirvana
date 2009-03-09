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

EvaTextEdit::EvaTextEdit(const QString &text, const QString &context, QWidget *parent, const char *name):
		KTextEdit(text, context, parent, name), isEnterSend(false)
{
	setAutoFormatting(QTextEdit::AutoNone);
	setWrapPolicy(QTextEdit::AtWordOrDocumentBoundary);
}

EvaTextEdit::EvaTextEdit(QWidget *parent, const char * name):
		KTextEdit(parent, name), isEnterSend(false)
{
	setAutoFormatting(QTextEdit::AutoNone);
	setWrapPolicy(QTextEdit::AtWordOrDocumentBoundary);
}

void EvaTextEdit::keyPressEvent(QKeyEvent *e)
{
	int para;
	int index;	
	getCursorPosition(&para,&index);
	if( (isEnterSend && (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return)  && (e->state() != Qt::KeyButtonMask)) || 
			( (!isEnterSend)  &&
			( (e->key() == Qt::Key_Enter) || (e->key() == Qt::Key_Return) ) && 
			( (e->state() & Qt::ControlButton)==Qt::ControlButton))  ){
		emit keyPressed(e);
		return;
	}
	KTextEdit::keyPressEvent(e);
	if((e->key() == Qt::Key_Enter) || (e->key() == Qt::Key_Return) ){
		QString txt = text();
		txt.replace("</p>\n<p>", "<br />");
		setText(txt);
		puts(txt.ascii());
		setCursorPosition(para, index + 1);
	}
	emit keyPressed(e);
}
