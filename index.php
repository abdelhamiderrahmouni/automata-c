<?php

class DFA_State
{
    public int $state_identifier;
    public bool $accept_state;
    public int $transitions_count;
    public array $transitions = [];
}

class DFA_Transition
{
    public ?DFA_State $origin_state = null;
    public ?DFA_State $destination_state = null;
    public string $trigger_value;
}

class DFA
{
    public int $states_count;
    public array $states; // DFA_State
    public DFA_State $current_state;
    public ?DFA_State $initial_state = null;
}

function init_automata(DFA $automata): void
{
    if (!$automata->initial_state) {
        printf("[-] Error: Initial state of automata not set.\n");
        exit(1);
    }

    $automata->current_state = $automata->initial_state;
}

function update_automata(string $ch, DFA $automata): bool
{
    $state = $automata->current_state;
    $made_transition = false;

    foreach ($state->transitions as $transition) {
        if ($transition->trigger_value == $ch) {
            $automata->current_state = $transition->destination_state;
            $made_transition = true;
            break;
        }
    }

    return $made_transition;
}

function belongs_to_language(string $str, DFA $automata): bool
{
    if (strlen($str) == 0) {
        printf("[-] Error: Empty string sent to analyse in belongs_to_language.\n");
        exit(1);
    }

    init_automata($automata);

    for ($i = 0; $i < strlen($str); $i++) {
        if (!update_automata($str[$i], $automata)) {
            return false;
        }
    }

    return $automata->current_state->accept_state;
}

function set_transition_to_state(DFA_Transition $transition, DFA_State $origin_state): void
{
    $origin_state->transitions[] = $transition;
}

function set_state_to_automata(DFA_State $state, DFA $automata): void
{
    $automata->states[] = $state;
}

function create_state(int $state_identifier, bool $accept_state, DFA $automata): DFA_State
{
    $state = new DFA_State();

    $state->accept_state = $accept_state;
    $state->transitions_count = 0;
    $state->state_identifier = $state_identifier;
    $state->transitions = [];

    set_state_to_automata($state, $automata);

    return $state;
}

function create_transition(string $trigger_value, DFA_State $origin_state, ?DFA_State $destination_state = null): DFA_Transition
{
    $transition = new DFA_Transition();

    $transition->trigger_value = $trigger_value;
    $transition->origin_state = $origin_state;
    $transition->destination_state = $destination_state;
    set_transition_to_state($transition, $origin_state);

    return $transition;
}

function generate_transitions(string $string, DFA_State $origin_state, ?DFA_State $destination_state = null): bool
{
    for ($i = 0; $i < strlen($string); $i++) {
        if (!create_transition($string[$i], $origin_state, $destination_state)) {
            return false;
        }
    }

    return true;
}

function create_automata(): DFA
{
    $automata = new DFA();
    $automata->states_count = 0;
    $automata->initial_state = null;
    $automata->states = [];

    return $automata;
}

// Debug functions

function describe_transition(DFA_Transition $transition): void
{
    printf("------ Origin state: %p\n", $transition->origin_state);
    printf("------ Destination state: %p\n", $transition->destination_state);
    printf("------ Trigger value: %c\n", $transition->trigger_value);
}

function describe_state(DFA_State $state): void
{
    printf("--- Accept state: %d\n", $state->accept_state);

    if ($state->transitions_count > 0) {
        printf("--:: Transitions dump (%d):\n", $state->transitions_count);
        foreach ($state->transitions as $i => $transition) {
            printf("------ Transition %d\n", $i);
            describe_transition($transition);
        }
    } else {
        printf("--:: State has no transitions to dump.\n");
    }
}

function describe_automata(DFA $automata): void
{
    printf("Automata dump:\n");
    if ($automata->states_count > 0) {
        printf(":: States dump (%d):\n", $automata->states_count);
        foreach ($automata->states as $i => $state) {
            printf(":: State %d\n", $i);
            describe_state($state);
        }
    } else {
        printf("--- Automata has no states to dump.\n");
    }
}

function main(): void
{
    $automata = create_automata();
    $initial_state = create_state(0, true, $automata);

    $INITIAL_STATE = 0;
    $ZERO_STATE = 1;
    $ONE_STATE = 2;
    $ACCEPT_STATE = 3;
    $initial_state = create_state($INITIAL_STATE, false, $automata);
    $zero_state = create_state($ZERO_STATE, false, $automata);
    $one_state = create_state($ONE_STATE, false, $automata);
    $accept_state = create_state($ACCEPT_STATE, true, $automata);

    // Generation of transitions
    generate_transitions("a-", $initial_state, $initial_state);
    generate_transitions("0", $initial_state, $zero_state);
    generate_transitions("1", $initial_state, $one_state);
    generate_transitions("0", $one_state, $zero_state);
    generate_transitions("1", $zero_state, $one_state);
    generate_transitions("+", $zero_state, $accept_state);
    generate_transitions("%", $one_state, $accept_state);

    // Set the initial state of the automaton
    $automata->initial_state = $initial_state;

    // Examples of words to test
    $word1 = "a-aaaa--0101%";
    $word2 = "100101011";

    // Check if the words belong to the language
    if (belongs_to_language($word1, $automata)) {
        printf("%s belongs to the language.\n", $word1);
    } else {
        printf("%s does not belong to the language.\n", $word1);
    }

    if (belongs_to_language($word2, $automata)) {
        printf("%s belongs to the language.\n", $word2);
    } else {
        printf("%s does not belong to the language.\n", $word2);
    }
}

main();
