#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct DFA_State {
    int state_identifier;
    bool accept_state;
    int transitions_count;
    struct DFA_Transition* transitions;
} DFA_State;

typedef struct DFA_Transition {
    struct DFA_State* origin_state;
    struct DFA_State* destination_state;
    char trigger_value;
} DFA_Transition;

typedef struct DFA {
    int states_count;
    struct DFA_State** states;
    struct DFA_State* current_state;
    struct DFA_State* initial_state;
} DFA;

void init_automata(DFA* automata) {
    if (!automata->initial_state) {
        printf("[-] Error: Initial state of automata not set.\n");
        exit(1);
    }

    automata->current_state = automata->initial_state;
}

bool update_automata(char ch, DFA* automata) {
    DFA_State* state = automata->current_state;
    bool made_transition = false;

    for (int j = 0; j < state->transitions_count; j++) {
        if (state->transitions[j].trigger_value == ch) {
            automata->current_state = state->transitions[j].destination_state;
            made_transition = true;
            break;
        }
    }

    return made_transition;
}

bool belongs_to_language(const char* str, DFA* automata) {
    if (str[0] == '\0') {
        printf("[-] Error: Empty string sent to analyse in belongs_to_language.\n");
        exit(1);
    }

    init_automata(automata);

    for (int i = 0; str[i] != '\0'; i++) {
        if (!update_automata(str[i], automata)) {
            return false;
        }
    }

    return automata->current_state->accept_state;
}

void set_transition_to_state(DFA_Transition* transition, DFA_State* origin_state) {
    origin_state->transitions[origin_state->transitions_count++] = *transition;
}

void set_state_to_automata(DFA_State* state, DFA* automata) {
    automata->states[automata->states_count++] = state;
}

DFA_State* create_state(int state_identifier, bool accept_state, DFA* automata) {
    DFA_State* state = (DFA_State*)malloc(sizeof(DFA_State));

    state->accept_state = accept_state;
    state->transitions_count = 0;
    state->state_identifier = state_identifier;
    state->transitions = NULL;
    
    set_state_to_automata(state, automata);

    return state;
}

DFA_Transition* create_transition(char trigger_value, DFA_State* origin_state, DFA_State* destination_state) {
    DFA_Transition* transition = (DFA_Transition*)malloc(sizeof(DFA_Transition));

    transition->trigger_value = trigger_value;
    transition->origin_state = origin_state;
    transition->destination_state = destination_state;
    set_transition_to_state(transition, origin_state);

    return transition;
}

bool generate_transitions(const char* string, DFA_State* origin_state, DFA_State* destination_state) {
    for (int i = 0; string[i] != '\0'; i++) {
        DFA_Transition* transition = create_transition(string[i], origin_state, destination_state);
        if (transition == NULL) {
            return false;
        }
    }

    return true;
}

DFA* create_automata() {
    DFA* automata = (DFA*)malloc(sizeof(DFA));
    automata->states_count = 0;
    automata->initial_state = NULL;
    automata->states = NULL;

    return automata;
}

// Debug functions

void describe_transition(DFA_Transition* transition) {
    printf("------ Origin state: %p\n", transition->origin_state);
    printf("------ Destination state: %p\n", transition->destination_state);
    printf("------ Trigger value: %c\n", transition->trigger_value);
}

void describe_state(DFA_State* state) {
    printf("--- Accept state: %d\n", state->accept_state);

    if (state->transitions_count > 0) {
        printf("--:: Transitions dump (%d):\n", state->transitions_count);
        for (int i = 0; i < state->transitions_count; i++) {
            printf("------ Transition %d\n", i);
            describe_transition(&state->transitions[i]);
        }
    } else {
        printf("--:: State has no transitions to dump.\n");
    }
}

void describe_automata(DFA* automata) {
    printf("Automata dump:\n");
    if (automata->states_count > 0) {
        printf(":: States dump (%d):\n", automata->states_count);
        for (int i = 0; i < automata->states_count; i++) {
            printf(":: State %d\n", i);
            describe_state(automata->states[i]);
        }
    } else {
        printf("--- Automata has no states to dump.\n");
    }
}

int main() {
    DFA* automata = create_automata();
    DFA_State* initial_state = create_state(0, true, automata);

    int INITIAL_STATE = 0;
    int ZERO_STATE = 1;
    int ONE_STATE = 2;
    int ACCEPT_STATE = 3;
    initial_state = create_state(INITIAL_STATE, false, automata);
    DFA_State* zero_state = create_state(ZERO_STATE, false, automata);
    DFA_State* one_state = create_state(ONE_STATE, false, automata);
    DFA_State* accept_state = create_state(ACCEPT_STATE, true, automata);

    // Generation of transitions
    generate_transitions("a-", initial_state, initial_state);
    generate_transitions("0", initial_state, zero_state);
    generate_transitions("1", initial_state, one_state);
    generate_transitions("0", one_state, zero_state);
    generate_transitions("1", zero_state, one_state);
    generate_transitions("+", zero_state, accept_state);
    generate_transitions("%", one_state, accept_state);

    // Set the initial state of the automaton
    automata->initial_state = initial_state;

    // Examples of words to test
    const char* word1 = "a-aaaa--0101%";
    const char* word2 = "100101011";

    // Check if the words belong to the language
    if (belongs_to_language(word1, automata)) {
        printf("%s belongs to the language.\n", word1);
    } else {
        printf("%s does not belong to the language.\n", word1);
    }

    if (belongs_to_language(word2, automata)) {
        printf("%s belongs to the language.\n", word2);
    } else {
        printf("%s does not belong to the language.\n", word2);
    }

    // Clean up allocated memory
    free(automata);
    free(initial_state);
    free(zero_state);
    free(one_state);
    free(accept_state);

    return 0;
}
