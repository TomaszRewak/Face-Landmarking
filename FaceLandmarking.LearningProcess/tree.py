import numpy as np
import shutil
import pathlib
from sklearn import tree
from data_reader import import_face_data
from tree_writer import write_tree

directory = '../Data/regressors/trees'
shutil.rmtree(directory, ignore_errors=True)
pathlib.Path(directory).mkdir(parents=True)

print('loading...')

data = import_face_data(15, 2, '../Data/features')

print('computing...')

for point, examples in data.items():
    x = examples['x'][1000:]
    y = examples['y'][1000:]

    x = np.array(x)
    y = [[round(max(-1, min(1, element))) for element in line] for line in y]

    y1 = np.array(y)[:,0]
    y2 = np.array(y)[:,1]
    
    tree1 = tree.DecisionTreeClassifier(min_samples_leaf=500, criterion ='entropy')
    tree2 = tree.DecisionTreeClassifier(min_samples_leaf=500, criterion ='entropy')

    tree1.fit(x, y1)
    tree2.fit(x, y2) 

    # save
    
    path = '{0}/{1}'.format(directory, point)

    write_tree(path, 'x', tree1)
    write_tree(path, 'y', tree2)