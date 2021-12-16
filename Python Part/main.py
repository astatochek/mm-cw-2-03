import networkx as nx
import matplotlib.pyplot as plt
import numpy as np

n = 3
# 1 - Greedy, 2 - Ants, 3 - Branch and Bound
for alg in range(1, n+1):

    G = nx.MultiDiGraph()

    f = open('../file.txt', 'r')
    n, m = [int(x) for x in (f.readline()).split()]
    #print(n, m)
    must_nodes = [int(x) for x in (f.readline()).split()]
    #print(must_nodes)
    pos = {}
    for i in range(n):
        pos[i] = np.array([int(x) for x in (f.readline()).split()])
    #print(pos)
    number_of_edges = int(f.readline())
    #print(number_of_edges)

    other_edges = []
    for i in range(number_of_edges):
        other_edges.append(tuple([int(x) for x in f.readline().split()]))

    #print(other_edges)

    if alg == 2 or alg == 3:
        f.readline()
        f.readline()
    if alg == 3:
        f.readline()
        f.readline()

    path = [int(x) for x in f.readline().split()]
    #print(path)

    path_edges = [(path[i-1], path[i]) for i in range(1, len(path))]

    #print(path_edges)

    line = f.readline()

    f.close()


    for i in range(n):
        G.add_node(i)

    G.add_edges_from(other_edges)


    # pos = nx.spring_layout(G)  # positions for all nodes - seed for reproducibility

    labels = {}
    for i in range(n):
        labels[i] = str(i+1)
    # nodes
    nx.draw_networkx_nodes(G, pos, nodelist=must_nodes, node_color='#3f8af3', node_size=400)
    nx.draw_networkx_nodes(G, pos, nodelist=[i for i in range(n) if i not in must_nodes], node_color='#d2d2d2', node_size=400)

    # edges
    nx.draw_networkx_edges(G, pos, width=0.5, edge_color="#d2d2d2", alpha=0.2)
    nx.draw_networkx_edges(G, pos, edgelist=path_edges, width=1, edge_color="black", alpha=0.5)
    # nx.draw_networkx_edge_labels(G, pos, edge_labels=weights)

    # labels
    nx.draw_networkx_labels(G, pos, font_size=10, font_color="whitesmoke")

    ax = plt.gca()
    ax.margins(0.08)
    fig = plt.gcf()
    fig.set_size_inches(18.5, 10.5)
    fig.text(0.02, 0.01, line,
            color='black',
            fontsize=20)
    plt.axis("off")
    plt.tight_layout()
    plt.show()


