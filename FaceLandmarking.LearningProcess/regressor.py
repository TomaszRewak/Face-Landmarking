import numpy as np
import shutil
import pathlib
from sklearn import tree
from readers.regressor_example_reader import read_regressor_examples
from writers.tree_writer import write_tree

directory = '../Data/regressors/trees'
shutil.rmtree(directory, ignore_errors=True)
pathlib.Path(directory).mkdir(parents=True)

print('loading...')

data = read_regressor_examples(15, 2, '../Data/features')

print('computing...')

for point, examples in data.items():
    x = examples['x'][1000:]
    y = examples['y'][1000:]

    x = np.array(x)
    y = [[max(-3, min(3, element)) for element in line] for line in y]

    y1 = np.array(y)[:,0]
    y2 = np.array(y)[:,1]
    
    tree1 = tree.DecisionTreeRegressor(min_samples_leaf=5000)
    tree2 = tree.DecisionTreeRegressor(min_samples_leaf=5000)

    tree1.fit(x, y1)
    tree2.fit(x, y2) 

    # save
    
    path = '{0}/{1}'.format(directory, point)

    write_tree(path, 'x', tree1)
    write_tree(path, 'y', tree2)