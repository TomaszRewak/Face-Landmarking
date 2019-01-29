import os

def write_tree(path, file_name, tree):
    if not os.path.exists(path):
        os.makedirs(path)

    with open(os.path.join(path, file_name), 'w') as file:
        file.write("{0}\n".format(len(tree.tree_.feature)))

        #values = [tree.classes_[cardinalities[0].tolist().index(max(cardinalities[0]))] for cardinalities in tree.tree_.value];

        values = [
            values[0][0]
            for values in tree.tree_.value
        ];
        
        for line in [tree.tree_.feature,
            tree.tree_.threshold,
            tree.tree_.children_left,
            tree.tree_.children_right,
            values]:

            for value in line:
                file.write("{0} ".format(value))

            file.write("\n")

