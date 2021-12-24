#include "compressedmatrix.hpp"
#include "matrix.hpp"
#include "simplematch.hpp"

/**
 * @brief The MatchLines struct
 */
struct MatchLines {
  // Single fixed treshold
  /**
   * @brief fixedTreshMatch
   * @param connection_matrix
   * @param cm
   * @param matLine
   * @param start
   * @param end
   * @param sm
   * @param fixedMatchTreshold
   * @param loop
   */
  template<class Gen>
  void fixedTreshMatch(Matrix &connection_matrix, CompressedMatrix<Gen> &cm, int matLine, int start, int end, SimpleMatch<Gen> &sm, double fixedMatchTreshold, int loop)
  {
    bool match = sm.match(connection_matrix, fixedMatchTreshold, start, end);
    if (match)
    {
      cm.setElement(matLine, sm.returnMatchingState(), fixedMatchTreshold);
      std::cout << "Found match for line " << matLine << " in loop " << loop << ", with matching nb " << sm.getNbMatch() << " and treshold " << fixedMatchTreshold << "\n";
    }
  }

  // Single avg treshold => jolie amélioration par rapport au fixed en nombre de tours de boucle pour trouver
  /**
   * @brief avgTreshMatch
   * @param connection_matrix
   * @param cm
   * @param matLine
   * @param start
   * @param end
   * @param sm
   * @param loop
   */
  template<class Gen>
  void avgTreshMatch(Matrix &connection_matrix, CompressedMatrix<Gen> &cm, int matLine, int start, int end, SimpleMatch<Gen> &sm, int loop)
  {
    double avgTreshold = connection_matrix.getAverageThreshold(matLine);
    // std::cout<<avgTreshold<<"\n";
    bool match = sm.match(connection_matrix, avgTreshold, start, end);
    if (match)
    {
      cm.setElement(matLine, sm.returnMatchingState(), avgTreshold);
      std::cout << "Found match for line " << matLine << " in loop " << loop << ", with matching nb " << sm.getNbMatch() << " and treshold " << avgTreshold << "\n";
    }
  }

  // Single avg treshold => jolie amélioration par rapport au fixed en nombre de tours de boucle pour trouver
  template<class Gen>
  void avgTreshMatchApprox(Matrix &connection_matrix, CompressedMatrix<Gen> &cm, int matLine, int start, int end, SimpleMatch<Gen> &sm, int toleratedErrors, int loop)
  {
    double avgTreshold = connection_matrix.getAverageThreshold(matLine);
    //std::cout<<avgTreshold<<"\n";
    bool match = sm.approxMatch(connection_matrix, avgTreshold, start, end, toleratedErrors);
    if (match)
    {
      cm.setElement(matLine, sm.returnMatchingState(), avgTreshold);
      std::cout << "Found match for line " << matLine << " in loop " << loop << ", with matching nb " << sm.getNbMatch() << " and treshold " << avgTreshold << "\n";
    }
  }

  // Multiple fixed tresholds => bad results
  /**
   * @brief stepTreshMatch
   * @param connection_matrix
   * @param cm
   * @param matLine
   * @param start
   * @param end
   * @param sm
   * @param tresholdStep
   * @param loop
   */
  template<class Gen>
  void stepTreshMatch(Matrix &connection_matrix, CompressedMatrix<Gen> &cm, int matLine, int start, int end, SimpleMatch<Gen> &sm, double tresholdStep, int loop)
  {
    double curTresh = tresholdStep;
    bool match = false;
    while (curTresh < 1 && !match)
    {
      match = sm.match(connection_matrix, curTresh, start, end);
      if (match)
      {
        cm.setElement(matLine, sm.returnMatchingState(), curTresh);
        std::cout << "Found match for line " << matLine << " in loop " << loop << ", with matching nb " << sm.getNbMatch() << " and treshold " << curTresh << "\n";
      }
      curTresh = curTresh + tresholdStep;
    }
  }

  //Multiple around avg tresholds : may be useful to limit exploration when treshold is near 0 or 1, but just using avg treshold seems better anyway.
  /**
   * @brief avgStepTreshMatch
   * @param connection_matrix
   * @param cm
   * @param matLine
   * @param start
   * @param end
   * @param sm
   * @param tresholdStep
   * @param maxDev
   * @param loop
   */
  template<class Gen>
  void avgStepTreshMatch(Matrix &connection_matrix, CompressedMatrix<Gen> &cm, int matLine, int start, int end, SimpleMatch<Gen> &sm, double tresholdStep, int maxDev, int loop)
  {
    double avgTreshold = connection_matrix.getAverageThreshold(matLine);
    bool match = sm.match(connection_matrix, avgTreshold, start, end);
    if (match)
    {
      cm.setElement(matLine, sm.returnMatchingState(), avgTreshold);
      std::cout << "Found match for line " << matLine << " in loop " << loop << ", with matching nb " << sm.getNbMatch() << " and treshold " << avgTreshold << "\n";
    }
    else
    {
      //We only explore around avg treshold
      double curTreshMax = avgTreshold + tresholdStep;
      int curDev = 0;
      while (curTreshMax < 1 && !match && curDev <= maxDev)
      {
        match = sm.match(connection_matrix, curTreshMax, start, end);
        if (match)
        {
          cm.setElement(matLine, sm.returnMatchingState(), curTreshMax);
          std::cout << "Found match for line " << matLine << " in loop " << loop << ", with matching nb " << sm.getNbMatch() << " and treshold " << curTreshMax << "\n";
        }
        curTreshMax = curTreshMax + tresholdStep;
        curDev++ ;
      }

      double curTreshMin = avgTreshold - tresholdStep;
      curDev = 0 ;
      while (curTreshMax < 1 && curTreshMin > 0 && !match && curDev <= maxDev)
      {
        match = sm.match(connection_matrix, curTreshMin, start, end);
        if (match)
        {
          cm.setElement(matLine, sm.returnMatchingState(), curTreshMin);
          std::cout << "Found match for line " << matLine << " in loop " << loop << ", with matching nb " << sm.getNbMatch() << " and treshold " << curTreshMin << "\n";
        }
        curTreshMin = curTreshMin - tresholdStep;
        curDev++ ;
      }
    }
  }
};
