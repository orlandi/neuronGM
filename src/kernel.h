/*
 * Copyright (c) 2013-2023 Javier G. Orlandi <javier.orlandi@ucalgary.ca>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#pragma once 

#define EIGEN_YES_I_KNOW_SPARSE_MODULE_IS_NOT_STABLE_YET 

#include <iostream>
#include <ctime>
#include <eigen3/Eigen/Sparse>
#include "gsl/gsl_rng.h"
#include <sim/sim_main.h>

//#define STD_DATA_COUNT 1000000

enum modelType { MODEL_GM, MODEL_BP, MODEL_REALBP, MODEL_REALBPS, MODEL_RNM, MODEL_GMP0, MODEL_RNMBP, MODEL_RNMREALBPS};

using namespace std;
using namespace Eigen;

class Kernel
{
	public:
    Kernel();

    void initialize(Sim& sim);
    void execute(Sim& sim);
    void finalize(Sim& sim);

    void loadNetwork();
    int step();
    // Step for the neuronGM model
    int stepGM();

    int stepCalculateAvalanches(int activeNeuron);
    int initializeRNG();
    void precomputeGammaFunction(bool verbose = true);

    void saveSpikes(bool partial = false);
    void saveAvalanches(bool partial = false);
    void resetSpikes(bool partial = false);
    void resetSimulation();
    int findRoot(int prevSpike);

//E    void printSparse(sp_umat M);
//E    void saveSparse(sp_umat M, string linksFile, string weightsFile);
//E    void saveSparse(sp_mat M, string linksFile, string weightsFile);

  private:
    ofstream outputPercolationStepsStream, outputSpikesStream; 
    ofstream outputAvalancheStatisticsStream, outputAvalancheStatisticsSlowStream, outputAvalancheStatisticsFullStream, outputParentRelationsStream;
    ofstream outputPercolationPointStream;
    string networkLinksFile, networkWeightsFile;
    string networkOutputLinksFile, networkOutputWeightsFile;
    string outputBaseFile, outputSpikes, outputMeans;
    string outputPercolationSteps, outputExtension, outputAvalancheStatistics, outputAvalancheStatisticsFull;
    string outputPercolationPoint;
    int model;
    double x, xInitialStep, xMinimumStep, xCurrentStep;
    double percolationThreshold;
    double p0, p1;
    double beta;
    double tau_D;
    int m0;
    int steps, networkSize, currentStep, N, maxVectorSize;
    double Kmax;
    int currentSpike, totalSpikeCount, parent, currentLink, currentParentRelation;
    bool depression;

    // eigen variables
    VectorXd P_m;
    VectorXd meanActivity, meanP, stdP, meanBranching, stdBranching, meanBranching2, meanD;
    VectorXd activeInputs;
    VectorXd currentBranching;
    VectorXd S, Sprev;
    VectorXd I, Iprev, prevCorrelatedSpikes;
    VectorXd inputVector;
    VectorXd D;

    SparseMatrix<double>  RS, RST;
    SparseMatrix<double>  RSeffective, RSeffectiveOriginal;

    VectorXi spikeNeuron, spikeTime;
    VectorXi spikeIndex, spikeParent, spikeGlobalIndex, spikeInputs;
    VectorXi spikeClusterSize, spikeClusterInputs;
    ArrayXXi spikeClusterDuration;
    ArrayXXi avalancheLinksData;
    ArrayXXi avalancheParentRelations;

    int percolationEvents;

    gsl_rng* rng;			// RNG structure
    int rngSeed;

    bool calculateMeans;
    bool avalancheOnlyHack;
    bool calculateAvalanches, calculateEffectiveNetwork, saveSpikesData;
    bool fastAvalancheCalculation, fastPercolationCalculation;
    bool saveFullAvalancheData;
    bool homogeneousGM;
    bool runFindPercolationPoint;
    time_t start, partial, end;
    bool tmpDebug;
    bool prevStepClean;
};

std::string sec2string(double seconds);
std::string sec2string(long seconds);

