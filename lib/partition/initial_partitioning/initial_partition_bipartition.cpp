/******************************************************************************
 * initial_partition_bipartition.cpp 
 *
 * Source of KaHIP -- Karlsruhe High Quality Partitioning.
 *
 ******************************************************************************
 * Copyright (C) 2013-2015 Christian Schulz <christian.schulz@kit.edu>
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *****************************************************************************/

#include <fstream>
#include "initial_partition_bipartition.h"
#include "uncoarsening/refinement/kway_graph_refinement/kway_graph_refinement.h"
#include "uncoarsening/refinement/mixed_refinement.h"
#include "graph_partitioner.h"

initial_partition_bipartition::initial_partition_bipartition() {

}

initial_partition_bipartition::~initial_partition_bipartition() {

}

void initial_partition_bipartition::initial_partition( const PartitionConfig & config,
                                                       const unsigned int seed,  
                                                       graph_access & G, int* partition_map) {
        graph_partitioner gp;
        PartitionConfig rec_config                  = config;
        rec_config.initial_partitioning_type        = INITIAL_PARTITIONING_BIPARTITION;
        rec_config.initial_partitioning_repetitions = 0;
        rec_config.global_cycle_iterations          = 1;
        rec_config.use_wcycles                      = false;
        rec_config.use_fullmultigrid                = false;
        rec_config.fm_search_limit                  = config.bipartition_post_ml_limits;
        rec_config.matching_type                    = MATCHING_GPA;
        rec_config.permutation_quality              = PERMUTATION_QUALITY_GOOD;
        rec_config.initial_partitioning             = true;
	rec_config.graph_allready_partitioned       = false;
        rec_config.label_propagation_refinement     = false;

        if( config.cluster_coarsening_during_ip == true) {
                rec_config.matching_type             = CLUSTER_COARSENING;
                rec_config.cluster_coarsening_factor = 12;
                rec_config.ensemble_clusterings      = false;
        }

        // turn off parallel mode
        rec_config.parallel_multitry_kway = false;
        rec_config.parallel_initial_partitioning = false;
        rec_config.parallel_lp = false;
        rec_config.parallel_coarsening_lp = false;
        rec_config.lp_before_local_search = false;
        rec_config.fast_contract_clustering = false;
        //rec_config.accept_small_coarser_graphs = true;

        // turn off common_neighborhood_clustering
        rec_config.common_neighborhood_clustering = false;

        if (rec_config.fastmultitry) {
                rec_config.fastmultitry = false;
                rec_config.quotient_graph_refinement_disabled = false;
        }

        if (rec_config.stop_rule == STOP_RULE_MEM || rec_config.stop_rule == STOP_RULE_MULTIPLE_K_STRONG_CONTRACTION) {
                rec_config.stop_rule = STOP_RULE_MULTIPLE_K;
        }

        std::streambuf* backup;
        std::ofstream ofs;
        if (!config.parallel_initial_partitioning) {
                backup = std::cout.rdbuf();
                ofs.open("/dev/null");
                std::cout.rdbuf(ofs.rdbuf());
        }

        gp.perform_recursive_partitioning(rec_config, G);

        if (!config.parallel_initial_partitioning) {
                ofs.close();
                std::cout.rdbuf(backup);
        }

        forall_nodes(G, n) {
                partition_map[n] =  G.getPartitionIndex(n);
        } endfor

} 

void initial_partition_bipartition::initial_partition( const PartitionConfig & config, 
                                                       const unsigned int seed,  
                                                       graph_access & G, 
                                                       int* xadj,
                                                       int* adjncy, 
                                                       int* vwgt, 
                                                       int* adjwgt,
                                                       int* partition_map) {

        std::cout <<  "not implemented yet"  << std::endl;
}

