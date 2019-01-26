import os

def write_nn(path, file_name, nn):
    if not os.path.exists(path):
        os.makedirs(path)

    with open(os.path.join(path, file_name), 'w') as file:
        biases = nn.intercepts_ 
        weights = nn.coefs_

        layers = len(biases) + 1
        file.write("{0}\n".format(layers))
        
        file.write("{0} ".format(len(weights[0] / biases[0])))
        for layer_biases in biases:
            file.write("{0} ".format(len(layer_biases)))
        file.write("\n")

        for i in range(layers - 1):

            for node_weights in weights[i]:
                for weight in node_weights:
                    file.write("{0:0.5f} ".format(weight))

            file.write("\n")

            for bias in biases[i]:
                file.write("{0:0.7f} ".format(bias))

            file.write("\n")