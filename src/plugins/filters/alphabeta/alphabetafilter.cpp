/***************************************************************************
 *                                                                         *
 *   copyright : (C) 2019 William Ledda                                    *
 *                   villy80@hotmail.it                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "alphabetafilter.h"
#include "objectstore.h"
#include "ui_alphabetafilterconfig.h"

static const QString& VECTOR_IN = "Y Vector";
static const QString& SCALAR_ALPHA_IN = "Alpha Scalar";
static const QString& SCALAR_BETA_IN = "Beta Scalar";
static const QString& SCALAR_DELTA_IN = "Dt Scalar";
static const QString& VECTOR_OUT = "Y";

class ConfigWidgetAlphaBetaFilterPlugin : public Kst::DataObjectConfigWidget, public Ui_AlphaBetaFilterConfig {
  public:
    ConfigWidgetAlphaBetaFilterPlugin(QSettings* cfg) : DataObjectConfigWidget(cfg), Ui_AlphaBetaFilterConfig() {
      _store = nullptr;
      setupUi(this);
    }

    ~ConfigWidgetAlphaBetaFilterPlugin() {}

    void setObjectStore(Kst::ObjectStore* store) {
      _store = store;
      _vector->setObjectStore(store);
      _alphaScalar->setObjectStore(store);
      _betaScalar->setObjectStore(store);
      _deltaScalar->setObjectStore(store);
      _alphaScalar->setDefaultValue(0.0);
      _betaScalar->setDefaultValue(0.0);
      _deltaScalar->setDefaultValue(0.002);
    }

    void setupSlots(QWidget* dialog) {
      if (dialog) {
        connect(_vector, SIGNAL(selectionChanged(QString)), dialog, SIGNAL(modified()));
        connect(_alphaScalar, SIGNAL(selectionChanged(QString)), dialog, SIGNAL(modified()));
        connect(_betaScalar, SIGNAL(selectionChanged(QString)), dialog, SIGNAL(modified()));
        connect(_deltaScalar, SIGNAL(selectionChanged(QString)), dialog, SIGNAL(modified()));
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

    Kst::VectorPtr selectedVector() {
      return _vector->selectedVector();
    }

    void setSelectedVector(Kst::VectorPtr vector) {
      return _vector->setSelectedVector(vector);
    }

    Kst::ScalarPtr selectedAlphaScalar() {
      return _alphaScalar->selectedScalar();
    }

    void setSelectedAlphaScalar(Kst::ScalarPtr scalar) {
      return _alphaScalar->setSelectedScalar(scalar);
    }

    Kst::ScalarPtr selectedBetaScalar() {
      return _betaScalar->selectedScalar();
    }

    void setSelectedBetaScalar(Kst::ScalarPtr scalar) {
      return _betaScalar->setSelectedScalar(scalar);
    }

    Kst::ScalarPtr selectedDeltaScalar() {
      return _deltaScalar->selectedScalar();
    }

    void setSelectedDeltaScalar(Kst::ScalarPtr scalar) {
      return _deltaScalar->setSelectedScalar(scalar);
    }

    virtual void setupFromObject(Kst::Object* dataObject) {
      if (AlphaBetaFilterSource* source = static_cast<AlphaBetaFilterSource*>(dataObject)) {
        setSelectedVector(source->vector());
        setSelectedAlphaScalar(source->alphaScalar());
        setSelectedBetaScalar(source->betaScalar());
        setSelectedDeltaScalar(source->deltaScalar());
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
        _cfg->beginGroup("Alpha Beta Filter Plugin");
        _cfg->setValue("Input Vector", _vector->selectedVector()->Name());
        _cfg->setValue("Beta Scalar", _betaScalar->selectedScalar()->Name());
        _cfg->setValue("Alpha Scalar", _alphaScalar->selectedScalar()->Name());
        _cfg->setValue("Delta Scalar", _deltaScalar->selectedScalar()->Name());
        _cfg->endGroup();
      }
    }

    virtual void load() {
      if (_cfg && _store) {
        _cfg->beginGroup("Alpha Beta Filter Plugin");
        QString vectorName = _cfg->value("Input Vector").toString();
        Kst::Object* object = _store->retrieveObject(vectorName);
        Kst::Vector* vector = static_cast<Kst::Vector*>(object);
        if (vector) {
          setSelectedVector(vector);
        }
        QString scalarName = _cfg->value("Alpha Scalar").toString();
        _alphaScalar->setSelectedScalar(scalarName);

        scalarName = _cfg->value("Beta Scalar").toString();
        _betaScalar->setSelectedScalar(scalarName);

        scalarName = _cfg->value("Delta Scalar").toString();
        _deltaScalar->setSelectedScalar(scalarName);

        _cfg->endGroup();
      }
    }

  private:
    Kst::ObjectStore *_store;

};


AlphaBetaFilterSource::AlphaBetaFilterSource(Kst::ObjectStore *store)
: Kst::BasicPlugin(store) {
}


AlphaBetaFilterSource::~AlphaBetaFilterSource() {
}


QString AlphaBetaFilterSource::_automaticDescriptiveName() const {
  if (vector()) {
    return tr("%1 smooth", "arg 1 is the name of the vector which has been smooth").arg(vector()->descriptiveName());
  } else {
    return tr("Alpha Beta");
  }
}


QString AlphaBetaFilterSource::descriptionTip() const {
  QString tip;

  //TODO Fix this!!!!!!
  tip = tr("Alpha Beta Filter: %1\n  Alpha: %2\n  Beta: %3\n  Delta: %4").
          arg(Name()).
          arg(alphaScalar()->value()).
          arg(betaScalar()->value()).
          arg(deltaScalar()->value());

  tip += tr("\nInput: %1").arg(vector()->descriptionTip());
  return tip;
}

void AlphaBetaFilterSource::change(Kst::DataObjectConfigWidget *configWidget) {
  if (ConfigWidgetAlphaBetaFilterPlugin* config = static_cast<ConfigWidgetAlphaBetaFilterPlugin*>(configWidget)) {
    setInputVector(VECTOR_IN, config->selectedVector());
    setInputScalar(SCALAR_ALPHA_IN, config->selectedAlphaScalar());
    setInputScalar(SCALAR_BETA_IN, config->selectedBetaScalar());
    setInputScalar(SCALAR_DELTA_IN, config->selectedDeltaScalar());
  }
}


void AlphaBetaFilterSource::setupOutputs() {
  setOutputVector(VECTOR_OUT, "");
}


bool AlphaBetaFilterSource::algorithm() {
  Kst::VectorPtr inputVector = _inputVectors[VECTOR_IN];
//  Kst::ScalarPtr alphaScalar = _inputScalars[SCALAR_ALPHA_IN];
//  Kst::ScalarPtr betaScalar = _inputScalars[SCALAR_BETA_IN];
//  Kst::ScalarPtr deltaScalar = _inputScalars[SCALAR_DELTA_IN];
  Kst::VectorPtr outputVector;
  // maintain kst file compatibility if the output vector name is changed.
  if (_outputVectors.contains(VECTOR_OUT)) {
    outputVector = _outputVectors[VECTOR_OUT];
  } else {
    outputVector = _outputVectors.values().at(0);
  }

  double alpha = _inputScalars[SCALAR_ALPHA_IN]->value();
  double beta  = _inputScalars[SCALAR_BETA_IN]->value();
  double delta = _inputScalars[SCALAR_DELTA_IN]->value();

  outputVector->resize(inputVector->length(), true);
  double const *v_in  = inputVector->noNanValue();
  double *v_out = outputVector->raw_V_ptr();

  double estimate = 0.0;
  double velocity = 0.0;
  double residual = 0.0;

  v_out[0] = estimate;

  for(int i = 0; i < inputVector->length(); i++) {
      estimate += delta * velocity;
      residual = v_in[i] - estimate;
      estimate += (alpha * residual);
      velocity += beta * (residual / delta);
      v_out[i] = estimate;
  }

//  void update(double observation, double deltaTime)
//  {
//    estimate += deltaTime*velocity;
//    double residual = calcResidual(observation, deltaTime);
//    estimate = wrap(estimate + alpha*residual);
//    velocity += beta*(residual/deltaTime);
//  }
//  double wrap(double value) const
//  {
//    return rangeKeeper ? rangeKeeper->wrapValue(value) : value;
//  }

//  double calcResidual(double observation, double deltaTime) const
//  {
//    double delta = observation-estimate;
//    return rangeKeeper ? rangeKeeper->wrapDelta(delta, deltaTime) : delta;
//  }


  Kst::LabelInfo label_info = inputVector->labelInfo();
  label_info.name = tr("%1 Alpha: %2 Beta: %3 Delta: %4").arg(label_info.name)
          .arg(alpha)
          .arg(beta)
          .arg(delta);
  outputVector->setLabelInfo(label_info);

  return true;
}


Kst::VectorPtr AlphaBetaFilterSource::vector() const {
  return _inputVectors[VECTOR_IN];
}

Kst::ScalarPtr AlphaBetaFilterSource::betaScalar() const {
  return _inputScalars[SCALAR_BETA_IN];
}

Kst::ScalarPtr AlphaBetaFilterSource::alphaScalar() const {
  return _inputScalars[SCALAR_ALPHA_IN];
}

Kst::ScalarPtr AlphaBetaFilterSource::deltaScalar() const {
  return _inputScalars[SCALAR_DELTA_IN];
}


QStringList AlphaBetaFilterSource::inputVectorList() const {
  return QStringList( VECTOR_IN );
}


QStringList AlphaBetaFilterSource::inputScalarList() const {
  QStringList inputScalars( SCALAR_ALPHA_IN );
  inputScalars += SCALAR_BETA_IN;
  inputScalars += SCALAR_DELTA_IN;
  return inputScalars;
}


QStringList AlphaBetaFilterSource::inputStringList() const {
  return QStringList( /*STRING_IN*/ );
}


QStringList AlphaBetaFilterSource::outputVectorList() const {
  return QStringList( VECTOR_OUT );
}


QStringList AlphaBetaFilterSource::outputScalarList() const {
  return QStringList( /*SCALAR_OUT*/ );
}


QStringList AlphaBetaFilterSource::outputStringList() const {
  return QStringList( /*STRING_OUT*/ );
}


void AlphaBetaFilterSource::saveProperties(QXmlStreamWriter &s) {
  Q_UNUSED(s);
//   s.writeAttribute("value", _configValue);
}


// Name used to identify the plugin.  Used when loading the plugin.
QString AlphaBetaFilterPlugin::pluginName() const { return tr("Alpha Beta Filter"); }
QString AlphaBetaFilterPlugin::pluginDescription() const { return tr("Compute Alpha Beta filter of a given input vector"); }


Kst::DataObject *AlphaBetaFilterPlugin::create(Kst::ObjectStore *store, Kst::DataObjectConfigWidget *configWidget, bool setupInputsOutputs) const {

  if (ConfigWidgetAlphaBetaFilterPlugin* config = static_cast<ConfigWidgetAlphaBetaFilterPlugin*>(configWidget)) {

    AlphaBetaFilterSource* object = store->createObject<AlphaBetaFilterSource>();

    if (setupInputsOutputs) {
      object->setInputScalar(SCALAR_ALPHA_IN, config->selectedAlphaScalar());
      object->setInputScalar(SCALAR_BETA_IN,  config->selectedBetaScalar());
      object->setInputScalar(SCALAR_DELTA_IN, config->selectedDeltaScalar());
      object->setupOutputs();
      object->setInputVector(VECTOR_IN, config->selectedVector());
    }

    object->setPluginName(pluginName());

    object->writeLock();
    object->registerChange();
    object->unlock();

    return object;
  }
  return nullptr;
}


Kst::DataObjectConfigWidget *AlphaBetaFilterPlugin::configWidget(QSettings *settingsObject) const {
  ConfigWidgetAlphaBetaFilterPlugin *widget = new ConfigWidgetAlphaBetaFilterPlugin(settingsObject);
  return widget;
}

#ifndef QT5
Q_EXPORT_PLUGIN2(kstplugin_AlphaBetaFilterPlugin, AlphaBetaFilterPlugin)
#endif

// vim: ts=2 sw=2 et
