// --------------------------------------------------------------------------
//                   OpenMS -- Open-Source Mass Spectrometry
// --------------------------------------------------------------------------
// Copyright The OpenMS Team -- Eberhard Karls University Tuebingen,
// ETH Zurich, and Freie Universitaet Berlin 2002-2015.
//
// This software is released under a three-clause BSD license:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of any author or any participating institution
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// For a full list of authors, refer to the file AUTHORS.
// --------------------------------------------------------------------------
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL ANY OF THE AUTHORS OR THE CONTRIBUTING
// INSTITUTIONS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// --------------------------------------------------------------------------
// $Maintainer: Hendrik Weisser $
// $Authors: Sven Nahnsen, Hendrik Weisser $
// --------------------------------------------------------------------------

#include <OpenMS/ANALYSIS/ID/ConsensusIDAlgorithmBest.h>
#include <OpenMS/CONCEPT/LogStream.h>

#include <cmath>

using namespace std;

#define DEBUG_ID_CONSENSUS
#undef  DEBUG_ID_CONSENSUS

namespace OpenMS
{
  ConsensusIDAlgorithmBest::ConsensusIDAlgorithmBest()
  {
    setName("ConsensusIDAlgorithmBest"); // DefaultParamHandler
  }


  void ConsensusIDAlgorithmBest::apply_(vector<PeptideIdentification>& ids)
  {
    SequenceGrouping grouping;
    groupHits_(ids, grouping);
    
    String score_type = ids[0].getScoreType();
    bool higher_better = ids[0].isHigherScoreBetter();
    ids.clear();
    ids.resize(1);
    ids[0].setScoreType(String("Consensus_best (") + score_type + ")");
    ids[0].setHigherScoreBetter(higher_better);
    for (SequenceGrouping::iterator group_it = grouping.begin(); 
         group_it != grouping.end(); ++group_it)
    {
      PeptideHit hit;
      hit.setSequence(group_it->first);
      if (higher_better)
      {
        hit.setScore(*max_element(group_it->second.second.begin(),
                                  group_it->second.second.end()));
      }
      else
      {
        hit.setScore(*min_element(group_it->second.second.begin(),
                                  group_it->second.second.end()));
      }
      hit.setCharge(group_it->second.first);
      ids[0].insertHit(hit);
#ifdef DEBUG_ID_CONSENSUS
      LOG_DEBUG << " - Output hit: " << hit.getSequence() << " "
                << hit.getScore() << endl;
#endif
    }
  }

} // namespace OpenMS
