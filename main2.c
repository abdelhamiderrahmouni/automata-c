#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "DFA.h"

int main() {
    DFA* automata = create_automata();

    // Create states
    DFA_State* state0 = create_state(0, false, automata);
    DFA_State* state1 = create_state(1, false, automata);
    DFA_State* state2 = create_state(2, false, automata);
    DFA_State* state3 = create_state(3, false, automata);
    DFA_State* state4 = create_state(4, true, automata);

    // Create transitions
    generate_transitions("a", state0, state1);
    generate_transitions("a", state1, state2);
    generate_transitions("b", state2, state3);
    generate_transitions("b", state3, state4);
    generate_transitions("a", state4, state1);

    // Set the initial state of the automaton
    automata->initial_state = state0;

    // Read the word to test
    char wordToTest[100];
    printf("Enter the word to test: ");
    scanf("%s", wordToTest);

    // Test the word and print the result
    if (belongs_to_language(wordToTest, automata)) {
        printf("The word is recognized by the automaton.\n");
    } else {
        printf("The word is not recognized by the automaton.\n");
    }

    // Clean up allocated memory
    free_automata(automata);

    return 0;
}
