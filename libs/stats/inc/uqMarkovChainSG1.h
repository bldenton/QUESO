/*--------------------------------------------------------------------------
 *--------------------------------------------------------------------------
 *
 * Copyright (C) 2008 The PECOS Development Team
 *
 * Please see http://pecos.ices.utexas.edu for more information.
 *
 * This file is part of the QUESO Library (Quantification of Uncertainty
 * for Estimation, Simulation and Optimization).
 *
 * QUESO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QUESO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QUESO. If not, see <http://www.gnu.org/licenses/>.
 *
 *--------------------------------------------------------------------------
 *
 * $Id$
 *
 * Brief description of this file: 
 * 
 *--------------------------------------------------------------------------
 *-------------------------------------------------------------------------- */

#ifndef __UQ_MAC_SG1_H__
#define __UQ_MAC_SG1_H__

#define UQ_USES_TK_CLASS
#undef  UQ_MAC_SG_REQUIRES_INVERTED_COV_MATRICES
#define UQ_MAC_SG_REQUIRES_TARGET_DISTRIBUTION_ONLY
#define UQ_NOTHING_JUST_FOR_TEST_OF_SVN_ID 1

#define UQ_MAC_SG_MARKOV_CHAIN_TYPE      1
#define UQ_MAC_SG_WHITE_NOISE_CHAIN_TYPE 2
#define UQ_MAC_SG_UNIFORM_CHAIN_TYPE     3
#define UQ_MAC_SG_FILENAME_FOR_NO_FILE   "."

// _ODV = option default value
#define UQ_MAC_SG_OUTPUT_FILE_NAME_ODV                     UQ_MAC_SG_FILENAME_FOR_NO_FILE
#define UQ_MAC_SG_OUTPUT_ALLOW_ODV                         ""

#define UQ_MAC_SG_RAW_CHAIN_TYPE_ODV                           UQ_MAC_SG_MARKOV_CHAIN_TYPE
#define UQ_MAC_SG_RAW_CHAIN_DATA_INPUT_FILE_NAME_ODV                UQ_MAC_SG_FILENAME_FOR_NO_FILE
#define UQ_MAC_SG_RAW_CHAIN_SIZE_ODV                           100
#define UQ_MAC_SG_RAW_CHAIN_GENERATE_EXTRA_ODV                 0
#define UQ_MAC_SG_RAW_CHAIN_DISPLAY_PERIOD_ODV                 500
#define UQ_MAC_SG_RAW_CHAIN_MEASURE_RUN_TIMES_ODV              0
#define UQ_MAC_SG_RAW_CHAIN_DATA_OUTPUT_FILE_NAME_ODV          UQ_MAC_SG_FILENAME_FOR_NO_FILE
#define UQ_MAC_SG_RAW_CHAIN_DATA_OUTPUT_ALLOW_ODV              ""
#define UQ_MAC_SG_RAW_CHAIN_COMPUTE_STATS_ODV                  0
#define UQ_MAC_SG_FILTERED_CHAIN_GENERATE_ODV              0
#define UQ_MAC_SG_FILTERED_CHAIN_DISCARDED_PORTION_ODV     0.
#define UQ_MAC_SG_FILTERED_CHAIN_LAG_ODV                   1
#define UQ_MAC_SG_FILTERED_CHAIN_DATA_OUTPUT_FILE_NAME_ODV UQ_MAC_SG_FILENAME_FOR_NO_FILE
#define UQ_MAC_SG_FILTERED_CHAIN_DATA_OUTPUT_ALLOW_ODV     ""
#define UQ_MAC_SG_FILTERED_CHAIN_COMPUTE_STATS_ODV         0
#define UQ_MAC_SG_MH_DISPLAY_CANDIDATES_ODV                0
#define UQ_MAC_SG_MH_PUT_OUT_OF_BOUNDS_IN_CHAIN_ODV        1
#define UQ_MAC_SG_TK_USE_LOCAL_HESSIAN_ODV                 0
#define UQ_MAC_SG_TK_USE_NEWTON_COMPONENT_ODV              1
#define UQ_MAC_SG_DR_MAX_NUM_EXTRA_STAGES_ODV              0
#define UQ_MAC_SG_DR_SCALES_FOR_EXTRA_STAGES_ODV           "1."
#define UQ_MAC_SG_AM_INIT_NON_ADAPT_INT_ODV                0
#define UQ_MAC_SG_AM_ADAPT_INTERVAL_ODV                    0
#define UQ_MAC_SG_AM_ETA_ODV                               1.
#define UQ_MAC_SG_AM_EPSILON_ODV                           1.e-5

#include <uqTKGroup.h>
#include <uqSequenceStatisticalOptions.h>
#include <uqVectorRV.h>
#include <uqVectorSpace.h>
#include <uqMarkovChainPositionData.h>
#include <uqScalarFunctionSynchronizer.h>
#include <uqMiscellaneous.h>
#include <uqSequenceOfVectors.h>
#include <uqArrayOfSequences.h>
#include <sys/time.h>
#include <fstream>

/*! A templated class that represents a Markov Chain generator. 'SG' stands for 'Sequence Generator'.
 */
template <class P_V,class P_M>
class uqMarkovChainSGClass
{
public:
  uqMarkovChainSGClass(const char*                         prefix,                  /*! Prefix.                     */
                       const uqBaseVectorRVClass<P_V,P_M>& sourceRv,                /*! The source random variable. */
                       const P_V&                          initialPosition,         /*! First position of chain.    */
                       const P_M*                          inputProposalCovMatrix); /*! Proposal covariance matrix. */ 
 ~uqMarkovChainSGClass();

  void   generateSequence           (uqBaseVectorSequenceClass<P_V,P_M>& workingChain); /*! Generate the chain and store it in 'workingChain' object */
  void   checkTheParallelEnvironment();

  void   print                      (std::ostream& os) const;


private:
//void   proc0GenerateSequence    (uqBaseVectorSequenceClass<P_V,P_M>& workingChain); /*! */
  void   resetChainAndRelatedInfo ();
  void   defineMyOptions          (po::options_description&                             optionsDesc);
  void   getMyOptionValues        (po::options_description&                             optionsDesc);

  void   generateWhiteNoiseChain  (      unsigned int                                   chainSize,
                                   uqBaseVectorSequenceClass<P_V,P_M>&                  workingChain);
  void   generateUniformChain     (      unsigned int                                   chainSize,
                                   uqBaseVectorSequenceClass<P_V,P_M>&                  workingChain);
  void   generateFullChain        (const P_V&                                           valuesOf1stPosition,
                                         unsigned int                                   chainSize,
                                   uqBaseVectorSequenceClass<P_V,P_M>&                  workingChain);
  void   readFullChain            (const std::string&                                   inputFileName,
                                         unsigned int                                   chainSize,
                                   uqBaseVectorSequenceClass<P_V,P_M>&                  workingChain);
  void   updateAdaptedCovMatrix   (const uqBaseVectorSequenceClass<P_V,P_M>&            subChain,
                                   unsigned int                                         idOfFirstPositionInSubChain,
                                   double&                                              lastChainSize,
                                   P_V&                                                 lastMean,
                                   P_M&                                                 lastAdaptedCovMatrix);

#ifdef UQ_USES_TK_CLASS
#else
  int    computeInitialCholFactors();
  void   updateTK                 ();
  double logProposal              (const uqMarkovChainPositionDataClass<P_V>&               x,
                                   const uqMarkovChainPositionDataClass<P_V>&               y,
                                   unsigned int                                             idOfProposalCovMatrix);
  double logProposal              (const std::vector<uqMarkovChainPositionDataClass<P_V>*>& inputPositions);
#endif
  double alpha                    (const uqMarkovChainPositionDataClass<P_V>&               x,
                                   const uqMarkovChainPositionDataClass<P_V>&               y,
                                   unsigned int                                             xStageId,
                                   unsigned int                                             yStageId,
                                   double*                                                  alphaQuotientPtr = NULL);
  double alpha                    (const std::vector<uqMarkovChainPositionDataClass<P_V>*>& inputPositions,
                                   const std::vector<unsigned int                        >& inputTKStageIds);
  bool   acceptAlpha              (double                                                   alpha);

  int    writeInfo                (const uqBaseVectorSequenceClass<P_V,P_M>&                workingChain,
                                   std::ofstream&                                           ofsvar) const;
                                 //const P_M*                                               mahalanobisMatrix = NULL,
                                 //bool                                                     applyMahalanobisInvert = true) const;

  const uqBaseEnvironmentClass&                     m_env;
        std::string                                 m_prefix;
  const uqVectorSpaceClass <P_V,P_M>&               m_vectorSpace;
  const uqBaseJointPdfClass<P_V,P_M>&               m_targetPdf;
        P_V                                         m_initialPosition;
  const P_M*                                        m_initialProposalCovMatrix;
        bool                                        m_nullInputProposalCovMatrix;
  const uqScalarFunctionSynchronizerClass<P_V,P_M>* m_targetPdfSynchronizer;

        po::options_description*           m_optionsDesc;
        std::string                        m_option_help;
        std::string                        m_option_dataOutputFileName;
        std::string                        m_option_dataOutputAllowedSet;

        std::string                        m_option_rawChain_type;
        std::string                        m_option_rawChain_dataInputFileName;
        std::string                        m_option_rawChain_size;
        std::string                        m_option_rawChain_generateExtra;
        std::string                        m_option_rawChain_displayPeriod;
        std::string                        m_option_rawChain_measureRunTimes;
        std::string                        m_option_rawChain_dataOutputFileName;
        std::string                        m_option_rawChain_dataOutputAllowedSet;
        std::string                        m_option_rawChain_computeStats;
        std::string                        m_option_filteredChain_generate;
        std::string                        m_option_filteredChain_discardedPortion;
        std::string                        m_option_filteredChain_lag;
        std::string                        m_option_filteredChain_dataOutputFileName;
        std::string                        m_option_filteredChain_dataOutputAllowedSet;
        std::string                        m_option_filteredChain_computeStats;
	std::string                        m_option_mh_displayCandidates;
	std::string                        m_option_mh_putOutOfBoundsInChain;
	std::string                        m_option_tk_useLocalHessian;
	std::string                        m_option_tk_useNewtonComponent;
        std::string                        m_option_dr_maxNumExtraStages;
        std::string                        m_option_dr_scalesForExtraStages;
        std::string                        m_option_am_initialNonAdaptInterval;
        std::string                        m_option_am_adaptInterval;
        std::string                        m_option_am_eta;
        std::string                        m_option_am_epsilon;

        std::string                        m_dataOutputFileName;
        std::set<unsigned int>             m_dataOutputAllowedSet;

        unsigned int                       m_rawChainType;
	std::string                        m_rawChainDataInputFileName;
        unsigned int                       m_rawChainSize;
        bool                               m_rawChainGenerateExtra;
        unsigned int                       m_rawChainDisplayPeriod;
        bool                               m_rawChainMeasureRunTimes;
        std::string                        m_rawChainDataOutputFileName;
        std::set<unsigned int>             m_rawChainDataOutputAllowedSet;
        bool                               m_rawChainComputeStats;
        uqSequenceStatisticalOptionsClass* m_rawChainStatisticalOptions;

        bool                               m_filteredChainGenerate;
        double                             m_filteredChainDiscardedPortion; // input or set during run time
        unsigned int                       m_filteredChainLag;              // input or set during run time
        std::string                        m_filteredChainDataOutputFileName;
        std::set<unsigned int>             m_filteredChainDataOutputAllowedSet;
        bool                               m_filteredChainComputeStats;
        uqSequenceStatisticalOptionsClass* m_filteredChainStatisticalOptions;

        bool                               m_mhDisplayCandidates;
        bool                               m_mhPutOutOfBoundsInChain;
        bool                               m_tkUseLocalHessian;
        bool                               m_tkUseNewtonComponent;
        unsigned int                       m_drMaxNumExtraStages;
        std::vector<double>                m_drScalesForCovMatrices;
        unsigned int                       m_amInitialNonAdaptInterval;
        unsigned int                       m_amAdaptInterval;
        double                             m_amEta;
        double                             m_amEpsilon;

        uqBaseTKGroupClass<P_V,P_M>*       m_tk;
#ifdef UQ_USES_TK_CLASS
#else
        bool                               m_tkIsSymmetric;
        std::vector<P_M*>                  m_lowerCholProposalCovMatrices;
        std::vector<P_M*>                  m_proposalCovMatrices;
#ifdef UQ_MAC_SG_REQUIRES_INVERTED_COV_MATRICES
        std::vector<P_M*>                  m_upperCholProposalPrecMatrices;
        std::vector<P_M*>                  m_proposalPrecMatrices;
#endif
#endif
        std::vector<unsigned int>          m_idsOfUniquePositions;
        std::vector<double>                m_logTargets;
        std::vector<double>                m_alphaQuotients;
        double                             m_rawChainRunTime;
        unsigned int                       m_numRejections;
        unsigned int                       m_numOutOfTargetSupport;
        double                             m_lastChainSize;
        P_V*                               m_lastMean;
        P_M*                               m_lastAdaptedCovMatrix;
};

template<class P_V,class P_M>
std::ostream& operator<<(std::ostream& os, const uqMarkovChainSGClass<P_V,P_M>& obj);

#include <uqMarkovChainSG2.h>

template<class P_V,class P_M>
uqMarkovChainSGClass<P_V,P_M>::uqMarkovChainSGClass(
  const char*                         prefix,
  const uqBaseVectorRVClass<P_V,P_M>& sourceRv,
  const P_V&                          initialPosition,
  const P_M*                          inputProposalCovMatrix)
  :
  m_env                                      (sourceRv.env()),
  m_prefix                                   ((std::string)(prefix) + "mc_"),
  m_vectorSpace                              (sourceRv.imageSet().vectorSpace()),
  m_targetPdf                                (sourceRv.pdf()),
  m_initialPosition                          (initialPosition),
  m_initialProposalCovMatrix                 (inputProposalCovMatrix),
  m_nullInputProposalCovMatrix               (inputProposalCovMatrix == NULL),
  m_targetPdfSynchronizer                    (new uqScalarFunctionSynchronizerClass<P_V,P_M>(m_targetPdf,m_initialPosition)),
  m_optionsDesc                              (new po::options_description("Bayesian Markov chain options")),
  m_option_help                              (m_prefix + "help"                              ),
  m_option_dataOutputFileName                (m_prefix + "dataOutputFileName"                ),
  m_option_dataOutputAllowedSet              (m_prefix + "dataOutputAllowedSet"              ),
  m_option_rawChain_type                     (m_prefix + "rawChain_type"                     ),
  m_option_rawChain_dataInputFileName        (m_prefix + "rawChain_dataInputFileName"        ),
  m_option_rawChain_size                     (m_prefix + "rawChain_size"                     ),
  m_option_rawChain_generateExtra            (m_prefix + "rawChain_generateExtra"            ),
  m_option_rawChain_displayPeriod            (m_prefix + "rawChain_displayPeriod"            ),
  m_option_rawChain_measureRunTimes          (m_prefix + "rawChain_measureRunTimes"          ),
  m_option_rawChain_dataOutputFileName       (m_prefix + "rawChain_dataOutputFileName"       ),
  m_option_rawChain_dataOutputAllowedSet     (m_prefix + "rawChain_dataOutputAllowedSet"     ),
  m_option_rawChain_computeStats             (m_prefix + "rawChain_computeStats"             ),
  m_option_filteredChain_generate            (m_prefix + "filteredChain_generate"            ),
  m_option_filteredChain_discardedPortion    (m_prefix + "filteredChain_discardedPortion"    ),
  m_option_filteredChain_lag                 (m_prefix + "filteredChain_lag"                 ),
  m_option_filteredChain_dataOutputFileName  (m_prefix + "filteredChain_dataOutputFileName"  ),
  m_option_filteredChain_dataOutputAllowedSet(m_prefix + "filteredChain_dataOutputAllowedSet"),
  m_option_filteredChain_computeStats        (m_prefix + "filteredChain_computeStats"        ),
  m_option_mh_displayCandidates              (m_prefix + "mh_displayCandidates"              ),
  m_option_mh_putOutOfBoundsInChain          (m_prefix + "mh_putOutOfBoundsInChain"          ),
  m_option_tk_useLocalHessian                (m_prefix + "tk_useLocalHessian"                ),
  m_option_tk_useNewtonComponent             (m_prefix + "tk_useNewtonComponent"             ),
  m_option_dr_maxNumExtraStages              (m_prefix + "dr_maxNumExtraStages"              ),
  m_option_dr_scalesForExtraStages           (m_prefix + "dr_scalesForExtraStages"           ),
  m_option_am_initialNonAdaptInterval        (m_prefix + "am_initialNonAdaptInterval"        ),
  m_option_am_adaptInterval                  (m_prefix + "am_adaptInterval"                  ),
  m_option_am_eta                            (m_prefix + "am_eta"                            ),
  m_option_am_epsilon                        (m_prefix + "am_epsilon"                        ),
  m_dataOutputFileName                       (UQ_MAC_SG_RAW_CHAIN_DATA_OUTPUT_FILE_NAME_ODV  ),
//m_dataOutputAllowedSet                     (),
  m_rawChainType                             (UQ_MAC_SG_RAW_CHAIN_TYPE_ODV),
  m_rawChainDataInputFileName                (UQ_MAC_SG_RAW_CHAIN_DATA_INPUT_FILE_NAME_ODV),
  m_rawChainSize                             (UQ_MAC_SG_RAW_CHAIN_SIZE_ODV),
  m_rawChainGenerateExtra                    (UQ_MAC_SG_RAW_CHAIN_GENERATE_EXTRA_ODV),
  m_rawChainDisplayPeriod                    (UQ_MAC_SG_RAW_CHAIN_DISPLAY_PERIOD_ODV),
  m_rawChainMeasureRunTimes                  (UQ_MAC_SG_RAW_CHAIN_MEASURE_RUN_TIMES_ODV),
  m_rawChainDataOutputFileName               (UQ_MAC_SG_RAW_CHAIN_DATA_OUTPUT_FILE_NAME_ODV),
//m_rawChainDataOutputAllowedSet             (),
  m_rawChainComputeStats                     (UQ_MAC_SG_RAW_CHAIN_COMPUTE_STATS_ODV),
  m_rawChainStatisticalOptions               (NULL),
  m_filteredChainGenerate                    (UQ_MAC_SG_FILTERED_CHAIN_GENERATE_ODV),
  m_filteredChainDiscardedPortion            (UQ_MAC_SG_FILTERED_CHAIN_DISCARDED_PORTION_ODV),
  m_filteredChainLag                         (UQ_MAC_SG_FILTERED_CHAIN_LAG_ODV),
  m_filteredChainDataOutputFileName          (UQ_MAC_SG_FILTERED_CHAIN_DATA_OUTPUT_FILE_NAME_ODV),
//m_filteredChainDataOutputAllowedSet        (),
  m_filteredChainComputeStats                (UQ_MAC_SG_FILTERED_CHAIN_COMPUTE_STATS_ODV),
  m_filteredChainStatisticalOptions          (NULL),
  m_mhDisplayCandidates                      (UQ_MAC_SG_MH_DISPLAY_CANDIDATES_ODV),
  m_mhPutOutOfBoundsInChain                  (UQ_MAC_SG_MH_PUT_OUT_OF_BOUNDS_IN_CHAIN_ODV),
  m_tkUseLocalHessian                        (UQ_MAC_SG_TK_USE_LOCAL_HESSIAN_ODV),
  m_tkUseNewtonComponent                     (UQ_MAC_SG_TK_USE_NEWTON_COMPONENT_ODV),
  m_drMaxNumExtraStages                      (UQ_MAC_SG_DR_MAX_NUM_EXTRA_STAGES_ODV),
  m_drScalesForCovMatrices                   (1,1.),
  m_amInitialNonAdaptInterval                (UQ_MAC_SG_AM_INIT_NON_ADAPT_INT_ODV),
  m_amAdaptInterval                          (UQ_MAC_SG_AM_ADAPT_INTERVAL_ODV),
  m_amEta                                    (UQ_MAC_SG_AM_ETA_ODV),
  m_amEpsilon                                (UQ_MAC_SG_AM_EPSILON_ODV),
  m_tk                                       (NULL),
#ifdef UQ_USES_TK_CLASS
#else
  m_tkIsSymmetric                            (true),
  m_lowerCholProposalCovMatrices             (1),//NULL),
  m_proposalCovMatrices                      (1),//NULL),
#ifdef UQ_MAC_SG_REQUIRES_INVERTED_COV_MATRICES
  m_upperCholProposalPrecMatrices            (1),//NULL),
  m_proposalPrecMatrices                     (1),//NULL),
#endif
#endif
  m_idsOfUniquePositions                     (0),//0.),
  m_logTargets                               (0),//0.),
  m_alphaQuotients                           (0),//0.),
  m_rawChainRunTime                          (0.),
  m_numRejections                            (0),
  m_numOutOfTargetSupport                    (0),
  m_lastChainSize                            (0),
  m_lastMean                                 (NULL),
  m_lastAdaptedCovMatrix                     (NULL)
{
  if (m_env.subDisplayFile()) {
    *m_env.subDisplayFile() << "Entering uqMarkovChainSGClass<P_V,P_M>::constructor()"
                           << std::endl;
  }

  defineMyOptions                (*m_optionsDesc);
  m_env.scanInputFileForMyOptions(*m_optionsDesc);
  getMyOptionValues              (*m_optionsDesc);

  if (m_env.subDisplayFile()) {
    *m_env.subDisplayFile() << "In uqMarkovChainSGClass<P_V,P_M>::constructor()"
                           << ": after getting values of options with prefix '" << m_prefix
                           << "', state of  object is:"
                           << "\n" << *this
                           << std::endl;
  }

  /////////////////////////////////////////////////////////////////
  // Instantiate the appropriate TK
  /////////////////////////////////////////////////////////////////
#ifdef UQ_USES_TK_CLASS
  if (m_env.subDisplayFile()) {
    *m_env.subDisplayFile() << "In uqMarkovChainSGClass<P_V,P_M>::constructor()"
                           << ": running with UQ_USES_TK_CLASS flag defined"
                           << std::endl;
  }
#else
  if (m_env.subDisplayFile()) {
    *m_env.subDisplayFile() << "In uqMarkovChainSGClass<P_V,P_M>::constructor()"
                           << ": running with UQ_USES_TK_CLASS flag undefined"
                           << std::endl;
  }
#endif
  if (m_tkUseLocalHessian) {
    m_tk = new uqHessianCovMatricesTKGroupClass<P_V,P_M>(m_prefix.c_str(),
                                                         m_vectorSpace,
                                                         m_drScalesForCovMatrices,
                                                         *m_targetPdfSynchronizer);
    if (m_env.subDisplayFile()) {
      *m_env.subDisplayFile() << "In uqMarkovChainSGClass<P_V,P_M>::constructor()"
                             << ": just instantiated a 'HessianCovMatrices' TK class"
                             << std::endl;
    }
  }
  else {
    if (m_initialProposalCovMatrix == NULL) {
      UQ_FATAL_TEST_MACRO(true,
                          m_env.fullRank(),
                          "uqMarkovChainSGClass<P_V,P_M>::constructor()",
                          "proposal cov matrix should have been passed by user, since, according to the input algorithm options, local Hessians will not be used in the proposal");
    }

    m_tk = new uqScaledCovMatrixTKGroupClass<P_V,P_M>(m_prefix.c_str(),
                                                      m_vectorSpace,
                                                      m_drScalesForCovMatrices,
                                                      *m_initialProposalCovMatrix);
    if (m_env.subDisplayFile()) {
      *m_env.subDisplayFile() << "In uqMarkovChainSGClass<P_V,P_M>::constructor()"
                             << ": just instantiated a 'ScaledCovMatrix' TK class"
                             << std::endl;
    }
  }


  if (m_rawChainComputeStats     ) m_rawChainStatisticalOptions      = new uqSequenceStatisticalOptionsClass(m_env,m_prefix + "rawChain_"     );
  if (m_filteredChainComputeStats) m_filteredChainStatisticalOptions = new uqSequenceStatisticalOptionsClass(m_env,m_prefix + "filteredChain_");

  if (m_env.subDisplayFile()) {
    *m_env.subDisplayFile() << "Leaving uqMarkovChainSGClass<P_V,P_M>::constructor()"
                           << std::endl;
  }
}

template<class P_V,class P_M>
uqMarkovChainSGClass<P_V,P_M>::~uqMarkovChainSGClass()
{
  //if (m_env.subDisplayFile()) {
  //  *m_env.subDisplayFile() << "Entering uqMarkovChainSGClass<P_V,P_M>::destructor()"
  //                         << std::endl;
  //}

  resetChainAndRelatedInfo();
  //if (m_lowerCholProposalCovMatrices[0]) delete m_lowerCholProposalCovMatrices[0]; // Loop inside 'resetChainAndRelatedInfo()' deletes just from position '1' on

  if (m_filteredChainStatisticalOptions) delete m_filteredChainStatisticalOptions;
  if (m_rawChainStatisticalOptions     ) delete m_rawChainStatisticalOptions;
  if (m_optionsDesc                    ) delete m_optionsDesc;

  if (m_tk                             ) delete m_tk;
  if (m_targetPdfSynchronizer          ) delete m_targetPdfSynchronizer;
  if (m_nullInputProposalCovMatrix     ) delete m_initialProposalCovMatrix;

  //if (m_env.subDisplayFile()) {
  //  *m_env.subDisplayFile() << "Leaving uqMarkovChainSGClass<P_V,P_M>::destructor()"
  //                         << std::endl;
  //}
}

template<class P_V,class P_M>
void
uqMarkovChainSGClass<P_V,P_M>::resetChainAndRelatedInfo()
{
  if (m_lastAdaptedCovMatrix) delete m_lastAdaptedCovMatrix;
  if (m_lastMean)             delete m_lastMean;
  m_lastChainSize         = 0;
  m_numOutOfTargetSupport = 0;
  m_rawChainRunTime       = 0.;
  m_numRejections         = 0;
  m_alphaQuotients.clear();
  m_logTargets.clear();
  m_idsOfUniquePositions.clear();

#ifdef UQ_USES_TK_CLASS
#else
#ifdef UQ_MAC_SG_REQUIRES_INVERTED_COV_MATRICES
  for (unsigned int i = 0; i < m_proposalPrecMatrices.size(); ++i) {
    if (m_proposalPrecMatrices[i]) delete m_proposalPrecMatrices[i];
  }
  for (unsigned int i = 0; i < m_upperCholProposalPrecMatrices.size(); ++i) {
    if (m_upperCholProposalPrecMatrices[i]) delete m_upperCholProposalPrecMatrices[i];
  }
#endif
  for (unsigned int i = 0; i < m_proposalCovMatrices.size(); ++i) {
    if (m_proposalCovMatrices[i]) {
      delete m_proposalCovMatrices[i];
      m_proposalCovMatrices[i] = NULL;
    }
  }
//m_proposalCovMatrices.clear(); // Do not clear
  for (unsigned int i = 0; i < m_lowerCholProposalCovMatrices.size(); ++i) { // Yes, from '1' on
    if (m_lowerCholProposalCovMatrices[i]) {
      delete m_lowerCholProposalCovMatrices[i];
      m_lowerCholProposalCovMatrices[i] = NULL;
    }
  }
//m_lowerCholProposalCovMatrices.clear(); // Do not clear
#endif

  return;
}

template<class P_V,class P_M>
void
uqMarkovChainSGClass<P_V,P_M>::defineMyOptions(
  po::options_description& optionsDesc)
{
  optionsDesc.add_options()
    (m_option_help.c_str(),                                                                                                                             "produce help message for Bayesian Markov chain distr. calculator")
    (m_option_dataOutputFileName.c_str(),                 po::value<std::string >()->default_value(UQ_MAC_SG_RAW_CHAIN_DATA_OUTPUT_FILE_NAME_ODV     ), "name of generic output file"                                     )
    (m_option_dataOutputAllowedSet.c_str(),               po::value<std::string >()->default_value(UQ_MAC_SG_RAW_CHAIN_DATA_OUTPUT_ALLOW_ODV         ), "subEnvs that will write to generic output file"                  )
    (m_option_rawChain_type.c_str(),                      po::value<unsigned int>()->default_value(UQ_MAC_SG_RAW_CHAIN_TYPE_ODV                      ), "type of raw chain (1=Markov, 2=White noise)"                     )
    (m_option_rawChain_dataInputFileName.c_str(),         po::value<std::string >()->default_value(UQ_MAC_SG_RAW_CHAIN_DATA_INPUT_FILE_NAME_ODV      ), "name of input file for raw chain "                               )
    (m_option_rawChain_size.c_str(),                      po::value<unsigned int>()->default_value(UQ_MAC_SG_RAW_CHAIN_SIZE_ODV                      ), "size of raw chain"                                               )
    (m_option_rawChain_generateExtra.c_str(),             po::value<bool        >()->default_value(UQ_MAC_SG_RAW_CHAIN_GENERATE_EXTRA_ODV            ), "generate extra information about raw chain"                      )
    (m_option_rawChain_displayPeriod.c_str(),             po::value<unsigned int>()->default_value(UQ_MAC_SG_RAW_CHAIN_DISPLAY_PERIOD_ODV            ), "period of message display during raw chain generation"           )
    (m_option_rawChain_measureRunTimes.c_str(),           po::value<bool        >()->default_value(UQ_MAC_SG_RAW_CHAIN_MEASURE_RUN_TIMES_ODV         ), "measure run times"                                               )
    (m_option_rawChain_dataOutputFileName.c_str(),        po::value<std::string >()->default_value(UQ_MAC_SG_RAW_CHAIN_DATA_OUTPUT_FILE_NAME_ODV     ), "name of output file for raw chain "                              )
    (m_option_rawChain_dataOutputAllowedSet.c_str(),      po::value<std::string >()->default_value(UQ_MAC_SG_RAW_CHAIN_DATA_OUTPUT_ALLOW_ODV         ), "subEnvs that will write to output file for raw chain"            )
    (m_option_rawChain_computeStats.c_str(),              po::value<bool        >()->default_value(UQ_MAC_SG_RAW_CHAIN_COMPUTE_STATS_ODV             ), "compute statistics on raw chain"                                 )
    (m_option_filteredChain_generate.c_str(),             po::value<bool        >()->default_value(UQ_MAC_SG_FILTERED_CHAIN_GENERATE_ODV             ), "generate filtered chain"                                         )
    (m_option_filteredChain_discardedPortion.c_str(),     po::value<double      >()->default_value(UQ_MAC_SG_FILTERED_CHAIN_DISCARDED_PORTION_ODV    ), "initial discarded portion for chain filtering"                   )
    (m_option_filteredChain_lag.c_str(),                  po::value<unsigned int>()->default_value(UQ_MAC_SG_FILTERED_CHAIN_LAG_ODV                  ), "spacing for chain filtering"                                     )
    (m_option_filteredChain_dataOutputFileName.c_str(),   po::value<std::string >()->default_value(UQ_MAC_SG_FILTERED_CHAIN_DATA_OUTPUT_FILE_NAME_ODV), "name of output file for filtered chain"                          )
    (m_option_filteredChain_dataOutputAllowedSet.c_str(), po::value<std::string >()->default_value(UQ_MAC_SG_FILTERED_CHAIN_DATA_OUTPUT_ALLOW_ODV    ), "subEnvs that will write to output file for filtered chain"       )
    (m_option_filteredChain_computeStats.c_str(),         po::value<bool        >()->default_value(UQ_MAC_SG_FILTERED_CHAIN_COMPUTE_STATS_ODV        ), "compute statistics on filtered chain"                            )
    (m_option_mh_displayCandidates.c_str(),               po::value<bool        >()->default_value(UQ_MAC_SG_MH_DISPLAY_CANDIDATES_ODV               ), "display candidates generated in the core MH algorithm"           )
    (m_option_mh_putOutOfBoundsInChain.c_str(),           po::value<bool        >()->default_value(UQ_MAC_SG_MH_PUT_OUT_OF_BOUNDS_IN_CHAIN_ODV       ), "put 'out of bound' candidates in chain as well"                  )
    (m_option_tk_useLocalHessian.c_str(),                 po::value<bool        >()->default_value(UQ_MAC_SG_TK_USE_LOCAL_HESSIAN_ODV                ), "'proposal' use local Hessian"                                    )
    (m_option_tk_useNewtonComponent.c_str(),              po::value<bool        >()->default_value(UQ_MAC_SG_TK_USE_NEWTON_COMPONENT_ODV             ), "'proposal' use Newton component"                                 )
    (m_option_dr_maxNumExtraStages.c_str(),               po::value<unsigned int>()->default_value(UQ_MAC_SG_DR_MAX_NUM_EXTRA_STAGES_ODV             ), "'dr' maximum number of extra stages"                             )
    (m_option_dr_scalesForExtraStages.c_str(),            po::value<std::string >()->default_value(UQ_MAC_SG_DR_SCALES_FOR_EXTRA_STAGES_ODV          ), "'dr' list of scales for proposal cov matrices from 2nd stage on" )
    (m_option_am_initialNonAdaptInterval.c_str(),         po::value<unsigned int>()->default_value(UQ_MAC_SG_AM_INIT_NON_ADAPT_INT_ODV               ), "'am' initial non adaptation interval"                            )
    (m_option_am_adaptInterval.c_str(),                   po::value<unsigned int>()->default_value(UQ_MAC_SG_AM_ADAPT_INTERVAL_ODV                   ), "'am' adaptation interval"                                        )
    (m_option_am_eta.c_str(),                             po::value<double      >()->default_value(UQ_MAC_SG_AM_ETA_ODV                              ), "'am' eta"                                                        )
    (m_option_am_epsilon.c_str(),                         po::value<double      >()->default_value(UQ_MAC_SG_AM_EPSILON_ODV                          ), "'am' epsilon"                                                    )
  ;

  return;
}

template<class P_V,class P_M>
void
uqMarkovChainSGClass<P_V,P_M>::getMyOptionValues(
  po::options_description& optionsDesc)
{
  if (m_env.allOptionsMap().count(m_option_help.c_str())) {
    if (m_env.subDisplayFile()) {
      *m_env.subDisplayFile() << optionsDesc
                             << std::endl;
    }
  }

  if (m_env.allOptionsMap().count(m_option_dataOutputFileName.c_str())) {
    m_dataOutputFileName = ((const po::variable_value&) m_env.allOptionsMap()[m_option_dataOutputFileName.c_str()]).as<std::string>();
  }

  if (m_env.allOptionsMap().count(m_option_dataOutputAllowedSet.c_str())) {
    m_dataOutputAllowedSet.clear();
    std::vector<double> tmpAllow(0,0.);
    std::string inputString = m_env.allOptionsMap()[m_option_dataOutputAllowedSet.c_str()].as<std::string>();
    uqMiscReadDoublesFromString(inputString,tmpAllow);

    if (tmpAllow.size() > 0) {
      for (unsigned int i = 0; i < tmpAllow.size(); ++i) {
        m_dataOutputAllowedSet.insert((unsigned int) tmpAllow[i]);
      }
    }
  }

  if (m_env.allOptionsMap().count(m_option_rawChain_type.c_str())) {
    m_rawChainType = ((const po::variable_value&) m_env.allOptionsMap()[m_option_rawChain_type.c_str()]).as<unsigned int>();
  }

  if (m_env.allOptionsMap().count(m_option_rawChain_dataInputFileName.c_str())) {
    m_rawChainDataInputFileName = ((const po::variable_value&) m_env.allOptionsMap()[m_option_rawChain_dataInputFileName.c_str()]).as<std::string>();
  }

  if (m_env.allOptionsMap().count(m_option_rawChain_size.c_str())) {
    m_rawChainSize = ((const po::variable_value&) m_env.allOptionsMap()[m_option_rawChain_size.c_str()]).as<unsigned int>();
  }

  if (m_env.allOptionsMap().count(m_option_rawChain_displayPeriod.c_str())) {
    m_rawChainDisplayPeriod = ((const po::variable_value&) m_env.allOptionsMap()[m_option_rawChain_displayPeriod.c_str()]).as<unsigned int>();
  }

  if (m_env.allOptionsMap().count(m_option_rawChain_measureRunTimes.c_str())) {
    m_rawChainMeasureRunTimes = ((const po::variable_value&) m_env.allOptionsMap()[m_option_rawChain_measureRunTimes.c_str()]).as<bool>();
  }

  if (m_env.allOptionsMap().count(m_option_rawChain_dataOutputFileName.c_str())) {
    m_rawChainDataOutputFileName = ((const po::variable_value&) m_env.allOptionsMap()[m_option_rawChain_dataOutputFileName.c_str()]).as<std::string>();
  }

  if (m_env.allOptionsMap().count(m_option_rawChain_dataOutputAllowedSet.c_str())) {
    m_rawChainDataOutputAllowedSet.clear();
    std::vector<double> tmpAllow(0,0.);
    std::string inputString = m_env.allOptionsMap()[m_option_rawChain_dataOutputAllowedSet.c_str()].as<std::string>();
    uqMiscReadDoublesFromString(inputString,tmpAllow);

    if (tmpAllow.size() > 0) {
      for (unsigned int i = 0; i < tmpAllow.size(); ++i) {
        m_rawChainDataOutputAllowedSet.insert((unsigned int) tmpAllow[i]);
      }
    }
  }

  if (m_env.allOptionsMap().count(m_option_rawChain_computeStats.c_str())) {
    m_rawChainComputeStats = ((const po::variable_value&) m_env.allOptionsMap()[m_option_rawChain_computeStats.c_str()]).as<bool>();
  }

  if (m_env.allOptionsMap().count(m_option_rawChain_generateExtra.c_str())) {
    m_rawChainGenerateExtra = ((const po::variable_value&) m_env.allOptionsMap()[m_option_rawChain_generateExtra.c_str()]).as<bool>();
  }

  if (m_env.allOptionsMap().count(m_option_filteredChain_generate.c_str())) {
    m_filteredChainGenerate = ((const po::variable_value&) m_env.allOptionsMap()[m_option_filteredChain_generate.c_str()]).as<bool>();
  }

  if (m_env.allOptionsMap().count(m_option_filteredChain_discardedPortion.c_str())) {
    m_filteredChainDiscardedPortion = ((const po::variable_value&) m_env.allOptionsMap()[m_option_filteredChain_discardedPortion.c_str()]).as<double>();
  }

  if (m_env.allOptionsMap().count(m_option_filteredChain_lag.c_str())) {
    m_filteredChainLag = ((const po::variable_value&) m_env.allOptionsMap()[m_option_filteredChain_lag.c_str()]).as<unsigned int>();
  }
  if ((m_filteredChainGenerate == true) &&
      (m_filteredChainLag      < 2    )) {
    std::cerr << "WARNING In uqMarkovChainSGClass<P_V,P_M>::getMyOptionsValues()"
              << ", fullRank "              << m_env.fullRank()
              << ", subEnvironment "        << m_env.subId()
              << ", subRank "               << m_env.subRank()
              << ", inter0Rank "            << m_env.inter0Rank()
              << ": forcing the value of '" << m_option_filteredChain_lag.c_str()
              << "' from "                  << m_filteredChainLag
              << " to "                     << 2
              << std::endl;
    m_filteredChainLag = 2;
  }

  if (m_env.allOptionsMap().count(m_option_filteredChain_dataOutputFileName.c_str())) {
    m_filteredChainDataOutputFileName = ((const po::variable_value&) m_env.allOptionsMap()[m_option_filteredChain_dataOutputFileName.c_str()]).as<std::string>();
  }

  if (m_env.allOptionsMap().count(m_option_filteredChain_dataOutputAllowedSet.c_str())) {
    m_filteredChainDataOutputAllowedSet.clear();
    std::vector<double> tmpAllow(0,0.);
    std::string inputString = m_env.allOptionsMap()[m_option_filteredChain_dataOutputAllowedSet.c_str()].as<std::string>();
    uqMiscReadDoublesFromString(inputString,tmpAllow);

    if (tmpAllow.size() > 0) {
      for (unsigned int i = 0; i < tmpAllow.size(); ++i) {
        m_filteredChainDataOutputAllowedSet.insert((unsigned int) tmpAllow[i]);
      }
    }
  }

  if (m_env.allOptionsMap().count(m_option_filteredChain_computeStats.c_str())) {
    m_filteredChainComputeStats = ((const po::variable_value&) m_env.allOptionsMap()[m_option_filteredChain_computeStats.c_str()]).as<bool>();
  }

  if (m_env.allOptionsMap().count(m_option_mh_displayCandidates.c_str())) {
    m_mhDisplayCandidates = ((const po::variable_value&) m_env.allOptionsMap()[m_option_mh_displayCandidates.c_str()]).as<bool>();
  }

  if (m_env.allOptionsMap().count(m_option_mh_putOutOfBoundsInChain.c_str())) {
    m_mhPutOutOfBoundsInChain = ((const po::variable_value&) m_env.allOptionsMap()[m_option_mh_putOutOfBoundsInChain.c_str()]).as<bool>();
  }

  if (m_env.allOptionsMap().count(m_option_tk_useLocalHessian.c_str())) {
    m_tkUseLocalHessian = ((const po::variable_value&) m_env.allOptionsMap()[m_option_tk_useLocalHessian.c_str()]).as<bool>();
  }

  if (m_env.allOptionsMap().count(m_option_tk_useNewtonComponent.c_str())) {
    m_tkUseNewtonComponent = ((const po::variable_value&) m_env.allOptionsMap()[m_option_tk_useNewtonComponent.c_str()]).as<bool>();
  }

  if (m_env.allOptionsMap().count(m_option_dr_maxNumExtraStages.c_str())) {
    m_drMaxNumExtraStages = ((const po::variable_value&) m_env.allOptionsMap()[m_option_dr_maxNumExtraStages.c_str()]).as<unsigned int>();
  }

  std::vector<double> tmpScales(0,0.);
  if (m_env.allOptionsMap().count(m_option_dr_scalesForExtraStages.c_str())) {
    std::string inputString = ((const po::variable_value&) m_env.allOptionsMap()[m_option_dr_scalesForExtraStages.c_str()]).as<std::string>();
    uqMiscReadDoublesFromString(inputString,tmpScales);
    //if (m_env.subDisplayFile()) {
    //  *m_env.subDisplayFile() << "In uqMarkovChainSGClass<P_V,P_M>::getMyOptionValues(): scales =";
    //  for (unsigned int i = 0; i < tmpScales.size(); ++i) {
    //    *m_env.subDisplayFile() << " " << tmpScales[i];
    //  }
    //  *m_env.subDisplayFile() << std::endl;
    //}
  }

  if (m_drMaxNumExtraStages > 0) {
    m_drScalesForCovMatrices.clear();
#ifdef UQ_USES_TK_CLASS
#else
    m_lowerCholProposalCovMatrices.clear();
    m_proposalCovMatrices.clear();
#endif

    double scale = 1.0;
    unsigned int tmpSize = tmpScales.size();

    m_drScalesForCovMatrices.resize(m_drMaxNumExtraStages+1,1.);
#ifdef UQ_USES_TK_CLASS
#else
    m_lowerCholProposalCovMatrices.resize(m_drMaxNumExtraStages+1,NULL);
    m_proposalCovMatrices.resize         (m_drMaxNumExtraStages+1,NULL);
#endif

    for (unsigned int i = 1; i < (m_drMaxNumExtraStages+1); ++i) {
      if (i <= tmpSize) scale = tmpScales[i-1];
      m_drScalesForCovMatrices[i] = scale;
    }
    //updateTK();
  }

  if (m_env.allOptionsMap().count(m_option_am_initialNonAdaptInterval.c_str())) {
    m_amInitialNonAdaptInterval = ((const po::variable_value&) m_env.allOptionsMap()[m_option_am_initialNonAdaptInterval.c_str()]).as<unsigned int>();
  }

  if (m_env.allOptionsMap().count(m_option_am_adaptInterval.c_str())) {
    m_amAdaptInterval = ((const po::variable_value&) m_env.allOptionsMap()[m_option_am_adaptInterval.c_str()]).as<unsigned int>();
  }

  if (m_env.allOptionsMap().count(m_option_am_eta.c_str())) {
    m_amEta = ((const po::variable_value&) m_env.allOptionsMap()[m_option_am_eta.c_str()]).as<double>();
  }

  if (m_env.allOptionsMap().count(m_option_am_epsilon.c_str())) {
    m_amEpsilon = ((const po::variable_value&) m_env.allOptionsMap()[m_option_am_epsilon.c_str()]).as<double>();
  }

  return;
}

#ifdef UQ_USES_TK_CLASS
#else
template<class P_V,class P_M>
int
uqMarkovChainSGClass<P_V,P_M>::computeInitialCholFactors()
{
//const P_M& proposalCovMatrix (*m_initialProposalCovMatrix);
//const P_M& proposalPrecMatrix(...);

  if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 5)) {
    *m_env.subDisplayFile() << "Entering uqMarkovChainSGClass<P_V,P_M>::computeInitialCholFactors()..."
              << std::endl;
  }

  int iRC = UQ_OK_RC;

  if (m_env.subDisplayFile()) {
    *m_env.subDisplayFile() << "In uqMarkovChainSGClass<P_V,P_M>::computeInitialCholFactors()"
                                   << ": using supplied initialProposalCovMatrix, whose contents are:"
                                   << std::endl;
    *m_env.subDisplayFile() << *m_initialProposalCovMatrix; // FIX ME: output might need to be in parallel
    *m_env.subDisplayFile() << std::endl;
  }

#ifdef UQ_USES_TK_CLASS
#else
  m_lowerCholProposalCovMatrices[0] = new P_M(*m_initialProposalCovMatrix); 
  iRC = m_lowerCholProposalCovMatrices[0]->chol();
  UQ_FATAL_RC_MACRO(iRC,
                    m_env.fullRank(),
                    "uqMarkovChainSGClass<P_V,P_M>::computeInitialCholFactors()",
                    "initialProposalCovMatrix is not positive definite");
  m_lowerCholProposalCovMatrices[0]->zeroUpper(false);

  m_proposalCovMatrices[0] = new P_M(*m_initialProposalCovMatrix);

  if (m_env.subDisplayFile()) {
    *m_env.subDisplayFile() << "In uqMarkovChainSGClass<P_V,P_M>::computeInitialCholFactors()"
                                   << ", m_lowerCholProposalCovMatrices[0] contents are:"
                                   << std::endl;
    *m_env.subDisplayFile() << *(m_lowerCholProposalCovMatrices[0]); // FIX ME: output might need to be in parallel
    *m_env.subDisplayFile() << std::endl;
  }

#ifdef UQ_MAC_SG_REQUIRES_INVERTED_COV_MATRICES
  const P_M* internalProposalPrecMatrix = proposalPrecMatrix;
  if (proposalPrecMatrix == NULL) {
    UQ_FATAL_RC_MACRO(UQ_INCOMPLETE_IMPLEMENTATION_RC,
                      m_env.fullRank(),
                      "uqMarkovChainSGClass<P_V,P_M>::computeInitialCholFactors()",
                      "not yet implemented for the case 'proposalPrecMatrix == NULL'");
  }

  m_upperCholProposalPrecMatrices[0] = new P_M(proposalPrecMatrix); 
  iRC = m_upperCholProposalPrecMatrices[0]->chol();
  UQ_FATAL_RC_MACRO(iRC,
                    m_env.fullRank(),
                    "uqMarkovChainSGClass<P_V,P_M>::computeInitialCholFactors()",
                    "proposalPrecMatrix is not positive definite");
  m_upperCholProposalPrecMatrices[0]->zeroLower(false);

  m_proposalPrecMatrices[0] = new P_M(proposalPrecMatrix);

  //if (m_env.subDisplayFile()) {
  //  *m_env.subDisplayFile() << "In uqMarkovChainSGClass<P_V,P_M>::computeInitialCholFactors()"
  //                                 << ", m_upperCholProposalPrecMatrices[0] contents are:"
  //                                 << std::endl;
  //  *m_env.subDisplayFile() << *(m_upperCholProposalPrecMatrices[0]); // FIX ME: output might need to be in parallel
  //  *m_env.subDisplayFile() << std::endl;
  //}

#endif
#endif

  if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 5)) {
    *m_env.subDisplayFile() << "Leaving uqMarkovChainSGClass<P_V,P_M>::computeInitialCholFactors()"
                           << std::endl;
  }

  return iRC;
}

template<class P_V,class P_M>
void
uqMarkovChainSGClass<P_V,P_M>::updateTK()
{
  if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 5)) {
    *m_env.subDisplayFile() << "Entering uqMarkovChainSGClass<P_V,P_M>::updateTK()"
                           << std::endl;
  }

  if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 5)) {
    *m_env.subDisplayFile() << "In uqMarkovChainSGClass<P_V,P_M>::updateTK()"
                           << ": m_drMaxNumExtraStages = "                 << m_drMaxNumExtraStages
                           << ", m_drScalesForCovMatrices.size() = "       << m_drScalesForCovMatrices.size()
#ifdef UQ_USES_TK_CLASS
#else
                           << ", m_lowerCholProposalCovMatrices.size() = " << m_lowerCholProposalCovMatrices.size()
#endif
                           << std::endl;
  }

#ifdef UQ_USES_TK_CLASS
#else
  for (unsigned int i = 1; i < (m_drMaxNumExtraStages+1); ++i) {
    double scale = m_drScalesForCovMatrices[i];
    if (m_lowerCholProposalCovMatrices[i]) delete m_lowerCholProposalCovMatrices[i];
    m_lowerCholProposalCovMatrices [i]   = new P_M(*(m_lowerCholProposalCovMatrices[i-1]));
  *(m_lowerCholProposalCovMatrices [i]) /= scale;
    if (m_proposalCovMatrices[i]) delete m_proposalCovMatrices[i];
    m_proposalCovMatrices[i]             = new P_M(*(m_proposalCovMatrices[i-1]));
  *(m_proposalCovMatrices[i])           /= (scale*scale);
#ifdef UQ_MAC_SG_REQUIRES_INVERTED_COV_MATRICES
    m_upperCholProposalPrecMatrices[i]   = new P_M(*(m_upperCholProposalPrecMatrices[i-1]));
  *(m_upperCholProposalPrecMatrices[i]) *= scale;
    m_proposalPrecMatrices[i]            = new P_M(*(m_proposalPrecMatrices[i-1]));
  *(m_proposalPrecMatrices[i])          *= (scale*scale);
#endif
  }
#endif

  if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 5)) {
    *m_env.subDisplayFile() << "Leaving uqMarkovChainSGClass<P_V,P_M>::updateTK()"
                           << std::endl;
  }

  return;
}

template<class P_V,class P_M>
double
uqMarkovChainSGClass<P_V,P_M>::logProposal(
  const uqMarkovChainPositionDataClass<P_V>& x,
  const uqMarkovChainPositionDataClass<P_V>& y,
  unsigned int                               idOfProposalCovMatrix)
{
#ifdef UQ_USES_TK_CLASS
  return 0.;
#else
  P_V diffVec(y.vecValues() - x.vecValues());
#ifdef UQ_MAC_SG_REQUIRES_INVERTED_COV_MATRICES
  double value = -0.5 * scalarProduct(diffVec, *(m_proposalPrecMatrices[idOfProposalCovMatrix]) * diffVec);
#else
  double value = -0.5 * scalarProduct(diffVec, m_proposalCovMatrices[idOfProposalCovMatrix]->invertMultiply(diffVec));
#endif
  return value;
#endif
}

template<class P_V,class P_M>
double
uqMarkovChainSGClass<P_V,P_M>::logProposal(const std::vector<uqMarkovChainPositionDataClass<P_V>*>& inputPositions)
{
  unsigned int inputSize = inputPositions.size();
  UQ_FATAL_TEST_MACRO((inputSize < 2),
                      m_env.fullRank(),
                      "uqMarkovChainSGClass<P_V,P_M>::logProposal()",
                      "inputPositions has size < 2");

  return this->logProposal(*(inputPositions[0            ]),
                           *(inputPositions[inputSize - 1]),
                           inputSize-2);
}
#endif // UQ_USES_TK_CLASS

template<class P_V,class P_M>
double
uqMarkovChainSGClass<P_V,P_M>::alpha(
  const uqMarkovChainPositionDataClass<P_V>& x,
  const uqMarkovChainPositionDataClass<P_V>& y,
  unsigned int                               xStageId,
  unsigned int                               yStageId,
  double*                                    alphaQuotientPtr)
{
  double alphaQuotient = 0.;
  if ((x.outOfTargetSupport() == false) &&
      (y.outOfTargetSupport() == false)) {
    if ((x.logTarget() == -INFINITY) ||
        (x.logTarget() ==  INFINITY) ||
        (isnan(x.logTarget())      )) {
      std::cerr << "WARNING In uqMarkovChainSGClass<P_V,P_M>::alpha(x,y)"
                << ", fullRank "        << m_env.fullRank()
                << ", subEnvironment "  << m_env.subId()
                << ", subRank "         << m_env.subRank()
                << ", inter0Rank "      << m_env.inter0Rank()
                << ": x.logTarget() = " << x.logTarget()
                << ", x.values() = "    << x.vecValues()
                << ", y.values() = "    << y.vecValues()
                << std::endl;
    }
    else if ((y.logTarget() == -INFINITY) ||
             (y.logTarget() ==  INFINITY) ||
             (isnan(y.logTarget())      )) {
      std::cerr << "WARNING In uqMarkovChainSGClass<P_V,P_M>::alpha(x,y)"
                << ", fullRank "        << m_env.fullRank()
                << ", subEnvironment "  << m_env.subId()
                << ", subRank "         << m_env.subRank()
                << ", inter0Rank "      << m_env.inter0Rank()
                << ": y.logTarget() = " << y.logTarget()
                << ", x.values() = "    << x.vecValues()
                << ", y.values() = "    << y.vecValues()
                << std::endl;
    }
    else {
      double yLogTargetToUse = y.logTarget();
#ifdef UQ_MAC_SG_REQUIRES_TARGET_DISTRIBUTION_ONLY
#else
      if (m_likelihoodObjComputesMisfits &&
          m_observableSpace.shouldVariancesBeUpdated()) {
        // Divide the misfitVector of 'y' by the misfitVarianceVector of 'x'
        yLogTargetToUse = -0.5 * ( y.m2lPrior() + (y.misfitVector()/x.misfitVarianceVector()).sumOfComponents() );
      }
#endif
#ifdef UQ_USES_TK_CLASS
      if (m_tk->symmetric()) {
#else
      if (m_tkIsSymmetric) {
#endif
        alphaQuotient = std::exp(yLogTargetToUse - x.logTarget());
        if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 10)) {
          *m_env.subDisplayFile() << "In uqMarkovChainSGClass<P_V,P_M>::alpha(x,y)"
                                 << ": symmetric proposal case"
                                 << ", x = "               << x.vecValues()
                                 << ", y = "               << y.vecValues()
                                 << ", yLogTargetToUse = " << yLogTargetToUse
                                 << ", x.logTarget() = "   << x.logTarget()
                                 << ", alpha = "           << alphaQuotient
                                 << std::endl;
        }
      }
      else {
#ifdef UQ_USES_TK_CLASS // AQUI
        double qyx = -.5 * m_tk->rv(yStageId).pdf().minus2LnValue(x.vecValues(),NULL,NULL,NULL,NULL);
        if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 10)) {
          const uqGaussianJointPdfClass<P_V,P_M>* pdfYX = dynamic_cast< const uqGaussianJointPdfClass<P_V,P_M>* >(&(m_tk->rv(yStageId).pdf()));
          *m_env.subDisplayFile() << "In uqMarkovChainSGClass<P_V,P_M>::alpha(x,y)"
                                 << ", rvYX.domainExpVector = " << pdfYX->domainExpVector()
                                 << ", rvYX.domainVarVector = " << pdfYX->domainVarVector()
                                 << ", rvYX.covMatrix = "       << pdfYX->covMatrix()
                                 << std::endl;
        }
        double qxy = -.5 * m_tk->rv(xStageId).pdf().minus2LnValue(y.vecValues(),NULL,NULL,NULL,NULL);
        if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 10)) {
          const uqGaussianJointPdfClass<P_V,P_M>* pdfXY = dynamic_cast< const uqGaussianJointPdfClass<P_V,P_M>* >(&(m_tk->rv(xStageId).pdf()));
          *m_env.subDisplayFile() << "In uqMarkovChainSGClass<P_V,P_M>::alpha(x,y)"
                                 << ", rvXY.domainExpVector = " << pdfXY->domainExpVector()
                                 << ", rvXY.domainVarVector = " << pdfXY->domainVarVector()
                                 << ", rvXY.covMatrix = "       << pdfXY->covMatrix()
                                 << std::endl;
        }
#else
        double qyx = logProposal(y,x,0);
        double qxy = logProposal(x,y,0);
#endif
        alphaQuotient = std::exp(yLogTargetToUse +
                                 qyx -
                                 x.logTarget() -
                                 qxy);
        if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 10)) {
          *m_env.subDisplayFile() << "In uqMarkovChainSGClass<P_V,P_M>::alpha(x,y)"
                                 << ": unsymmetric proposal case"
                                 << ", xStageId = "        << xStageId
                                 << ", yStageId = "        << yStageId
                                 << ", x = "               << x.vecValues()
                                 << ", y = "               << y.vecValues()
                                 << ", yLogTargetToUse = " << yLogTargetToUse
                                 << ", q(y,x) = "          << qyx
                                 << ", x.logTarget() = "   << x.logTarget()
                                 << ", q(x,y) = "          << qxy
                                 << ", alpha = "           << alphaQuotient
                                 << std::endl;
        }
      }
    } // protection logic against logTarget values
  }
  else {
    if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 10)) {
      *m_env.subDisplayFile() << "In uqMarkovChainSGClass<P_V,P_M>::alpha(x,y)"
                             << ": x.outOfTargetSupport = " << x.outOfTargetSupport()
                             << ", y.outOfTargetSupport = " << y.outOfTargetSupport()
                             << std::endl;
    }
  }
  if (alphaQuotientPtr != NULL) *alphaQuotientPtr = alphaQuotient;

  return std::min(1.,alphaQuotient);
}

template<class P_V,class P_M>
double
uqMarkovChainSGClass<P_V,P_M>::alpha(
  const std::vector<uqMarkovChainPositionDataClass<P_V>*>& inputPositionsData,
  const std::vector<unsigned int                        >& inputTKStageIds)
{
  unsigned int inputSize = inputPositionsData.size();
  if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 10)) {
    *m_env.subDisplayFile() << "Entering uqMarkovChainSGClass<P_V,P_M>::alpha(vec)"
                           << ", inputSize = " << inputSize
                           << std::endl;
  }
  UQ_FATAL_TEST_MACRO((inputSize < 2),
                      m_env.fullRank(),
                      "uqMarkovChainSGClass<P_V,P_M>::alpha(vec)",
                      "inputPositionsData has size < 2");

  // If necessary, return 0. right away
  if (inputPositionsData[0          ]->outOfTargetSupport()) return 0.;
  if (inputPositionsData[inputSize-1]->outOfTargetSupport()) return 0.;

  if ((inputPositionsData[0]->logTarget() == -INFINITY) ||
      (inputPositionsData[0]->logTarget() ==  INFINITY) ||
      (isnan(inputPositionsData[0]->logTarget())      )) {
    std::cerr << "WARNING In uqMarkovChainSGClass<P_V,P_M>::alpha(vec)"
              << ", fullRank "       << m_env.fullRank()
              << ", subEnvironment " << m_env.subId()
              << ", subRank "        << m_env.subRank()
              << ", inter0Rank "     << m_env.inter0Rank()
              << ": inputSize = "    << inputSize
              << ", inputPositionsData[0]->logTarget() = " << inputPositionsData[0]->logTarget()
              << ", [0]->values() = "                      << inputPositionsData[0]->vecValues()
              << ", [inputSize - 1]->values() = "          << inputPositionsData[inputSize-1]->vecValues()
              << std::endl;
    return 0.;
  }
  else if ((inputPositionsData[inputSize - 1]->logTarget() == -INFINITY) ||
           (inputPositionsData[inputSize - 1]->logTarget() ==  INFINITY) ||
           (isnan(inputPositionsData[inputSize - 1]->logTarget())      )) {
    std::cerr << "WARNING In uqMarkovChainSGClass<P_V,P_M>::alpha(vec)"
              << ", fullRank "       << m_env.fullRank()
              << ", subEnvironment " << m_env.subId()
              << ", subRank "        << m_env.subRank()
              << ", inter0Rank "     << m_env.inter0Rank()
              << ": inputSize = "    << inputSize
              << ", inputPositionsData[inputSize - 1]->logTarget() = " << inputPositionsData[inputSize-1]->logTarget()
              << ", [0]->values() = "                                  << inputPositionsData[0]->vecValues()
              << ", [inputSize - 1]->values() = "                      << inputPositionsData[inputSize-1]->vecValues()
              << std::endl;
    return 0.;
  }

  // If inputSize is 2, recursion is not needed
  if (inputSize == 2) return this->alpha(*(inputPositionsData[0            ]),
                                         *(inputPositionsData[inputSize - 1]),
                                         inputTKStageIds[0],
                                         inputTKStageIds[inputSize-1]);

  // Prepare two vectors of positions
  std::vector<uqMarkovChainPositionDataClass<P_V>*>         positionsData  (inputSize,NULL);
  std::vector<uqMarkovChainPositionDataClass<P_V>*> backwardPositionsData  (inputSize,NULL);

  std::vector<unsigned int                        >         tkStageIds     (inputSize,0);
  std::vector<unsigned int                        > backwardTKStageIds     (inputSize,0);

  std::vector<unsigned int                        >         tkStageIdsLess1(inputSize,0);
  std::vector<unsigned int                        > backwardTKStageIdsLess1(inputSize,0);

  for (unsigned int i = 0; i < inputSize; ++i) {
            positionsData  [i] = inputPositionsData[i];
    backwardPositionsData  [i] = inputPositionsData[inputSize-i-1];

            tkStageIds     [i] = inputTKStageIds   [i];
    backwardTKStageIds     [i] = inputTKStageIds   [inputSize-i-1];

            tkStageIdsLess1[i] = inputTKStageIds   [i];
    backwardTKStageIdsLess1[i] = inputTKStageIds   [inputSize-i-1];
  }

          tkStageIdsLess1.pop_back();
  backwardTKStageIdsLess1.pop_back();

  // Initialize cumulative variables
  double logNumerator      = 0.;
  double logDenominator    = 0.;
  double alphasNumerator   = 1.;
  double alphasDenominator = 1.;

  // Compute cumulative variables
#ifdef UQ_USES_TK_CLASS // AQUI
  const P_V& _lastTKPosition         = m_tk->preComputingPosition(        tkStageIds[inputSize-1]);
  const P_V& _lastBackwardTKPosition = m_tk->preComputingPosition(backwardTKStageIds[inputSize-1]);

  double numContrib = -.5 * m_tk->rv(backwardTKStageIdsLess1).pdf().minus2LnValue(_lastBackwardTKPosition,NULL,NULL,NULL,NULL);
  double denContrib = -.5 * m_tk->rv(        tkStageIdsLess1).pdf().minus2LnValue(_lastTKPosition        ,NULL,NULL,NULL,NULL);
#else
  double numContrib = logProposal(backwardPositionsData);
  double denContrib = logProposal(        positionsData);
#endif
  if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 10)) {
    *m_env.subDisplayFile() << "In uqMarkovChainSGClass<P_V,P_M>::alpha(vec)"
                           << ", inputSize = "  << inputSize
                           << ", before loop"
                           << ": numContrib = " << numContrib
                           << ", denContrib = " << denContrib
                           << std::endl;
  }
  logNumerator   += numContrib;
  logDenominator += denContrib;

  for (unsigned int i = 0; i < (inputSize-2); ++i) { // That is why size must be >= 2
            positionsData.pop_back();
    backwardPositionsData.pop_back();

#ifdef UQ_USES_TK_CLASS // AQUI
    const P_V& lastTKPosition         = m_tk->preComputingPosition(        tkStageIds[inputSize-2-i]);
    const P_V& lastBackwardTKPosition = m_tk->preComputingPosition(backwardTKStageIds[inputSize-2-i]);

            tkStageIds.pop_back();
    backwardTKStageIds.pop_back();

            tkStageIdsLess1.pop_back();
    backwardTKStageIdsLess1.pop_back();

    numContrib = -.5 * m_tk->rv(backwardTKStageIdsLess1).pdf().minus2LnValue(lastBackwardTKPosition,NULL,NULL,NULL,NULL);
    denContrib = -.5 * m_tk->rv(        tkStageIdsLess1).pdf().minus2LnValue(lastTKPosition        ,NULL,NULL,NULL,NULL);
#else
    numContrib = logProposal(backwardPositionsData);
    denContrib = logProposal(        positionsData);
#endif
    if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 10)) {
      *m_env.subDisplayFile() << "In uqMarkovChainSGClass<P_V,P_M>::alpha(vec)"
                             << ", inputSize = "  << inputSize
                             << ", in loop, i = " << i
                             << ": numContrib = " << numContrib
                             << ", denContrib = " << denContrib
                             << std::endl;
    }
    logNumerator   += numContrib;
    logDenominator += denContrib;

    alphasNumerator   *= (1. - this->alpha(backwardPositionsData,backwardTKStageIds));
    alphasDenominator *= (1. - this->alpha(        positionsData,        tkStageIds));
  }

  double numeratorLogTargetToUse = backwardPositionsData[0]->logTarget();
#ifdef UQ_MAC_SG_REQUIRES_TARGET_DISTRIBUTION_ONLY
#else
  if (m_likelihoodObjComputesMisfits &&
      m_observableSpace.shouldVariancesBeUpdated()) {
    // Divide the misfitVector of 'back[0]' by the misfitVarianceVector of 'pos[0]'
    numeratorLogTargetToUse = -0.5 * ( backwardPositionsData[0]->m2lPrior() +
      (backwardPositionsData[0]->misfitVector()/positionsData[0]->misfitVarianceVector()).sumOfComponents() );
  }
#endif
  numContrib = numeratorLogTargetToUse;
  denContrib = positionsData[0]->logTarget();
  if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 10)) {
    *m_env.subDisplayFile() << "In uqMarkovChainSGClass<P_V,P_M>::alpha(vec)"
                           << ", inputSize = "  << inputSize
                           << ", after loop"
                           << ": numContrib = " << numContrib
                           << ", denContrib = " << denContrib
                           << std::endl;
  }
  logNumerator   += numContrib;
  logDenominator += denContrib;

  if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 10)) {
    *m_env.subDisplayFile() << "Leaving uqMarkovChainSGClass<P_V,P_M>::alpha(vec)"
                           << ", inputSize = "         << inputSize
                           << ": alphasNumerator = "   << alphasNumerator
                           << ", alphasDenominator = " << alphasDenominator
                           << ", logNumerator = "      << logNumerator
                           << ", logDenominator = "    << logDenominator
                           << std::endl;
  }

  // Return result
  return std::min(1.,(alphasNumerator/alphasDenominator)*std::exp(logNumerator-logDenominator));
}

template<class P_V,class P_M>
bool
uqMarkovChainSGClass<P_V,P_M>::acceptAlpha(double alpha)
{
  bool result = false;

  if      (alpha <= 0.                          ) result = false;
  else if (alpha >= 1.                          ) result = true;
  else if (alpha >= gsl_rng_uniform(m_env.rng())) result = true;
  else                                            result = false;

  return result;
}

template<class P_V,class P_M>
int
uqMarkovChainSGClass<P_V,P_M>::writeInfo(
  const uqBaseVectorSequenceClass<P_V,P_M>& workingChain,
  std::ofstream&                            ofsvar) const
//const P_M*                   mahalanobisMatrix,
//bool                         applyMahalanobisInvert) const
{
  if (m_env.subDisplayFile()) {
    *m_env.subDisplayFile() << "\n"
                           << "\n-----------------------------------------------------"
                           << "\n Writing extra information about the Markov chain " << workingChain.name() << " to output file ..."
                           << "\n-----------------------------------------------------"
                           << "\n"
                           << std::endl;
  }

  int iRC = UQ_OK_RC;

  if (m_rawChainGenerateExtra) {
    // Write m_logTargets
    ofsvar << m_prefix << "logTargets_sub" << m_env.subIdString() << " = zeros(" << m_logTargets.size()
           << ","                                                                << 1
           << ");"
           << std::endl;
    ofsvar << m_prefix << "logTargets_sub" << m_env.subIdString() << " = [";
    for (unsigned int i = 0; i < m_logTargets.size(); ++i) {
      ofsvar << m_logTargets[i]
             << std::endl;
    }
    ofsvar << "];\n";

    // Write m_alphaQuotients
    ofsvar << m_prefix << "alphaQuotients_sub" << m_env.subIdString() << " = zeros(" << m_alphaQuotients.size()
           << ","                                                                    << 1
           << ");"
           << std::endl;
    ofsvar << m_prefix << "alphaQuotients_sub" << m_env.subIdString() << " = [";
    for (unsigned int i = 0; i < m_alphaQuotients.size(); ++i) {
      ofsvar << m_alphaQuotients[i]
             << std::endl;
    }
    ofsvar << "];\n";
  }

  // Write names of components
  ofsvar << m_prefix << "componentNames = {";
  m_vectorSpace.printComponentsNames(ofsvar,false);
  ofsvar << "};\n";

#if 0
  // Write mahalanobis distances
  if (mahalanobisMatrix != NULL) {
    P_V diffVec(m_vectorSpace.zeroVector());
    ofsvar << m_prefix << "d = [";
    if (applyMahalanobisInvert) {
      P_V tmpVec(m_vectorSpace.zeroVector());
      P_V vec0(m_vectorSpace.zeroVector());
      workingChain.getPositionValues(0,vec0);
      for (unsigned int i = 0; i < workingChain.subSequenceSize(); ++i) {
        workingChain.getPositionValues(i,tmpVec);
        diffVec = tmpVec - vec0;
        //diffVec = *(workingChain[i]) - *(workingChain[0]);
        ofsvar << scalarProduct(diffVec, mahalanobisMatrix->invertMultiply(diffVec))
               << std::endl;
      }
    }
    else {
      P_V tmpVec(m_vectorSpace.zeroVector());
      P_V vec0(m_vectorSpace.zeroVector());
      workingChain.getPositionValues(0,vec0);
      for (unsigned int i = 0; i < workingChain.subSequenceSize(); ++i) {
        workingChain.getPositionValues(i,tmpVec);
        diffVec = tmpVec - vec0;
        //diffVec = *(workingChain[i]) - *(workingChain[0]);
        ofsvar << scalarProduct(diffVec, *mahalanobisMatrix * diffVec)
               << std::endl;
      }
    }
    ofsvar << "];\n";
  }
#endif

#if 0
  // Write prior mean values
  ofsvar << m_prefix << "priorMeanValues = ["
         << m_vectorSpace.priorMuValues()
         << "];\n";

  // Write prior sigma values
  ofsvar << m_prefix << "priorSigmaValues = ["
         << m_vectorSpace.priorSigmaValues()
         << "];\n";

#if 0
  ofsvar << m_prefix << "results.prior = [queso_priorMeanValues',queso_priorSigmaValues'];\n";
#endif

  // Write vector space lower bounds
  ofsvar << m_prefix << "minValues = ["
         << m_vectorSpace.minValues()
         << "];\n";

  // Write vector space upper bounds
  ofsvar << m_prefix << "maxValues = ["
         << m_vectorSpace.maxValues()
         << "];\n";
#endif

#if 0
  ofsvar << m_prefix << "results.limits = [queso_low',queso_upp'];\n";

  // Write out data for mcmcpred.m of MATLAB MCMC toolbox
  ofsvar << m_prefix << "results.parind = ["; // FIXME
  for (unsigned int i = 0; i < m_vectorSpace.dim(); ++i) {
    ofsvar << i+1
           << std::endl;
  }
  ofsvar << "];\n";

  ofsvar << m_prefix << "results.local = [\n"; // FIXME
  for (unsigned int i = 0; i < m_vectorSpace.dim(); ++i) {
    ofsvar << " 0";
    //<< std::endl;
  }
  ofsvar << "];\n";

  if (m_rawChainUse2) {
  }
  else {
    bool savedVectorPrintState = workingChain[workingChain.subSequenceSize()-1]->getPrintHorizontally();
    workingChain[workingChain.subSequenceSize()-1]->setPrintHorizontally(false);
    ofsvar << m_prefix << "results.theta = ["
        << *(workingChain[workingChain.subSequenceSize()-1])
        << "];\n";
    workingChain[workingChain.subSequenceSize()-1]->setPrintHorizontally(savedVectorPrintState);
  }
  
  ofsvar << m_prefix << "results.nbatch = 1.;\n"; // FIXME

  if (mahalanobisMatrix != NULL) {
    // Write covMatrix
    ofsvar << m_prefix << "mahalanobisMatrix = ["
           << *mahalanobisMatrix
           << "];\n";
  }
#endif

  // Write number of rejections
  ofsvar << m_prefix << "rejected = " << (double) m_numRejections/(double) (workingChain.subSequenceSize()-1)
         << ";\n"
         << std::endl;

  // Write number of out of target support
  ofsvar << m_prefix << "outTargetSupport = " << (double) m_numOutOfTargetSupport/(double) (workingChain.subSequenceSize()-1)
         << ";\n"
         << std::endl;

  // Write chain run time
  ofsvar << m_prefix << "runTime = " << m_rawChainRunTime
         << ";\n"
         << std::endl;

  if (m_env.subDisplayFile()) {
    *m_env.subDisplayFile() << "\n-----------------------------------------------------"
                                  << "\n Finished writing extra information about the Markov chain " << workingChain.name()
                                  << "\n-----------------------------------------------------"
                                  << "\n"
                                  << std::endl;
  }

  return iRC;
}

template<class P_V,class P_M>
void
uqMarkovChainSGClass<P_V,P_M>::print(std::ostream& os) const
{
  os <<         m_option_dataOutputFileName   << " = " << m_dataOutputFileName
     << "\n" << m_option_dataOutputAllowedSet << " = ";
  for (std::set<unsigned int>::iterator setIt = m_dataOutputAllowedSet.begin(); setIt != m_dataOutputAllowedSet.end(); ++setIt) {
    os << *setIt << " ";
  }
  os << "\n" << m_option_rawChain_type                 << " = " << m_rawChainType
     << "\n" << m_option_rawChain_dataInputFileName    << " = " << m_rawChainDataInputFileName
     << "\n" << m_option_rawChain_size                 << " = " << m_rawChainSize
     << "\n" << m_option_rawChain_generateExtra        << " = " << m_rawChainGenerateExtra
     << "\n" << m_option_rawChain_displayPeriod        << " = " << m_rawChainDisplayPeriod
     << "\n" << m_option_rawChain_measureRunTimes      << " = " << m_rawChainMeasureRunTimes
     << "\n" << m_option_rawChain_dataOutputFileName   << " = " << m_rawChainDataOutputFileName
     << "\n" << m_option_rawChain_dataOutputAllowedSet << " = ";
  for (std::set<unsigned int>::iterator setIt = m_rawChainDataOutputAllowedSet.begin(); setIt != m_rawChainDataOutputAllowedSet.end(); ++setIt) {
    os << *setIt << " ";
  }
  os << "\n" << m_option_rawChain_computeStats              << " = " << m_rawChainComputeStats
     << "\n" << m_option_filteredChain_generate             << " = " << m_filteredChainGenerate
     << "\n" << m_option_filteredChain_discardedPortion     << " = " << m_filteredChainDiscardedPortion
     << "\n" << m_option_filteredChain_lag                  << " = " << m_filteredChainLag
     << "\n" << m_option_filteredChain_dataOutputFileName   << " = " << m_filteredChainDataOutputFileName
     << "\n" << m_option_filteredChain_dataOutputAllowedSet << " = ";
  for (std::set<unsigned int>::iterator setIt = m_filteredChainDataOutputAllowedSet.begin(); setIt != m_filteredChainDataOutputAllowedSet.end(); ++setIt) {
    os << *setIt << " ";
  }
  os << "\n" << m_option_filteredChain_computeStats << " = " << m_filteredChainComputeStats
     << "\n" << m_option_mh_displayCandidates       << " = " << m_mhDisplayCandidates
     << "\n" << m_option_mh_putOutOfBoundsInChain   << " = " << m_mhPutOutOfBoundsInChain
     << "\n" << m_option_tk_useLocalHessian         << " = " << m_tkUseLocalHessian
     << "\n" << m_option_tk_useNewtonComponent      << " = " << m_tkUseNewtonComponent
     << "\n" << m_option_dr_maxNumExtraStages       << " = " << m_drMaxNumExtraStages
     << "\n" << m_option_dr_scalesForExtraStages    << " = ";
  for (unsigned int i = 0; i < m_drScalesForCovMatrices.size(); ++i) {
    os << m_drScalesForCovMatrices[i] << " ";
  }
  os << "\n" << m_option_am_initialNonAdaptInterval << " = " << m_amInitialNonAdaptInterval
     << "\n" << m_option_am_adaptInterval           << " = " << m_amAdaptInterval
     << "\n" << m_option_am_eta                     << " = " << m_amEta
     << "\n" << m_option_am_epsilon                 << " = " << m_amEpsilon
     << std::endl;

  return;
}

template<class P_V,class P_M>
std::ostream& operator<<(std::ostream& os, const uqMarkovChainSGClass<P_V,P_M>& obj)
{
  obj.print(os);

  return os;
}
#endif // __UQ_MAC_SG1_H__
