/******************************************************************************
 * coarsening_configurator.h 
 *
 * Source of KaHIP -- Karlsruhe High Quality Partitioning.
 *
 *****************************************************************************/

#ifndef COARSENING_CONFIGURATOR_8UJ78WYS
#define COARSENING_CONFIGURATOR_8UJ78WYS

#include "contraction.h"
#include "data_structure/graph_hierarchy.h"
#include "definitions.h"
#include "edge_rating/edge_ratings.h"
#include "matching/gpa/gpa_matching.h"
#include "matching/random_matching.h"
#include "partition/coarsening/matching/local_max.h"
#include "clustering/size_constraint_label_propagation.h"
#include "stop_rules/stop_rules.h"

class coarsening_configurator {
        public:
                coarsening_configurator( ) {};
                virtual ~coarsening_configurator() {};

                void configure_coarsening(const PartitionConfig & partition_config, 
                                          matching** edge_matcher, 
                                          unsigned level); 
};

inline void coarsening_configurator::configure_coarsening( const PartitionConfig & partition_config, 
                                                           matching** edge_matcher, 
                                                           unsigned level) {

        switch(partition_config.matching_type) {
                case MATCHING_RANDOM: 
                        *edge_matcher = new random_matching();
                        break; 
                case MATCHING_GPA:
                        *edge_matcher = new gpa_matching();
                        break;
                case MATCHING_RANDOM_GPA:
                        *edge_matcher = new gpa_matching();
                        break;
               case CLUSTER_COARSENING:
                        *edge_matcher = new size_constraint_label_propagation();
                        break;
                case MATCHING_SEQUENTIAL_LOCAL_MAX:
                        *edge_matcher = new parallel::local_max_matching();
                        break;
                case MATCHING_PARALLEL_LOCAL_MAX:
                        *edge_matcher = new parallel::local_max_matching();
                        break;

        }

        if( partition_config.matching_type == MATCHING_RANDOM_GPA && level < partition_config.aggressive_random_levels) {
                delete *edge_matcher;
                *edge_matcher = new random_matching();
        }  
}

#endif /* end of include guard: COARSENING_CONFIGURATOR_8UJ78WYS */
