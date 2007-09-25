/***************************************************************************
 *                                                                         *
 *   copyright : (C) 2007 The University of Toronto                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "vectordialog.h"

#include "dialogpage.h"

#include "kstsvector.h"

#include <QDir>

namespace Kst {

VectorTab::VectorTab(QWidget *parent)
  : DialogTab(parent), _mode(ReadOnlyVector) {

  setupUi(this);
  setTabTitle(tr("Vector"));

  connect(_readFromSource, SIGNAL(toggled(bool)),
          this, SLOT(readFromSourceChanged()));
  connect(_fileName, SIGNAL(changed(const QString &)),
          this, SLOT(fileNameChanged(const QString &)));

  _fileName->setFile(QDir::currentPath());
}


VectorTab::~VectorTab() {
}


qreal VectorTab::from() const {
  return _from->text().toDouble();
}


void VectorTab::setFrom(qreal from) {
  _from->setText(QString::number(from));
}


qreal VectorTab::to() const {
  return _to->text().toDouble();
}


void VectorTab::setTo(qreal to) {
  _to->setText(QString::number(to));
}


int VectorTab::numberOfSamples() const {
  return _numberOfSamples->value();
}


void VectorTab::readFromSourceChanged() {

  if (_readFromSource->isChecked())
    setMode(ReadOnlyVector);
  else
    setMode(SlaveVector);

  _rvectorGroup->setEnabled(_readFromSource->isChecked());
  _dataRange->setEnabled(_readFromSource->isChecked());
  _svectorGroup->setEnabled(!_readFromSource->isChecked());
}


void VectorTab::fileNameChanged(const QString &file) {
  //FIXME deep magic...
  QFileInfo info(file);
  if (info.exists() && info.isFile())
    qDebug() << "fileNameChanged" << endl;
}


VectorDialog::VectorDialog(QWidget *parent)
  : DataDialog(parent) {

  setWindowTitle(tr("New Vector"));

  _vectorTab = new VectorTab(this);
  addDataTab(_vectorTab);
}


VectorDialog::VectorDialog(KstObjectPtr dataObject, QWidget *parent)
  : DataDialog(dataObject, parent) {

  setWindowTitle(tr("Edit Vector"));

  _vectorTab = new VectorTab(this);
  addDataTab(_vectorTab);
}


VectorDialog::~VectorDialog() {
}


void VectorDialog::setDefaults() {
  //FIXME
}


KstObjectPtr VectorDialog::createNewDataObject() const {

  if (_vectorTab->mode() == VectorTab::ReadOnlyVector) {

    qDebug() << "ReadOnlyVectors not supported yet...!" << endl;
    return 0;

  } else if (_vectorTab->mode() == VectorTab::SlaveVector) {

    const qreal from = _vectorTab->from();
    const qreal to = _vectorTab->to();
    const int numberOfSamples = _vectorTab->numberOfSamples();
    const KstObjectTag tag = KstObjectTag(tagName(), KstObjectTag::globalTagContext);

    KstSVectorPtr vector = new KstSVector(from, to, numberOfSamples, tag);
    return static_cast<KstObjectPtr>(vector);

  }

  return 0;
}


KstObjectPtr VectorDialog::editExistingDataObject() const {
  qDebug() << "editExistingDataObject" << endl;
  return 0;
}

}

// vim: ts=2 sw=2 et
