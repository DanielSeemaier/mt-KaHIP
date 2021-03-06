/******************************************************************************
 * coarsening.h 
 *
 * Source of KaHIP -- Karlsruhe High Quality Partitioning.
 *
 *****************************************************************************/

#ifndef COARSENING_UU97ZBTR
#define COARSENING_UU97ZBTR

#include "data_structure/graph_access.h"
#include "data_structure/graph_hierarchy.h"
#include "partition_config.h"
#include "stop_rules/stop_rules.h"

#include <memory>

class coarsening {
public:
        coarsening ();
        virtual ~coarsening ();

        void perform_coarsening(const PartitionConfig & config, graph_access & G, graph_hierarchy & hierarchy);

private:
        std::unique_ptr<stop_rule> get_stop_rule(graph_access& G, PartitionConfig& config) {
                if (config.mode_node_separators) {
                        return std::make_unique<separator_simple_stop_rule>(config, G.number_of_nodes());
                } else {
                        if(config.stop_rule == STOP_RULE_SIMPLE) {
                                return  std::make_unique<simple_stop_rule>(config, G.number_of_nodes());
                        } else if (config.stop_rule == STOP_RULE_MULTIPLE_K) {
                                return  std::make_unique<multiple_k_stop_rule>(config, G.number_of_nodes());
                        } else if (config.stop_rule == STOP_RULE_MEM) {
                                return  std::make_unique<mem_stop_rule>(config, G.number_of_nodes());
                        } else if (config.stop_rule == STOP_RULE_MULTIPLE_K_STRONG_CONTRACTION) {
                                return  std::make_unique<multiple_k_strong_contraction>(config, G.number_of_nodes(), G.number_of_edges());
                        } else if (config.stop_rule == STOP_RULE_MULTIPLE_K_WITH_MATCHING) {
                                return std::make_unique<stop_rule_with_reset<multiple_k_stop_rule>>(
                                        config,
                                        [](PartitionConfig& config) {
                                                config.matching_type = CLUSTER_COARSENING;
                                        },
                                        G.number_of_nodes(), G.number_of_edges());
                        } else if (config.stop_rule == STOP_RULE_MULTIPLE_K_STRONG_CONTRACTION_WITH_MATCHING) {
                                return std::make_unique<double_stop_rule<multiple_k_stop_rule, multiple_k_strong_contraction>>(
                                        config,
                                        [](PartitionConfig& config) {
                                                config.matching_type = CLUSTER_COARSENING;
                                        },
                                        G.number_of_nodes(), G.number_of_edges());
                        } else {
                                return  std::make_unique<strong_stop_rule>(config, G.number_of_nodes());
                        }
                }
        }
};

#endif /* end of include guard: COARSENING_UU97ZBTR */
