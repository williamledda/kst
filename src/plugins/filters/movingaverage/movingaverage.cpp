/***************************************************************************
 *                                                                         *
 *   copyright : (C) 2007 The University of Toronto                        *
 *                   netterfield@astro.utoronto.ca                         *
 *   copyright : (C) 2005  University of British Columbia                  *
 *                   dscott@phas.ubc.ca                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "movingaverage.h"
#include "objectstore.h"
#include "ui_movingaverageconfig.h"

static const QString& VECTOR_IN   = "Vector In";
static const QString& WINDOW_SIZE = "Window size";
static const QString& VECTOR_OUT  = "Avg(Y)";

class ConfigMovingAveragePlugin : public Kst::DataObjectConfigWidget, public Ui_MovingAverageConfig {
  public:
    ConfigMovingAveragePlugin(QSettings* cfg) : DataObjectConfigWidget(cfg), Ui_MovingAverageConfig() {
      _store = 0;
      setupUi(this);
    }

    ~ConfigMovingAveragePlugin() {}

    void setObjectStore(Kst::ObjectStore* store) { 
      _store = store; 
      _vector->setObjectStore(store); 
    }

    void setupSlots(QWidget* dialog) {
      if (dialog) {
        connect(_vector, SIGNAL(selectionChanged(QString)), dialog, SIGNAL(modified()));
      }
    }

    void setVectorX(Kst::VectorPtr vector) {
      setSelectedVector(vector);
    }

    void setVectorY(Kst::VectorPtr vector) {
      setSelectedVector(vector);
    }

    void setVectorsLocked(bool locked = true) {
      _vector->setEnabled(!locked);
    }

    Kst::VectorPtr selectedVector() { return _vector->selectedVector(); };
    void setSelectedVector(Kst::VectorPtr vector) { return _vector->setSelectedVector(vector); };

    virtual void setupFromObject(Kst::Object* dataObject) {
      if (MovingAverageSource* source = static_cast<MovingAverageSource*>(dataObject)) {
        setSelectedVector(source->vector());
      }
    }

    virtual bool configurePropertiesFromXml(Kst::ObjectStore *store, QXmlStreamAttributes& attrs) {
      Q_UNUSED(store);
      Q_UNUSED(attrs);

      bool validTag = true;

//       QStringRef av;
//       av = attrs.value("value");
//       if (!av.isNull()) {
//         _configValue = QVariant(av.toString()).toBool();
//       }

      return validTag;
    }

  public slots:
    virtual void save() {
      if (_cfg) {
        _cfg->beginGroup("Moving Average DataObject Plugin");
        _cfg->setValue("Input Vector", _vector->selectedVector()->Name());
        _cfg->endGroup();
      }
    }

    virtual void load() {
      if (_cfg && _store) {
        _cfg->beginGroup("Moving Average DataObject Plugin");
        QString vectorName = _cfg->value("Input Vector").toString();
        Kst::Object* object = _store->retrieveObject(vectorName);
        Kst::Vector* vector = static_cast<Kst::Vector*>(object);
        if (vector) {
          setSelectedVector(vector);
        }
        _cfg->endGroup();
      }
    }

  private:
    Kst::ObjectStore *_store;

};


MovingAverageSource::MovingAverageSource(Kst::ObjectStore *store)
: Kst::BasicPlugin(store) {
}


MovingAverageSource::~MovingAverageSource() {
}


QString MovingAverageSource::_automaticDescriptiveName() const {
  if (vector()) {
    return tr("%1 Moving Average").arg(vector()->descriptiveName());
  } else {
    return tr("Moving Average");
  }
}

QString MovingAverageSource::descriptionTip() const {
  QString tip;

  tip = tr("Moving Average: %1\n").arg(Name());

  tip += tr("\nInput: %1").arg(vector()->descriptionTip());
  return tip;
}


void MovingAverageSource::change(Kst::DataObjectConfigWidget *configWidget) {
  if (ConfigMovingAveragePlugin* config = static_cast<ConfigMovingAveragePlugin*>(configWidget)) {
    setInputVector(VECTOR_IN, config->selectedVector());
  }
}


void MovingAverageSource::setupOutputs() {
  setOutputVector(VECTOR_OUT, "");
}


// TODO
bool MovingAverageSource::algorithm() {
  Kst::VectorPtr inputVector = _inputVectors[VECTOR_IN];
  Kst::VectorPtr outputVector;
  // maintain kst file compatibility if the output vector name is changed.
  if (_outputVectors.contains(VECTOR_OUT)) {
    outputVector = _outputVectors[VECTOR_OUT];
  } else {
    outputVector = _outputVectors.values().at(0);
  }


  /* Memory allocation */
  outputVector->resize(inputVector->length(), true);

  double const *v_in = inputVector->noNanValue();
  double *v_out = outputVector->raw_V_ptr();
  v_out[0] = v_in[0]; // i = 1

  for (int i = 1; i < inputVector->length(); ++i) {
    v_out[i] = (v_in[i] + (i * v_out[i-1])) / (i+1);
  }

  return true;
}


Kst::VectorPtr MovingAverageSource::vector() const {
  return _inputVectors[VECTOR_IN];
}


QStringList MovingAverageSource::inputVectorList() const {
  return QStringList( VECTOR_IN );
}


QStringList MovingAverageSource::inputScalarList() const {
  return QStringList( /*SCALAR_IN*/ );
}


QStringList MovingAverageSource::inputStringList() const {
  return QStringList( /*STRING_IN*/ );
}


QStringList MovingAverageSource::outputVectorList() const {
  return QStringList( VECTOR_OUT );
}


QStringList MovingAverageSource::outputScalarList() const {
  return QStringList( /*SCALAR_OUT*/ );
}


QStringList MovingAverageSource::outputStringList() const {
  return QStringList( /*STRING_OUT*/ );
}


void MovingAverageSource::saveProperties(QXmlStreamWriter &s) {
  Q_UNUSED(s);
//   s.writeAttribute("value", _configValue);
}


QString MovingAveragePlugin::pluginName() const { return tr("Moving Average"); }
QString MovingAveragePlugin::pluginDescription() const { return tr("Computes the Moving average of the input vector."); }


Kst::DataObject *MovingAveragePlugin::create(Kst::ObjectStore *store, Kst::DataObjectConfigWidget *configWidget, bool setupInputsOutputs) const {

  if (ConfigMovingAveragePlugin* config = static_cast<ConfigMovingAveragePlugin*>(configWidget)) {

    MovingAverageSource* object = store->createObject<MovingAverageSource>();

    if (setupInputsOutputs) {
      object->setupOutputs();
      object->setInputVector(VECTOR_IN, config->selectedVector());
    }

    object->setPluginName(pluginName());

    object->writeLock();
    object->registerChange();
    object->unlock();

    return object;
  }
  return 0;
}


Kst::DataObjectConfigWidget *MovingAveragePlugin::configWidget(QSettings *settingsObject) const {
  ConfigMovingAveragePlugin *widget = new ConfigMovingAveragePlugin(settingsObject);
  return widget;
}

#ifndef QT5
Q_EXPORT_PLUGIN2(kstplugin_BinPlugin, MovingAveragePlugin)
#endif

// vim: ts=2 sw=2 et
