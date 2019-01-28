import numpy as np
import shutil
import pathlib
from sklearn.neural_network import MLPRegressor
from autoencoder_example_reader import import_face_data
from nn_writer import write_nn

data = import_face_data('../Data/autoencoder/examples')

hidden_layer_sizes = (50) # 388
nn = MLPRegressor(hidden_layer_sizes = hidden_layer_sizes, 
    activation = 'relu')

x = data[0::2]
y = data[1::2]

set_size = len(x)
train_set_size = int(set_size * 9 / 10)

x_train = x[0:train_set_size]
y_train = y[0:train_set_size]
nn.fit(x_train, y_train)

x_test = data[train_set_size:]
y_test = data[train_set_size:]
score = nn.score(x_test, y_test)

print(score)

directory = '../Data/regressors/nn';
shutil.rmtree(directory, ignore_errors=True)
write_nn(directory, 'autoencoder', nn)