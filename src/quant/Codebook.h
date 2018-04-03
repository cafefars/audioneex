/*
  Copyright (c) 2014, Alberto Gramaglia

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

*/

#ifndef CODEBOOK_H
#define CODEBOOK_H

#include <cstdint>
#include <vector>
#include <string>
#include <memory>

#include "BinaryVector.h"
#include "Fingerprint.h"

namespace Audioneex
{

// -------- Internal exceptions introduced by the following classes -------

class InvalidAudioCodesException : public Audioneex::Exception {
  public: explicit InvalidAudioCodesException(const std::string& msg) :
                   Audioneex::Exception(msg) {}
};


struct Cluster
{
    uint32_t     ID;
    float        SumD;       // Sum of distances from points in cluster to centroid
    uint32_t     Npoints;    // Number of pints in the cluster
    BinaryVector Centroid;

    std::vector< std::pair<int,int> > Points;  //< TESTING STUFFS TO BE REMOVED

    Cluster() : ID(-1), SumD(0.0f), Npoints(0) {}
};


class Codebook
{
    std::vector<Cluster>  m_Clusters;

  public:

    typedef struct QResults_t
    {
        int word;
        int dist;
        QResults_t() : word(-1), dist(-1) {}
    }
    QResults;

    Codebook() {}
    ~Codebook() {}

    void set(std::vector<Cluster>  &clusters)  { m_Clusters = clusters; }
    const std::vector<Cluster>& get() const    { return m_Clusters; }
    void put(Cluster& cluster)                 { m_Clusters.push_back(cluster); }
    const Cluster& get(int word) const         { return m_Clusters[word]; }
    size_t size() const                        { return m_Clusters.size(); }

    /// Deserialize a Codebook object from a raw byte array
    static std::unique_ptr <Codebook> deserialize(const uint8_t* data, size_t data_size);
    /// Serialize a Codebook object into a raw byte array
    static void serialize(const Codebook& cbook, std::vector<uint8_t> &data);
    /// Save a codebook to a file
    static void Save(const Codebook &cbook, const std::string &filename);
    /// Load a codebook from a file
    static std::unique_ptr <Codebook> Load(const std::string &filename);

    QResults  quantize(LocalFingerprint_t &lf);

    void FindDuplicates();
    void Analyze();

};


// Clusters comparator
inline bool operator==(const Cluster &cluster1, const Cluster &cluster2){
    return (cluster1.ID == cluster2.ID &&
            cluster1.SumD == cluster2.SumD &&
            cluster1.Npoints == cluster2.Npoints &&
            cluster1.Centroid == cluster2.Centroid);
}

}// end namespace Audioneex

#endif
