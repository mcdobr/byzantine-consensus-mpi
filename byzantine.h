#ifndef BYZANTINE_BYZANTINE_H
#define BYZANTINE_BYZANTINE_H

enum node_type {
    BAD,
    GOOD
};

struct vote_result_majority {
    int decision;
    int count;
};


int reach_consensus(int process, int no_processes);
enum node_type get_node_type(int process, int no_processes);
void broadcast_to_all_except_self(int process, int no_processes, enum node_type node_type, int decision);
void broadcast_to_all_except_self_good(int process, int no_processes, int decision);
void broadcast_to_all_except_self_bad(int process, int no_processes);
void receive_from_all_except_self(int process, int no_processes, int *decisions);
struct vote_result_majority compute_majority(int no_processes, const int * decisions);
int receive_from_king(int king_process);
int get_number_of_failures(int no_processes);

#endif //BYZANTINE_BYZANTINE_H
