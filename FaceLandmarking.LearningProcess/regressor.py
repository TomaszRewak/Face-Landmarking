import numpy as np
import shutil
import pathlib
from sklearn.tree import DecisionTreeRegressor
from readers.regressor_example_reader import read_regressor_examples
from writers.tree_writer import write_tree
from os.path import join
from os import listdir


out_directory = '../Data/regressors/trees'
shutil.rmtree(out_directory, ignore_errors=True)
pathlib.Path(out_directory).mkdir(parents=True)

in_directory = '../Data/features';
files = listdir(in_directory)

for point in files:
    print('point: {0}'.format(point))

    in_path = join(in_directory, point)
    examples = read_regressor_examples(12 * 3, 2, in_path)

    x = examples['x']
    y = [[max(-1, min(1, element)) for element in line] for line in examples['y']]

    x_train = x[1000:]
    y_train = y[1000:]

    y1_train = np.array(y_train)[:,0]
    y2_train = np.array(y_train)[:,1]
    
    tree1 = DecisionTreeRegressor(max_depth=5)
    tree2 = DecisionTreeRegressor(max_depth=5)

    print('fitting...')

    tree1.fit(x_train, y1_train)
    tree2.fit(x_train, y2_train) 

    x_test = x[:1000]
    y_test = y[:1000]

    y1_test = np.array(y_test)[:,0]
    y2_test = np.array(y_test)[:,1]

    score1 = tree1.score(x_test, y1_test)
    print('score: {0}'.format(score1))

    score2 = tree2.score(x_test, y2_test)
    print('score: {0}'.format(score2))

    print('save...')
    
    out_path = join(out_directory, point)

    write_tree(out_path, 'x', tree1)
    write_tree(out_path, 'y', tree2)