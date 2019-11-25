#include <stdlib.h>
#include <mpi.h>
#include "byzantine.h"

const int NO_ACTION = -1;
const int ATTACK_ACTION = 1;
const int RETREAT_ACTION = 2;

const int DECISION_TAG = 1;


int reach_consensus(const int process, const int no_processes) {
    const int no_failures = get_number_of_failures(no_processes);

    enum node_type node_type = get_node_type(process, no_processes);
    int decision = ATTACK_ACTION;
    int *decisions = malloc(no_processes * sizeof(int));

    for (int king = 0; king < no_failures; ++king) {
        broadcast_to_all_except_self(process, no_processes, node_type, decision);
        decisions[process] = decision;

        receive_from_all_except_self(process, no_processes, decisions);

        struct vote_result_majority vote_result;
        vote_result = compute_majority(no_processes, decisions);

        int kings_decision;
        if (process == king) {
            kings_decision = vote_result.decision;
            broadcast_to_all_except_self(process, no_processes, node_type, kings_decision);
        } else {
            kings_decision = receive_from_king(king);
        }

        if (vote_result.count > no_processes / 2 + no_failures) {
            decision = vote_result.decision;
        } else {
            decision = kings_decision;
        }
    }
    free(decisions);

    return decision;
}

enum node_type get_node_type(const int process, const int no_processes) {
    const int NO_FAILURES = get_number_of_failures(no_processes);

    enum node_type state;
    if (process < NO_FAILURES) {
        state = BAD;
    } else {
        state = GOOD;
    }

    return state;
}

void broadcast_to_all_except_self(const int process, const int no_processes, const enum node_type node_type, int decision) {
    if (node_type == GOOD) {
        broadcast_to_all_except_self_good(process, no_processes, decision);
    } else if (node_type == BAD) {
        broadcast_to_all_except_self_bad(process, no_processes);
    }
}

void broadcast_to_all_except_self_good(const int process, const int no_processes, const int decision) {
    int planned_action = NO_ACTION;

    for (int dest_process = 0; dest_process < no_processes; ++dest_process) {
        if (dest_process != process) {
            MPI_Send(&planned_action, 1, MPI_INT, dest_process, DECISION_TAG, MPI_COMM_WORLD);
        }
    }
}

void broadcast_to_all_except_self_bad(const int process, const int no_processes) {

    for (int dest_process = 0; dest_process < no_processes; ++dest_process) {
        if (dest_process != process) {
            int planned_action = 0; //get_random_action();
            MPI_Send(&planned_action, 1, MPI_INT, dest_process, DECISION_TAG, MPI_COMM_WORLD);
        }
    }
}

void receive_from_all_except_self(const int process, const int no_processes, int *decisions) {
    MPI_Status status;

    for (int sender = 0; sender < no_processes; ++sender) {
        if (sender != process) {
            int value;
            MPI_Recv(&value, 1, MPI_INT, MPI_ANY_SOURCE, DECISION_TAG, MPI_COMM_WORLD, &status);
            decisions[status.MPI_SOURCE] = value;
        }
    }
}

struct vote_result_majority compute_majority(const int no_processes, const int * const decisions) {
    // TODO: compute majority by passing once
    // pass once more for getting the count

    struct vote_result_majority vote_result;
    return vote_result;
}


int receive_from_king(int king_process) {
    int value;
    MPI_Status status;
    MPI_Recv(&value, 1, MPI_INT, king_process, DECISION_TAG, MPI_COMM_WORLD, &status);
    return value;
}

int get_number_of_failures(const int no_processes) {
    return no_processes / 4;
}