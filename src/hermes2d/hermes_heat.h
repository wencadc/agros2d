#ifndef HEAT_H
#define HEAT_H

#include "util.h"
#include "hermes_field.h"
#include "hermes2d.h"
#include "solverdialog.h"
#include "solver_umfpack.h"

struct HermesHeat : public HermesField
{
    Q_OBJECT
public:
    HermesHeat() { physicField = PHYSICFIELD_HEAT_TRANSFER; }

    inline int numberOfSolution() { return 1; }
    bool hasFrequency() { return false; }

    void readEdgeMarkerFromDomElement(QDomElement *element);
    void writeEdgeMarkerToDomElement(QDomElement *element, SceneEdgeMarker *marker);
    void readLabelMarkerFromDomElement(QDomElement *element);
    void writeLabelMarkerToDomElement(QDomElement *element, SceneLabelMarker *marker);

    LocalPointValue *localPointValue(Point point);
    QStringList localPointValueHeader();

    SurfaceIntegralValue *surfaceIntegralValue();
    QStringList surfaceIntegralValueHeader();

    VolumeIntegralValue *volumeIntegralValue();
    QStringList volumeIntegralValueHeader();

    SceneEdgeMarker *newEdgeMarker();
    SceneLabelMarker *newLabelMarker();

    SolutionArray *solve(SolverThread *solverThread);

    inline PhysicFieldVariable contourPhysicFieldVariable() { return PHYSICFIELDVARIABLE_HEAT_TEMPERATURE; }
    inline PhysicFieldVariable scalarPhysicFieldVariable() { return PHYSICFIELDVARIABLE_HEAT_TEMPERATURE; }
    inline PhysicFieldVariableComp scalarPhysicFieldVariableComp() { return PHYSICFIELDVARIABLECOMP_SCALAR; }
    inline PhysicFieldVariable vectorPhysicFieldVariable() { return PHYSICFIELDVARIABLE_HEAT_FLUX; }

    void fillComboBoxScalarVariable(QComboBox *cmbFieldVariable)
    {
        cmbFieldVariable->addItem(physicFieldVariableString(PHYSICFIELDVARIABLE_HEAT_TEMPERATURE), PHYSICFIELDVARIABLE_HEAT_TEMPERATURE);
        cmbFieldVariable->addItem(physicFieldVariableString(PHYSICFIELDVARIABLE_HEAT_TEMPERATURE_GRADIENT), PHYSICFIELDVARIABLE_HEAT_TEMPERATURE_GRADIENT);
        cmbFieldVariable->addItem(physicFieldVariableString(PHYSICFIELDVARIABLE_HEAT_FLUX), PHYSICFIELDVARIABLE_HEAT_FLUX);
        cmbFieldVariable->addItem(physicFieldVariableString(PHYSICFIELDVARIABLE_HEAT_CONDUCTIVITY), PHYSICFIELDVARIABLE_HEAT_CONDUCTIVITY);
    }

    void fillComboBoxVectorVariable(QComboBox *cmbFieldVariable)
    {
        cmbFieldVariable->addItem(physicFieldVariableString(PHYSICFIELDVARIABLE_HEAT_TEMPERATURE_GRADIENT), PHYSICFIELDVARIABLE_HEAT_TEMPERATURE_GRADIENT);
        cmbFieldVariable->addItem(physicFieldVariableString(PHYSICFIELDVARIABLE_HEAT_FLUX), PHYSICFIELDVARIABLE_HEAT_FLUX);
    }

    void showLocalValue(QTreeWidget *trvWidget, LocalPointValue *localPointValue);
    void showSurfaceIntegralValue(QTreeWidget *trvWidget, SurfaceIntegralValue *surfaceIntegralValue);
    void showVolumeIntegralValue(QTreeWidget *trvWidget, VolumeIntegralValue *volumeIntegralValue);
};

class LocalPointValueHeat : public LocalPointValue
{
public:
    double volume_heat;
    double thermal_conductivity;
    double temperature;
    Point F;
    Point G;

    LocalPointValueHeat(Point &point);
    double variableValue(PhysicFieldVariable physicFieldVariable, PhysicFieldVariableComp physicFieldVariableComp);
    QStringList variables();
};

class SurfaceIntegralValueHeat : public SurfaceIntegralValue
{
public:
    double averageTemperature;
    double temperatureDifference;
    double heatFlux;

    SurfaceIntegralValueHeat();
    QStringList variables();
};

class VolumeIntegralValueHeat : public VolumeIntegralValue
{
public:
    double averageTemperature;
    double averageTemperatureGradientX;
    double averageTemperatureGradientY;
    double averageTemperatureGradient;
    double averageHeatFluxX;
    double averageHeatFluxY;
    double averageHeatFlux;

    VolumeIntegralValueHeat();
    QStringList variables();
};

class SceneEdgeHeatMarker : public SceneEdgeMarker
{
public:
    Value temperature;
    Value heatFlux;
    Value h;
    Value externalTemperature;

    SceneEdgeHeatMarker(const QString &name, PhysicFieldBC type, Value temperature);
    SceneEdgeHeatMarker(const QString &name, PhysicFieldBC type, Value heatFlux, Value h, Value externalTemperature);

    QString script();
    QMap<QString, QString> data();
    int showDialog(QWidget *parent);
};

class SceneLabelHeatMarker : public SceneLabelMarker
{
public:
    Value thermal_conductivity;
    Value volume_heat;

    SceneLabelHeatMarker(const QString &name, Value volume_heat, Value thermal_conductivity);

    QString script();
    QMap<QString, QString> data();
    int showDialog(QWidget *parent);
};

class DSceneEdgeHeatMarker : public DSceneEdgeMarker
{
    Q_OBJECT

public:
    DSceneEdgeHeatMarker(SceneEdgeHeatMarker *edgeEdgeHeatMarker, QWidget *parent);
    ~DSceneEdgeHeatMarker();

protected:
    QLayout *createContent();

    void load();
    bool save();

private:
    QComboBox *cmbType;
    SLineEditValue *txtTemperature;
    SLineEditValue *txtHeatFlux;
    SLineEditValue *txtHeatTransferCoefficient;
    SLineEditValue *txtExternalTemperature;

private slots:
    void doTypeChanged(int index);
};

class DSceneLabelHeatMarker : public DSceneLabelMarker
{
    Q_OBJECT

public:
    DSceneLabelHeatMarker(QWidget *parent, SceneLabelHeatMarker *labelHeatMarker);
    ~DSceneLabelHeatMarker();

protected:
    QLayout *createContent();

    void load();
    bool save();

private:
    SLineEditValue *txtThermalConductivity;
    SLineEditValue *txtVolumeHeat;
};

#endif // HEAT_H
