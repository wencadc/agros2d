#ifndef HERMES_ELASTICITY_H
#define HERMES_ELASTICITY_H

#include "util.h"
#include "hermes_field.h"
#include "hermes2d.h"
#include "solverdialog.h"
#include "solver_umfpack.h"

struct HermesElasticity : public HermesField
{
    Q_OBJECT
public:
    HermesElasticity() { physicField = PHYSICFIELD_ELASTICITY; }

    inline int numberOfSolution() { return 2; }
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

    inline PhysicFieldVariable contourPhysicFieldVariable() { return PHYSICFIELDVARIABLE_ELASTICITY_VON_MISES_STRESS; }
    inline PhysicFieldVariable scalarPhysicFieldVariable() { return PHYSICFIELDVARIABLE_ELASTICITY_VON_MISES_STRESS; }
    inline PhysicFieldVariableComp scalarPhysicFieldVariableComp() { return PHYSICFIELDVARIABLECOMP_SCALAR; }
    inline PhysicFieldVariable vectorPhysicFieldVariable() { return PHYSICFIELDVARIABLE_UNDEFINED; }

    void fillComboBoxScalarVariable(QComboBox *cmbFieldVariable)
    {
        cmbFieldVariable->addItem(physicFieldVariableString(PHYSICFIELDVARIABLE_ELASTICITY_VON_MISES_STRESS), PHYSICFIELDVARIABLE_ELASTICITY_VON_MISES_STRESS);
    }

    void fillComboBoxVectorVariable(QComboBox *cmbFieldVariable)
    {
        cmbFieldVariable->addItem(physicFieldVariableString(PHYSICFIELDVARIABLE_HEAT_TEMPERATURE_GRADIENT), PHYSICFIELDVARIABLE_HEAT_TEMPERATURE_GRADIENT);
    }

    void showLocalValue(QTreeWidget *trvWidget, LocalPointValue *localPointValue);
    void showSurfaceIntegralValue(QTreeWidget *trvWidget, SurfaceIntegralValue *surfaceIntegralValue);
    void showVolumeIntegralValue(QTreeWidget *trvWidget, VolumeIntegralValue *volumeIntegralValue);
};

class LocalPointValueElasticity : public LocalPointValue
{
public:
    double young_modulus;
    double poisson_ratio;
    double von_mises_stress;

    LocalPointValueElasticity(Point &point);
    double variableValue(PhysicFieldVariable physicFieldVariable, PhysicFieldVariableComp physicFieldVariableComp);
    QStringList variables();
};

class SceneEdgeElasticityMarker : public SceneEdgeMarker
{
public:
    PhysicFieldBC typeX;
    PhysicFieldBC typeY;
    double forceX;
    double forceY;

    QString script();
    QMap<QString, QString> data();
    SceneEdgeElasticityMarker(const QString &name, PhysicFieldBC typeX, PhysicFieldBC typeY, double forceX, double forceY);

    int showDialog(QWidget *parent);
};

class SceneLabelElasticityMarker : public SceneLabelMarker
{
public:
    double young_modulus;
    double poisson_ratio;

    SceneLabelElasticityMarker(const QString &name, double young_modulus, double poisson_ratio);

    // Lame constant
    inline double lambda() { return (young_modulus * poisson_ratio) / ((1 + poisson_ratio) * (1 - 2*poisson_ratio)); }
    inline double mu() { return young_modulus / (2*(1 + poisson_ratio)); }

    QString script();
    QMap<QString, QString> data();
    int showDialog(QWidget *parent);
};

class DSceneEdgeElasticityMarker : public DSceneEdgeMarker
{
    Q_OBJECT

public:
    DSceneEdgeElasticityMarker(SceneEdgeElasticityMarker *edgeEdgeElasticityMarker, QWidget *parent);
    ~DSceneEdgeElasticityMarker();

protected:
    QLayout *createContent();

    void load();
    bool save();

private:
    QComboBox *cmbTypeX;
    QComboBox *cmbTypeY;
    SLineEdit *txtForceX;
    SLineEdit *txtForceY;
};

class DSceneLabelElasticityMarker : public DSceneLabelMarker
{
    Q_OBJECT

public:
    DSceneLabelElasticityMarker(QWidget *parent, SceneLabelElasticityMarker *labelElasticityMarker);
    ~DSceneLabelElasticityMarker();

protected:
    QLayout *createContent();

    void load();
    bool save();

private:
    SLineEdit *txtYoungModulus;
    SLineEdit *txtPoissonNumber;
};

#endif // HERMES_ELASTICITY_H
