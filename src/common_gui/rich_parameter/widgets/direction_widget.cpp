/*****************************************************************************
 * MeshLab                                                           o o     *
 * Visual and Computer Graphics Library                            o     o   *
 *                                                                _   O  _   *
 * Copyright(C) 2004-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 2 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 *                                                                           *
 ****************************************************************************/

#include "direction_widget.h"

#include <QApplication>
#include <QClipboard>
#include <QColorDialog>
#include <QFileDialog>
#include <common/ml_document/mesh_document.h>

DirectionWidget::DirectionWidget(
	QWidget*             p,
	const RichDirection& rpf,
	const RichDirection& rdef,
	QWidget*             gla_curr) :
		Point3Widget(p, rpf, rdef, gla_curr)
{
	// if we have a connection to the current glarea we can setup the additional
	// button for getting the current view direction.
	if (gla_curr) {
		QStringList names;
		names << "View Dir.";
		names << "Raster Camera Dir.";

		getPoint3Combo->addItems(names);

		connect(
			gla_curr,
			SIGNAL(transmitViewDir(QString, Point3m)),
			this,
			SLOT(setValue(QString, Point3m)));
		connect(
			gla_curr,
			SIGNAL(transmitShot(QString, Shotm)),
			this,
			SLOT(setShotValue(QString, Shotm)));
		connect(this, SIGNAL(askViewDir(QString)), gla_curr, SLOT(sendViewDir(QString)));
		connect(this, SIGNAL(askCameraDir(QString)), gla_curr, SLOT(sendRasterShot(QString)));

		connect(getPoint3Button, SIGNAL(clicked()), this, SLOT(getPoint()));
	}
}

DirectionWidget::~DirectionWidget()
{
	this->disconnect();
}

void DirectionWidget::getPoint()
{
	int index = getPoint3Combo->currentIndex();
	switch (index) {
	case 0: emit askViewDir(paramName); break;
	case 1: emit askCameraDir(paramName); break;

	default: assert(0);
	}
}
