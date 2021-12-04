#include "compressedmatrix.hpp"
#include "matrix.hpp"
#include "simplematch.hpp"

class MatchLines
{
public:
  // Single fixed treshold
  void fixedTreshMatch(Matrix &connection_matrix, CompressedMatrix &cm, int matLine, int start, int end, SimpleMatch &sm, double fixedMatchTreshold, int loop)
  {
    bool match = sm.match(connection_matrix, fixedMatchTreshold, start, end);
    if (match)
    {
      cm.setElement(matLine, sm.returnMatchingState(), fixedMatchTreshold);
      std::cout << "Found match for line " << matLine << " in loop " << loop << ", with matching nb " << sm.getNbMatch() << " and treshold " << fixedMatchTreshold << "\n";
    }
  }

  // Single avg treshold => jolie amélioration par rapport au fixed en nombre de tours de boucle pour trouver
  void avgTreshMatch(Matrix &connection_matrix, CompressedMatrix &cm, int matLine, int start, int end, SimpleMatch &sm, int loop)
  {
    double avgTreshold = connection_matrix.getAverageThreshold(matLine);
    //std::cout<<avgTreshold<<"\n";
    bool match = sm.match(connection_matrix, avgTreshold, start, end);
    if (match)
    {
      cm.setElement(matLine, sm.returnMatchingState(), avgTreshold);
      std::cout << "Found match for line " << matLine << " in loop " << loop << ", with matching nb " << sm.getNbMatch() << " and treshold " << avgTreshold << "\n";
    }
  }

  // Single avg treshold => jolie amélioration par rapport au fixed en nombre de tours de boucle pour trouver
  void avgTreshMatchApprox(Matrix &connection_matrix, CompressedMatrix &cm, int matLine, int start, int end, SimpleMatch &sm, int toleratedErrors, int loop)
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
  void stepTreshMatch(Matrix &connection_matrix, CompressedMatrix &cm, int matLine, int start, int end, SimpleMatch &sm, double tresholdStep, int loop)
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
  void avgStepTreshMatch(Matrix &connection_matrix, CompressedMatrix &cm, int matLine, int start, int end, SimpleMatch &sm, double tresholdStep, int maxDev, int loop)
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