import csv
from os.path import join
import numpy as np


def import_face_data(file_path):
    examples = [];

    with open(file_path, mode='r', encoding='utf-8') as file:
        reader = csv.reader(file, delimiter=' ')


        for row in reader:
            examples.append(np.array([float(value) for value in row if value]))

    return examples