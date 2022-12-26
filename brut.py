def get_subtree_colors(node: int):
    subtree_colors = [colors[node - 1]]
    for child in children.get(node, []):
        subtree_colors += get_subtree_colors(child)

    return subtree_colors


def checkIfAlmostSingleColor(node: int):
    subtree_colors = get_subtree_colors(node)
    color_dict = {}
    color_count = 0
    repeating_colors = 0

    for color in subtree_colors:
        color_dict[color] = color_dict.get(color, 0) + 1

    for color, num in color_dict.items():
        if num > 1:
            repeating_colors += 1

        color_count += 1

    return color_count <= 2 and repeating_colors <= 1


def changeColor(node: int, color: int):
    colors[node - 1] = color


def run():
    global colors
    global children

    n, m = [int(x) for x in input().split()]
    tree_dsc = [int(x) for x in input().split()]
    colors = [int(x) for x in input().split()]
    children = {}

    for child, parent in enumerate(tree_dsc, start=2):
        if children.get(parent) is None:
            children[parent] = [child]
        else:
            children[parent].append(child)

    for i in range(m):
        input_line = input().split()
        if input_line[0] == "?":
            if checkIfAlmostSingleColor(int(input_line[1])):
                print("TAK")
            else:
                print("NIE")
        if input_line[0] == "z":
            changeColor(int(input_line[1]), int(input_line[2]))


run()
