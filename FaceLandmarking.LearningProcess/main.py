import numpy as np
import random
from data_reader import import_face_data
from sklearn.neural_network import MLPRegressor
import math

num_of_features = 23
num_of_hidden_nodes = 5
num_of_decisions = 2
data_path = '../Data/features'

data = import_face_data(num_of_features, num_of_decisions, data_path, exact='0')

def sigmoid(x):
    return 1 / (1 + np.exp(-x))


def sigmoid_deriv(x):
    return x * (1 - x)


for point, examples in data.items():
    examples = list(zip(examples['x'], examples['y']))
    random.shuffle(examples)

    x, y = ([ x for x,y in examples ], [ y for x,y in examples ])

    #y = [[math.sqrt(math.sqrt(element)) if element > 0 else -math.sqrt(math.sqrt(-element)) for element in line] for line in y];
    y = [[max(-1, min(1, element)) for element in line] for line in y];

    x = np.array(x)
    y = np.array(y)
    
    x = (x - x.min(axis=0)) / (x.max(axis=0) - x.min(axis=0))
    y = (y - y.min(axis=0)) / (y.max(axis=0) - y.min(axis=0))
    
    x_test = x[0:100]
    y_test = y[0:100]
    
    x = x[100:]
    y = y[100:]

    ######

    clf = MLPRegressor(solver='lbfgs', alpha=1e-6, hidden_layer_sizes=(5,), max_iter=100000)
    clf.fit(x, y) 
    
    predicted = clf.predict(x_test)
    
    err = np.array(predicted) - np.array(y_test)
    
    print("Error: " + str(np.mean(np.abs(err))))


    ######

    #syn0 = 0.5 * (np.random.random((num_of_features, num_of_hidden_nodes)) - 0.5)
    #bia0 = 0.2 * (np.random.random(num_of_hidden_nodes) - 0.5)
    #syn1 = 0.5 * (np.random.random((num_of_hidden_nodes, num_of_decisions)) - 0.5)
    #
    #for j in range(100000):
    #
    #    l0 = x
    #    l1 = sigmoid(np.dot(l0,syn0)) + bia0
    #    l2 = sigmoid(np.dot(l1,syn1))
    #
    #    l2_error = y - l2
    #    l2_delta = l2_error * sigmoid_deriv(l2)
    #
    #    l1_error = l2_delta.dot(syn1.T)
    #    l1_delta = l1_error * sigmoid_deriv(l1)
    #
    #    l0_test = x_test
    #    l1_test = sigmoid(np.dot(l0_test, syn0)) + bia0
    #    l2_test = sigmoid(np.dot(l1_test, syn1))
    #    l2_test_error = y_test - l2_test
    #
    #    if (j % 100) == 0:
    #        print("Error: " + str(np.mean(np.abs(l2_error))) + ", " + str(np.mean(np.abs(l2_test_error))))
    #        
    #    syn0 += l0.T.dot(l1_delta * 0.001)
    #    bia0 += np.sum(l1_delta * 0.001)
    #    syn1 += l1.T.dot(l2_delta * 0.001)
