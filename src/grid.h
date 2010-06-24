#pragma once

#include "readConfig.h"

class oneDimGrid
{
public:
    oneDimGrid(configOptions& theOptions);

    dvector x; // The grid points
    dvector dampVal; // ratio of convective to diffusive coefficients (e.g. nu/v)

    int alpha; // domain curvature parameter. 0: planar, 1: cylindrical
    int ju, jb; // indices of burned / unburned boundaries
    int kQdot; // index into solutionState of the heat release rate

    // Parameters for controlling internal grid points:
    dvector vtol; // relative solution variable tolerance for point insertion
    dvector dvtol; // global derivative solution variable tolerance for point insertion
    double absvtol; // absolute tolerance (ignore components with range smaller than this)
    double rmTol; // relative grid point removal tolerance
    double uniformityTol; // maximum ratio of adjacent grid point separation distances
    double gridMin, gridMax; // minimum and maximum grid point separation
    double dampConst; // relative allowable numerical damping
    double centerGridMin; // minimum separation of center grid points for curved flames

    // Parameters for controlling exterior grid points:
    bool fixedBurnedVal;
    bool unburnedLeft;

    double boundaryTol;
    double boundaryTolRm;
    int addPointCount; // number of points to add when regridding

    // Derived mesh parameters (calculated by updateValues)
    dvector cfm, cf, cfp; // first centered difference
    dvector dlj;
    dvector hh;
    dvector rphalf;
    dvector r;
    int jj; // number of grid points

    // Update the grid based on the solutionState, and adjust it to fit
    // on the new grid. Each element of the solutionState is a vector
    // containing the value of a solution variable at each grid point.
    // Return value is true if the grid has been modified
    // adapt inserts and removes points within the problem domain
    // regrid inserts and removes points at the ends of the problem domain
    bool adapt(vector<dvector>& y, vector<dvector>& ydot);
    bool regrid(vector<dvector>& y, vector<dvector>& ydot);
    void updateValues(void);

    void updateBoundaryIndices(void);

    enum leftBoundaryConfigOptions {
        lbFixedVal,
        lbZeroGradNonCenter,
        lbControlVolume
    };

    int leftBoundaryConfig;

private:
    vector<dvector>* yIn;
    vector<dvector>* ydotIn;

    configOptions& options;
    int nVars; // number of solution variables at each grid point

    void removePoint(int jRemove, vector<dvector>& y, vector<dvector>& ydot);
    void addPoint(int jInsert, vector<dvector>& y, vector<dvector>& ydot);

    bool removeRight(void);
    bool removeLeft(void);
    bool addRight(void);
    bool addLeft(void);
};


class GridBased
{
public:
    GridBased();
    configOptions options;

    // the grid:
    oneDimGrid grid;

protected:
    // local names for some things that are part of the grid:
    dvector& x;
    dvector& r;
    dvector& rphalf;
    dvector& hh;
    dvector& dlj;
    dvector& cfm;
    dvector& cf;
    dvector& cfp;
};