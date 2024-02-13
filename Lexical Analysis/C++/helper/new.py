from collections import defaultdict

class NfaNode:
    def __init__(self, state=-1):
        self.state = state
        self.acceptance = False
        self.transition = defaultdict(set)

class Nfa:
    def __init__(self, numStates):
        self.states = [NfaNode(i) for i in range(numStates)]
        self.initialState = 0

class DfaNode:
    def __init__(self):
        self.state = -1
        self.acceptance = False
        self.transition = {}

class Dfa:
    def __init__(self, initial):
        self.states = []
        self.initialState = initial

def epClosure(currentState, NFA):
    closure = set()
    stateStack = [currentState]
    visited = set()

    while stateStack:
        currentState = stateStack.pop()
        closure.add(currentState)
        visited.add(currentState)

        if 'e' in NFA[currentState].transition:
            for trans in NFA[currentState].transition['e']:
                if trans not in closure and trans not in visited:
                    closure.add(trans)
                    stateStack.append(trans)
                    visited.add(trans)

    return closure

def eNfatoNFA(eNFA):
    numStates = len(eNFA.states)
    NFA = Nfa(numStates)

    for state in eNFA.states:
        first = epClosure(state.state, eNFA.states)

        for symbol in ['0', '1']:
            second = set()

            for value in first:
                temp = eNFA.states[value].transition[symbol]
                if temp:
                    second.update(temp)

            ans = set()

            for value in second:
                arr = epClosure(value, eNFA.states)
                if arr:
                    ans.update(arr)

            NFA.states[state.state].transition[symbol] = ans
            NFA.states[state.state].acceptance = False

    for state in range(len(NFA.states)):
        isAccepting = False
        closure = epClosure(state, eNFA.states)
        for x in closure:
            if eNFA.states[x].acceptance:
                isAccepting = True

        NFA.states[state].acceptance = isAccepting

    return NFA

def nfaToDFA(NFA):
    DFA = Dfa(0)
    DFA.states.append(DfaNode())

    stateNumbers = {frozenset({0}): 0}
    visited = set()
    stateQueue = [frozenset({0})]

    while stateQueue:
        currentState = stateQueue.pop(0)
        visited.add(currentState)

        zeroTransition, oneTransition = set(), set()

        for state in currentState:
            for nextState in NFA.states[state].transition['0']:
                zeroTransition.add(nextState)
            for nextState in NFA.states[state].transition['1']:
                oneTransition.add(nextState)

        if zeroTransition not in visited:
            visited.add(zeroTransition)
            stateQueue.append(zeroTransition)
            DFA.states.append(DfaNode())
            stateNumbers[zeroTransition] = len(DFA.states) - 1

        if oneTransition not in visited:
            visited.add(oneTransition)
            stateQueue.append(oneTransition)
            DFA.states.append(DfaNode())
            stateNumbers[oneTransition] = len(DFA.states) - 1

        DFA.states[stateNumbers[currentState]].transition['0'] = stateNumbers[zeroTransition]
        DFA.states[stateNumbers[currentState]].transition['1'] = stateNumbers[oneTransition]

    for i in range(len(NFA.states)):
        if NFA.states[i].acceptance:
            for state in visited:
                if i in state:
                    DFA.states[stateNumbers[state]].acceptance = True

    return DFA

def main():
    eNFA = Nfa(5)
    eNFA.states[0].transition['e'] = {1, 2}
    eNFA.states[1].transition['0'] = {3}
    eNFA.states[2].transition['1'] = {3}
    eNFA.states[3].transition['1'] = {4}
    eNFA.states[4].acceptance = True

    NFA = eNfatoNFA(eNFA)
    DFA = nfaToDFA(NFA)

    return 0

if __name__ == "__main__":
    main()