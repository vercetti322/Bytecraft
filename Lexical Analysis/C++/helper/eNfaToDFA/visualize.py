import networkx as nx
import matplotlib.pyplot as plt

def draw_dfa(states, alphabet, transition_function, start_state, accept_states):
    G = nx.DiGraph()
    G.add_nodes_from(states)
    for state in states:
        for letter in alphabet:
            G.add_edge(state, transition_function[state][letter], label=letter)

    pos = nx.spring_layout(G)
    plt.figure(figsize=(10,10))

    nx.draw_networkx_edges(G, pos, arrowstyle='->', arrowsize=30)
    nx.draw_networkx_labels(G, pos, font_size=20)
    edge_labels = {(u, v): G[u][v]['label'] for u, v in G.edges()}
    nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels, font_size=15)

    nx.draw_networkx_nodes(G, pos, nodelist=accept_states, node_color='lightblue', node_size=1500)
    nx.draw_networkx_nodes(G, pos, nodelist=[start_state], node_color='yellow', node_size=1500)
    nx.draw_networkx_nodes(G, pos, nodelist=[s for s in states if s not in accept_states and s != start_state], node_color='lightgray', node_size=1500)

    plt.show()

states = {0, 1, 2, 3}
alphabet = {'0', '1'}
transition_function = {0: {'0': 1, '1': 2}, 1: {'0': 1, '1': 1}, 2: {'0': 3, '1': 2}, 3: {'0': 3, '1': 1}}
start_state = 0
accept_states = {0, 2}

draw_dfa(states, alphabet, transition_function, start_state, accept_states)