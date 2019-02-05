import csv


def read_regressor_examples(num_of_features, num_of_decisions, file_path):
    xs = []
    ys = []

    with open(file_path, mode='r', encoding='utf-8') as file:
        reader = csv.reader(file, delimiter=' ')

        for row in reader:
            x = [float(value) for value in row[0 : num_of_features]]
            y = [float(value) for value in row[num_of_features : num_of_features + num_of_decisions]]

            xs.append(x)
            ys.append(y)

    return {
        'x': xs,
        'y': ys
    }
