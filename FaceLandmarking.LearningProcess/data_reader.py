import csv
from os import listdir
from os.path import join


def import_face_data(num_of_features, num_of_decisions, data_path, exact=None):
    files = listdir(data_path)

    if (exact != None):
        files = [file for file in files if file == exact]

    landmarks = {}

    for file_name in files:
        xs = []
        ys = []

        file_path = join(data_path, file_name)
        with open(file_path, mode='r', encoding='utf-8') as file:
            reader = csv.reader(file, delimiter=' ')

            for row in reader:
                x = [float(value) for value in row[0 : num_of_features]]
                y = [float(value) for value in row[num_of_features : num_of_features + num_of_decisions]]

                xs.append(x)
                ys.append(y)

        landmarks[file_name] = {
                'x': xs,
                'y': ys
            }

    return landmarks
